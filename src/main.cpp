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

#include <threadSound.h>

#include <deplacement.h>
#include <herkulex.h>

#include <global.h>
#include <Instruction.h>
#include <Strategie.h>
#include <debug.h>

#include <sstream>
#include <fstream>

#include <lvgl.h>

ThreadCAN threadCAN;
ThreadSD threadSD;
ThreadLvgl threadLvgl;
Ihm ihm(&threadLvgl);
Deplacement deplacement(threadCAN);
herkulex Herkulex(threadCAN);

vector<string> fichiers;

DigitalIn jackPin(D7);

Thread *recalage;
void runRecalage();
int recalageErreur; // 0 si recalage réussi, valeur négative sinon

Thread *match; 
void runMatch();
bool jack();
Timeout timerMatch;
int tempsRestant();
void forceFinMatch();
volatile bool flagForceFinMatch = false;
bool Recalage = true;

bool listeFichiers();
bool lectureFichier(int choix);

struct S_Instruction strat_instructions[SIZE_BUFFER_FILE]; // La liste des instruction chargé en mémoire
unsigned char nb_instructions;                             // Le nombre d'instruction dans le fichier de strategie
unsigned char actual_instruction = 0;                      // La ligne de l'instruction en cours d'execution
unsigned char InversStrat = 1;                             // Si à 1, indique que l'on part de l'autre cote de la table(inversion des Y)
unsigned short waitingAckID = 0;                           // L'id du ack attendu
unsigned short waitingAckFrom = 0;                         // La provenance du ack attendu
int waitingAckID_FIN;
int waitingAckFrom_FIN;
unsigned short waitingId = 0;
E_stratGameEtat gameEtat = ETAT_GAME_INIT;
EventFlags flag;
struct S_Instruction instruction;
bool machineStrategie();
E_Stratposdebut etat_pos = RECALAGE_1;
bool machineRecalage();
void process_instructions(struct S_Instruction instruction);

int main()
{
  char buf[100];

  threadSD.registerCANControl(threadCAN);
  threadCAN.registerIds(0x01, 0x7FF, canProcessRx);
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
  ihm.recalagePositionInit();

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
        ihm.msgBoxInit("Lecture de la stratégie\n", "En cours", false);
        ThisThread::sleep_for(1s);
        etat = 10;
      }
      else if (ihm.recalage_HautGaucheClicked())
      {
        // printf("recalage_HautGaucheClicked\n");
        Hauteur = ROBOT_EN_HAUT;
        Cote = ROBOT_A_GAUCHE;
      }
      else if (ihm.recalage_BasGaucheClicked())
      {
        // printf("recalage_BasGaucheClicked\n");
        Hauteur = ROBOT_EN_BAS;
        Cote = ROBOT_A_GAUCHE;
      }
      else if (ihm.recalage_HautDroitClicked())
      {
        // printf("recalage_HautDroitClicked\n");
        Hauteur = ROBOT_EN_HAUT;
        Cote = ROBOT_A_DROITE;
      }
      else if (ihm.recalage_BasDroitClicked())
      {
        // printf("recalage_BasDroitClicked\n");
        Hauteur = ROBOT_EN_BAS;
        Cote = ROBOT_A_DROITE;
      }
      else if(ihm.ActivationRecalageClicked()){
        if(Recalage){Recalage = false;}
        else {Recalage = true;}
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
      else if(!Recalage){
        ihm.msgBoxClose();
        ihm.msgBoxJackInit();
        etat = 2;
      }
      else
      {
        // si fin recalage :
        if (recalage->get_state() == Thread::Deleted)
        {
          delete recalage;
          recalage = nullptr;
          ihm.msgBoxClose();
          if (recalageErreur == 0)
          {
            ihm.msgBoxJackInit();
            etat = 2;
          }
          else
          {
            sprintf(buf, "Problème recalage %d", recalageErreur);
            ihm.msgBoxInit("Erreur\n", buf, true);
            etat = 5;
          }
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
        timerMatch.attach(forceFinMatch, std::chrono::seconds(tempsAffiche));
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
          int t = tempsRestant() + 1;
          if (tempsAffiche != t)
          {
            tempsAffiche = t;
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
      sprintf(buf, "Terminé\n\nScore = %d", SCORE_GLOBAL);
      ihm.msgBoxMessage(buf);
      etat = 5;
      break;

    case 5:
      if (ihm.msgBoxCancelClicked())
      {
        etat = 0;
      }
      break;

    case 10:
      if (lectureFichier(choix))
      {
        etat = 1;
        if(Recalage){
          // démarrer le recalage
          recalage = new Thread;
          recalage->start(runRecalage);
          ihm.msgBoxClose();
          ihm.msgBoxInit("Recalage en cours\n", "Attente\n", true);
        }
      }
      else
      {
        etat = 5;
        ihm.msgBoxClose();
        ihm.msgBoxInit("Erreur\n", "Problème fichier\n", true);
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
  string reply = threadSD.cdName("/");
  // string reply = threadSD.cdName("/strategie");
  // Vérifie que le dossier "/strategie" existe
  // if (reply.find("/strategie") != 0)
  // {
  //   return false;
  // }
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
  struct S_Instruction instruction_ligne;
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
      instruction_ligne = stringToInstruction((char *)(ligne.c_str()));
      strat_instructions[nb_instructions] = instruction_ligne;
      nb_instructions++;
      // debug_Instruction(strat_instructions[nb_instructions]);
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
  recalageErreur = 0;
  deplacement.asservOn();
  Herkulex.stepMotorMode(1);
  wait_us(50 * 1000);
  actual_instruction = 0;
  instruction = strat_instructions[actual_instruction];
  // deplacement.asservOn();
  while (machineRecalage());

  /*
  for (int i = 1; i <= 5; i++)
  {
    ThisThread::sleep_for(1s);
    //printf("R%d\n", i);
  }
  */
}

void runMatch()
{
  gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;

  // for (int i = 1; i <= 3; i++)
  // {
  //   ThisThread::sleep_for(1s);
  //   // printf("M%d\n", i);
  // }

  while (machineStrategie());
}

// Retourne true si le jack est retiré
bool jack()
{
  return (jackPin.read());
}

void forceFinMatch()
{
  flagForceFinMatch = true;
}

int tempsRestant()
{
  return std::chrono::duration_cast<std::chrono::seconds>(timerMatch.remaining_time()).count();
}

void process_instructions(struct S_Instruction instruction)
{

  switch (instruction.order)
  {
  case MV_RECALAGE:
  {
    if (instruction.nextActionType == MECANIQUE)
    {
      instruction.nextActionType = WAIT;
      int16_t distance = (((instruction.direction == FORWARD) ? 1 : -1) * 1000); // On indique une distance de 3000 pour etre sur que le robot va ce recaler
      uint8_t coordonnee = 0;
      uint16_t val_recalage;
      if (instruction.precision == RECALAGE_Y)
      {
        coordonnee = 2;
        // if (InversStrat == 1 && ingnorInversionOnce == 0)
        // {
        //   val_recalage = 3000 - instruction.arg1; // Inversion du Y
        // }
        // else
        // {
        val_recalage = instruction.arg1;
        // }
      }
      else
      {
        coordonnee = 1;
        val_recalage = instruction.arg1;
      }
      deplacement.recalage(distance, coordonnee, val_recalage);
      waitingAckID = ASSERVISSEMENT_RECALAGE;
      waitingAckFrom = ACKNOWLEDGE_MOTEUR;
      flag.wait_all(AckFrom_FLAG, 20000); // si jamais il y a un timeout, qu'est ce qu'il faut faire? retenter? arreter le match?

      waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
      waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
      flag.wait_all(AckFrom_FIN_FLAG, 20000);
    }
    else
    {
    }
  }
  break;
  case MV_TURN:
  {
    int16_t angle = instruction.arg3;
    target_x_robot = x_robot;
    target_y_robot = y_robot;
    target_theta_robot = theta_robot + angle;

    // if (InversStrat == 1 && ingnorInversionOnce == 0)
    // {
    //   angle = -angle;
    // }

    if (instruction.direction == ABSOLUTE)
    {
      // C'est un rotation absolu, il faut la convertir en relative

      angle = (angle - theta_robot) % 3600;
      if (angle > 1800)
        angle = angle - 3600;

      else if (angle < -1800)
        angle = angle + 3600;
    }

    waitingAckID = ASSERVISSEMENT_ROTATION;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    deplacement.rotation(angle);
    // printf("deplacement.rotation(angle : %d);\n", angle);
    flag.wait_all(AckFrom_FLAG, 20000);

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    flag.wait_all(AckFrom_FIN_FLAG, 20000);
  }
  break;
  case MV_LINE:
  {
    waitingAckID = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    // if (instruction.nextActionType == ENCHAINEMENT)
    // {
    //   MV_enchainement++;
    //   localData5 = 1;
    // }
    // else
    // {
    //   if (MV_enchainement > 0)
    //   { // Utilisé en cas d'enchainement,
    //     localData5 = 2;
    //     MV_enchainement = 0;
    //   }
    //   else
    //   {
    //     localData5 = 0;
    //   }
    // }
    int16_t distance = (((instruction.direction == FORWARD) ? 1 : -1) * instruction.arg1);
    // if (instruction.direction == FORWARD)
    //   asser_stop_direction = 1;
    // else
    //   asser_stop_direction = -1;
    // GoStraight(localData2, 0, 0, localData5);
    deplacement.toutDroit(distance);

    target_x_robot = x_robot + distance * cos((double)theta_robot * M_PI / 1800.0);
    target_y_robot = y_robot + distance * sin((double)theta_robot * M_PI / 1800.0);
    target_theta_robot = theta_robot;

    flag.wait_all(AckFrom_FLAG, 20000);

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    flag.wait_all(AckFrom_FIN_FLAG, 20000);
  }
  break;
  case MV_XYT:
  {
    // on effectue XYT normalement selon les instructions
    uint16_t x;
    uint16_t y;
    int16_t theta;
    uint8_t sens;
    if (instruction.direction == BACKWARD)
    {
      sens = -1;
      // asser_stop_direction = -1;
    }
    else
    {
      sens = 1;
      // asser_stop_direction = 1;
    }

    // if (InversStrat == 1 && ingnorInversionOnce == 0)
    // {
    //   localData2 = -instruction.arg3;
    //   localData3 = 3000 - instruction.arg2; // Inversion du Y
    // }
    // else
    // {
    y = instruction.arg2;
    theta = instruction.arg3;
    //}
    waitingAckID = ASSERVISSEMENT_XYT;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    if ((instruction.arg1 <= 0) || (y <= 0))
    {
      deplacement.positionXYTheta(target_x_robot, target_y_robot, target_theta_robot, sens);
    }
    else
    {
      deplacement.positionXYTheta(instruction.arg1, y, theta, sens);
    }

    target_x_robot = instruction.arg1;
    target_y_robot = y;
    target_theta_robot = theta;

    flag.wait_all(AckFrom_FLAG, 20000);

    waitingAckID_FIN = ASSERVISSEMENT_XYT;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    flag.wait_all(AckFrom_FIN_FLAG, 20000);
  }
  break;
  case MV_COURBURE:
  {
    int16_t rayon;
    int16_t angle;
    int8_t sens;

    float alpha = 0, theta = 0;
    short alph = 0;
    // actionPrecedente = MV_COURBURE;

    // if (instruction.nextActionType == ENCHAINEMENT)
    // {
    //   MV_enchainement++;
    //   localData5 = 1;
    // }
    // else
    // {
    //   if (MV_enchainement > 0)
    //   {
    //     localData5 = 2;
    //     MV_enchainement = 0;
    //   }
    //   else
    //   {
    //     localData5 = 0;
    //   }
    // }

    // if (InversStrat == 1 && ingnorInversionOnce == 0)
    // {
    //   if (instruction.direction == LEFT)
    //     instruction.direction = RIGHT;
    //   else
    //     instruction.direction = LEFT;
    // }

    sens = ((instruction.direction == LEFT) ? 1 : -1);
    angle = instruction.arg3;
    /*if(InversStrat == 1 && ingnorInversionOnce == 0) {
        localData1 = -localData1;//Inversion de la direction
    }*/
    enum E_InstructionDirection directionxyt;
    waitingAckID = ASSERVISSEMENT_COURBURE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    deplacement.courbure(instruction.arg1, angle, sens);

    alph = angle;
    if (angle > 0)
    {
      direction = 1;
    }
    else
    {
      direction = -1;
    }
    if (angle > 0)
    {
      directionxyt = FORWARD;
      asser_stop_direction = 1;
    }
    else
    {
      directionxyt = BACKWARD;
      asser_stop_direction = -1;
    }
    alpha = angle * M_PI / 1800.0f;
    theta = theta_robot * M_PI / 1800.0f;
    int nbre;
    nbre = abs(alph) / 450;
    for (int c = 0; c < nbre + 1; c++)
    {
      dodgeq.inst[c].order = MV_XYT;
      dodgeq.inst[c].direction = directionxyt;
      if (instruction.direction == LEFT)
      {                                                                                  //-------------LEFT
        target_x_robot = x_robot + instruction.arg1 * (sin(theta + alpha) - sin(theta)); // X
        target_y_robot = y_robot + instruction.arg1 * (cos(theta) - cos(theta + alpha)); // Y
        target_theta_robot = theta_robot + alph;                                         // T
        dodgeq.inst[c].arg1 = target_x_robot;                                            // X
        dodgeq.inst[c].arg2 = target_y_robot;                                            // Y
        dodgeq.inst[c].arg3 = target_theta_robot;                                        // T
      }
      else
      {
        target_x_robot = x_robot + instruction.arg1 * (sin(theta) - sin(theta - alpha)); // X
        target_y_robot = y_robot + instruction.arg1 * (cos(theta - alpha) - cos(theta)); // Y
        target_theta_robot = theta_robot - alph;                                         // T
        dodgeq.inst[c].arg1 = target_x_robot;                                            // X
        dodgeq.inst[c].arg2 = target_y_robot;                                            // Y
        dodgeq.inst[c].arg3 = target_theta_robot;                                        // T
      }
      alpha -= alpha / ((float)nbre);
      alph -= alph / (nbre + 1);
    }

    flag.wait_all(AckFrom_FLAG, 20000);

    waitingAckID_FIN = ASSERVISSEMENT_COURBURE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;

    flag.wait_all(AckFrom_FIN_FLAG, 20000);
  }
  break;
  // case MV_BEZIER:
  // {
  // }
  // break;
  case PINCE:
  {
    uint8_t Etage = (instruction.arg1 & 0xFF);
    uint8_t etatHerkulex = ((instruction.arg2 == 1) ? 1 : 0);
    uint8_t sens = (instruction.arg3 & 0xFF);
    waitingAckID = IDCAN_PINCE;
    waitingAckFrom = ACKNOWLEDGE_ACTIONNEURS;
    // printf("Herkulex.controlePince(Etage : %d, etatHerkulex : %d, sens : %d);\n",Etage,etatHerkulex, sens);
    
    Herkulex.controlePince(Etage, etatHerkulex, sens);
    printf("Herkulex.controlePince(Etage : %d, etatHerkulex : %d, sens : %d);\n", Etage, etatHerkulex, sens);
    flag.wait_all(AckFrom_FLAG, 20000);

    waitingAckID_FIN = IDCAN_PINCE;
    waitingAckFrom_FIN = INSTRUCTION_END_PINCE;
    flag.wait_all(AckFrom_FIN_FLAG, 20000);

    // else if(instruction.arg1 == 172){
    //    uint8_t etatPinceArriere = instruction.arg2;
    //    short position[2]={272, 454};
    //    if(etatPinceArriere){position[0] = 786; position[1] = 592;}

    //   waitingAckID = IDCAN_HERKULEX;
    //   waitingAckFrom = ACKNOWLEDGE_ACTIONNEURS;
    //   Herkulex.controleHerkulexPosition_Mul_ensemble(2, position[0], 3, position[1]);

    //   flag.wait_all(AckFrom_FLAG, 20000);

    //   waitingAckID_FIN = IDCAN_HERKULEX;
    //   waitingAckFrom_FIN = INSTRUCTION_END_HERKULEX;
    //   flag.wait_all(AckFrom_FIN_FLAG, 20000);

    // }
  }
  break;
    // case POSITION_DEBUT:
    // {
    // }
    // break;

  default:
    break;
  }

  if (!instruction.nextLineOK)
  {
    actual_instruction += 1;
  }
  else
  {
    actual_instruction = instruction.nextLineOK;
  }

  gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;
}

bool machineRecalage()
{
  switch (etat_pos)
  {
  case RECALAGE_1:
  {
    threadCAN.sendAck(RECALAGE_START, 0);
    int16_t distance = 1000;
    uint16_t val_recalage;

    if (Hauteur == ROBOT_EN_BAS)
    {
      val_recalage = 2000 - (MOITIEE_ROBOT);
    }
    else
    {
      val_recalage = MOITIEE_ROBOT;
      distance *= -1;
    }

    waitingAckID = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    deplacement.recalage(distance, 1, val_recalage);
    // printf("deplacement.recalage(distance : %d, 1, val_recalage : %d);\n", distance, val_recalage);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECALAGE_1, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -1;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECALAGE_1, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -2;
      return false;
    }

    etat_pos = RECULER_1;
  }
  break;
  case RECULER_1:
  {
    waitingAckID = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    int16_t distance = 100;
    if (Hauteur == ROBOT_EN_BAS)
    {
      distance *= -1;
    }
    deplacement.toutDroit(distance);
    // printf("deplacement.toutDroit(distance : %d);\n", distance);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECULER_1, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -3;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECULER_1, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -4;
      return false;
    }
    etat_pos = TOURNER;
  }
  break;
  case TOURNER:
  {
    waitingAckID = ASSERVISSEMENT_ROTATION;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    int16_t angle = 900;
    if (Cote == ROBOT_A_GAUCHE)
    {
      angle = 900;
    }
    else if (Cote == ROBOT_A_DROITE)
    {
      angle = -900;
    }

    deplacement.rotation(angle);
    // printf("deplacement.rotation(angle : %d);\n", angle);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, TOURNER, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -5;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_ROTATION;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, TOURNER, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -6;
      return false;
    }
    etat_pos = RECALAGE_2;
  }
  break;
  case RECALAGE_2:
  {
    int16_t distance = -1000;
    uint16_t val_recalage;

    if (Cote == ROBOT_A_DROITE)
    {
      val_recalage = 3000 - (MOITIEE_ROBOT);
    } // par devant
    else
    {
      val_recalage = MOITIEE_ROBOT;
    } // par derriere

    waitingAckID = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    deplacement.recalage(distance, 2, val_recalage);
    // printf("deplacement.recalage(distance : %d, 2, val_recalage : %d);\n", distance, val_recalage);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECALAGE_2, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -7;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECALAGE_2, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -8;
      return false;
    }

    etat_pos = RECULER_2;
  }
  break;
  case RECULER_2:
  {
    waitingAckID = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    int16_t distance = 100;
    deplacement.toutDroit(distance);
    // printf("deplacement.toutDroit(distance : %d);\n", distance);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECULER_2, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -9;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage fail, RECULER_2, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -10;
      return false;
    }
    etat_pos = GOTOPOS;
  }
  break;
  case GOTOPOS:
  {
    waitingAckID = ASSERVISSEMENT_XYT;
    waitingAckFrom = ACKNOWLEDGE_MOTEUR;
    deplacement.positionXYTheta(instruction.arg1, instruction.arg2, instruction.arg3, 0);
    // printf("deplacement.positionXYTheta(instruction.arg1 : %d, instruction.arg2 : %d, instruction.arg3 : %d, 0);\n", instruction.arg1, instruction.arg2, instruction.arg3);
    if (flag.wait_all(AckFrom_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage, GOTOPOS, waitingAckID\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -11;
      return false;
    }

    waitingAckID_FIN = ASSERVISSEMENT_XYT;
    waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
    if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osErrorTimeout)
    {
      // printf("osErrorTimeout, recalage, GOTOPOS, waitingAckID_FIN\n");
      gameEtat = ETAT_GAME_INIT;
      recalageErreur = -12;
      return false;
    }
    etat_pos = FIN_POS;
  }
  break;
  case FIN_POS:
  {
    actual_instruction = instruction.nextLineOK;
    target_x_robot = x_robot;
    target_y_robot = y_robot;
    target_theta_robot = theta_robot;

    gameEtat = ETAT_GAME_WAIT_FOR_JACK;
    return false;
  }
  break;

  default:
    break;
  }
  return true;
}

bool machineStrategie()
{
  switch (gameEtat)
  {
  case ETAT_GAME_LOAD_NEXT_INSTRUCTION:
    // printf("load next instruction\n");
    if (actual_instruction >= nb_instructions || actual_instruction == 255)
    {
      gameEtat = ETAT_END;
      // Il n'y a plus d'instruction, fin du jeu
    }
    else
    {
      instruction = strat_instructions[actual_instruction];
      // On effectue le traitement de l'instruction
      gameEtat = ETAT_GAME_PROCESS_INSTRUCTION;
    }
    break;

  case ETAT_GAME_PROCESS_INSTRUCTION:
  {

    //      Traitement de l'instruction, envoie de la trame CAN
    debug_Instruction(instruction);
    process_instructions(instruction);
  }
  break;

  case ETAT_END:
  {
    // printf("GAME ENDED\n");
    return false;
  }
  break;

  default:
    break;
  }
  return true;
}
