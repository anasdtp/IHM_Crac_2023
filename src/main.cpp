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

ThreadCAN threadCAN(true);
ThreadSD threadSD;
ThreadLvgl threadLvgl;
Ihm ihm(&threadLvgl);
Deplacement deplacement(threadCAN);

vector<string> fichiers;

Ticker timer;
int tempsRestant;
void timerInterrupt();

void listeFichiers();
void lectureFichier(int choix);

int main()
{
  threadSD.registerCANControl(threadCAN);

  int secReboot = 60;
  while (1)
  {
    int flag;
    char buf[100];
    flag = threadSD.status();
    if (flag & ThreadSD::FLAG_NO_CARD)
    {
      sprintf(buf, "Reboot dans %2ds", secReboot);
      ihm.sdMsg("Carte SD absente", buf);
    }
    else
    {
      ihm.sdMsg("Carte SD presente");
    }
    if (flag & ThreadSD::FLAG_READY)
      break;
    ThisThread::sleep_for(1s);
    if (secReboot-- <= 0)
      NVIC_SystemReset();
  }

  listeFichiers();
  ihm.matchInit(fichiers);

  int etat = 0;
  int choix = -1;

  while (1)
  {
    if (etat == 0)
    {
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
        ihm.msgBoxJackInit();
        lectureFichier(choix);
        etat = 1;
      }
    }
    else
    {
      if (ihm.jackAnnuleClicked())
      {
        etat = 0;
      }
      else if (ihm.jackSimuleClicked() /* || jack() */)
      {
        ihm.msgBoxJackClose();
        break;
      }
    }
    ThisThread::sleep_for(10ms);
  }

  tempsRestant = 1000; // temps en dixième de seconde
  timer.attach(timerInterrupt, 100ms);

  while (1)
  {
    ThisThread::sleep_for(1s);
    //        automate_etat_ihm();
    //         Strategie();//Boucle dans l'automate principal
    //         //gestion_Message_CAN();
    //         canProcessRx();
  }
}

void listeFichiers()
{
  // Attend que la carte SD soit prête
  threadSD.waitReady();
  // Se déplace dans le dossier "/strategie"
  threadSD.cdName("/strategie");
  // Liste les dossiers et fichiers présents sur la carte
  threadSD.ls();
  // Récupère le résultat sous la forme *dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
  string txt(threadSD.getReply());
  // Enlève le ? à la fin
  if (!txt.empty())
    txt.pop_back();
  fichiers.clear();
  stringstream txtStream(txt);
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
}

void lectureFichier(int choix)
{
  string ficStrat;
  if (choix >= 0)
    ficStrat = "/sd/strategie/" + fichiers[choix];
  // Que faire si choix == -1 ????
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
  }
  else
  {
    // ERREUR: Impossible d'ouvrir le fichier en lecture
    // On fait la même chose que pour choix == -1 ????
  }
}

void timerInterrupt()
{
  tempsRestant--;
}
