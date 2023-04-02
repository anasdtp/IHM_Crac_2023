/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

#include <FATFileSystem.h>

#include <SDIOBlockDevice.h>

#include <threadCAN.h>
#include <threadSD.h>
#include <threadLvgl.h>
#include <deplacement.h>

#include <sstream>
#include <fstream>

#include <lvgl.h>

ThreadCAN threadCAN;
ThreadSD threadSD;
ThreadLvgl threadLvgl;
Ihm ihm(&threadLvgl);
Deplacement deplacement(threadCAN);

vector<string> fichiers;

Thread *recalage;
void runRecalage();

Thread *match;
void runMatch();
int score = 100;
bool jack();
Timeout timer;
void forceFinMatch();
volatile bool flagForceFinMatch = false;

bool listeFichiers();
bool lectureFichier(int choix);

int main()
{
  char buf[100];

  threadSD.registerCANControl(threadCAN);

  int secReboot = 60;
  while (1)
  {
    int flag;
    flag = threadSD.status();
    if (flag & ThreadSD::FLAG_NO_CARD)
    {
      sprintf(buf, "Reboot dans %2ds", secReboot);
      ihm.sdMsg("Carte SD absente", buf);
    }
    else
    {
      ihm.sdMsg("Carte SD présente");
    }
    if (flag & ThreadSD::FLAG_READY)
      break;
    ThisThread::sleep_for(1s);
    if (secReboot-- <= 0)
      NVIC_SystemReset();
  }

  if (!listeFichiers())
  {
    ihm.sdMsg(nullptr, "Dossier /strategie non trouvé");
  }
  else if (fichiers.size() == 0)
  {
    ihm.sdMsg(nullptr, "Aucun fichier trouvé");
  }
  else
  {
    int nb = fichiers.size();
    if (nb == 1)
    {
      sprintf(buf, "Un fichier trouvé");
    }
    else
    {
      sprintf(buf, "%d fichiers trouvés", nb);
    }
    ihm.sdMsg(nullptr, buf);
  }
  ihm.matchInit(fichiers);

  int etat = 0;
  int choix = -1;
  int tempsAffiche;

  while (1)
  {
    switch (etat)
    {
    case 0:
      if (ihm.departClicked())
      {
        choix = ihm.choixStrategie();
        if (choix == -1)
        {
          ihm.msgBoxRecalageInit("[Stratégie par défaut]");
        }
        else
        {
          ihm.msgBoxRecalageInit(fichiers[choix].c_str());
        }
      }
      else if (ihm.refreshSDClicked())
      {
        listeFichiers();
        ihm.matchRollerSetOptions(fichiers);
      }
      else if (ihm.recalageClicked())
      {
        ihm.msgBoxRecalageClose();
        if (lectureFichier(choix))
        {
          etat = 1;
          // démarrer le recalage
          recalage = new Thread;
          recalage->start(runRecalage);
          ihm.msgBoxInit("Recalage en cours\n", "Attente\n", true);
        }
        else
        {
          etat = 5;
          ihm.msgBoxInit("Erreur\n", "Problème fichier\n", true);
        }
      }
      break;

    case 1:
      if (ihm.msgBoxCancelClicked())
      {
        // annuler le recalage en cours
        recalage->terminate();
        delete recalage;
        recalage = nullptr;
        etat = 0;
      }
      else
      {
        // si fin recalage :
        if (recalage->get_state() == Thread::Deleted)
        {
          delete recalage;
          recalage = nullptr;
          ihm.msgBoxClose();
          ihm.msgBoxJackInit();
          etat = 2;
        }
      }
      break;

    case 2:
      if (ihm.jackAnnuleClicked())
      {
        etat = 0;
      }
      else if (ihm.jackSimuleClicked() || jack())
      {
        flagForceFinMatch = false;
        tempsAffiche = 100;
        timer.attach(forceFinMatch, std::chrono::seconds(tempsAffiche));
        // démarrer le match
        match = new Thread;
        match->start(runMatch);
        ihm.msgBoxJackClose();
        ihm.msgBoxInit("Match\n", "En cours\n", true);
        etat = 3;
      }
      break;

    case 3:
      // Le match a démarré
      if (ihm.msgBoxCancelClicked())
      {
        // Annuler le match en cours
        match->terminate();
        delete match;
        match = nullptr;
        etat = 0;
      }
      else
      {
        // Si fin du match
        if (flagForceFinMatch)
        {
          match->terminate();
          delete match;
          match = nullptr;
          etat = 4;
        }
        else if (match->get_state() == Thread::Deleted)
        {
          delete match;
          match = nullptr;
          etat = 4;
        }
        else
        {
          int tempsRestant = std::chrono::duration_cast<std::chrono::seconds>(timer.remaining_time()-1us).count()+1;
          if (tempsAffiche != tempsRestant)
          {
            tempsAffiche = tempsRestant;
            if (tempsAffiche > 0)
            {
              sprintf(buf, "Reste %d s", tempsAffiche);
              ihm.msgBoxMessage(buf);
            }
            else
            {
              ihm.msgBoxMessage("Terminé");
            }
          }
        }
      }
      break;

    case 4:
      // Affichage du score
      sprintf(buf, "Terminé\n\nScore = %d", score);
      ihm.msgBoxMessage(buf);
      etat = 5;
      break;

    case 5:
      if (ihm.msgBoxCancelClicked())
      {
        etat = 0;
      }
      break;
    }
    ThisThread::sleep_for(10ms);
  }
}

bool listeFichiers()
{
  // Vide la liste de fichiers
  fichiers.clear();
  // Attend que la carte SD soit prête
  threadSD.waitReady();
  // Se déplace dans le dossier "/strategie" et liste les fichiers présents
  string reply = threadSD.cdName("/strategie");
  // Vérifie que le dossier "/strategie" existe
  if (reply.find("/strategie") != 0)
  {
    return false;
  }
  // Récupère le résultat sous la forme /chemin*dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
  // Enlève le ? à la fin
  if (!reply.empty())
  {
    reply.pop_back();
  }
  stringstream txtStream(reply);
  string item;
  // Ignore tous les dossiers
  if (getline(txtStream, item, ':'))
  {
    while (getline(txtStream, item, ':'))
    {
      // Range chaque nom de fichier dans un tableau de string
      fichiers.push_back(item);
    }
  }
  return true;
}

bool lectureFichier(int choix)
{
  string ficStrat;
  if (choix < 0)
  {
    // Que faire si choix == -1 ????
    return false;
  }
  ficStrat = "/sd/strategie/" + fichiers[choix];
  ifstream monFlux(ficStrat); // Ouverture d'un fichier en lecture
  if (monFlux)
  {
    // Tout est prêt pour la lecture.
    string ligne;
    while (getline(monFlux, ligne))
    { // On lit une ligne complète
      printf("%s\n", ligne.c_str());
    }
    monFlux.close();
    return true;
  }
  // ERREUR: Impossible d'ouvrir le fichier en lecture
  // On fait la même chose que pour choix == -1 ????
  return false;
}

void runRecalage()
{
  for (int i = 1; i <= 5; i++)
  {
    ThisThread::sleep_for(1s);
    printf("R%d\n", i);
  }
}

void runMatch()
{
  for (int i = 1; i <= 20; i++)
  {
    ThisThread::sleep_for(1s);
    printf("M%d\n", i);
  }
}

// Retourne true si le jack est retiré
bool jack()
{
  return false;
}

void forceFinMatch()
{
  flagForceFinMatch = true;
}
