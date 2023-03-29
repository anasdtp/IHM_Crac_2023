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

ThreadCAN threadCAN(false, PB_8, PB_9);
ThreadSD threadSD;
ThreadLvgl threadLvgl;
Ihm ihm(&threadLvgl);
Deplacement deplacement(threadCAN);

int main ()
{
    threadSD.registerCANControl(threadCAN);
 
    int secReboot = 60;
    while (1) {
        int flag;
        char buf[100];
        flag = threadSD.status();
        if (flag & ThreadSD::FLAG_NO_CARD) {
          sprintf(buf, "Reboot dans %2ds", secReboot);
          ihm.sdMsg("Carte SD absente", buf); 
        } else {
          ihm.sdMsg("Carte SD presente");
        }
        if (flag & ThreadSD::FLAG_READY) break;
        ThisThread::sleep_for(1s);
        if (secReboot-- <= 0) NVIC_SystemReset();
    }
    // Attend que la carte SD soit prête
    threadSD.waitReady();
    // Liste les dossiers et fichiers présents sur la carte
    threadSD.ls();
    // Récupère le résultat sous la forme *dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
    string txt(threadSD.getReply());
    // Enlève le ? à la fin
    if (!txt.empty()) txt.pop_back();
    vector <string> fichiers;
    stringstream txtStream(txt);
    string item;
    // Ignore tous les dossiers
    if (getline (txtStream, item, ':')) {
        while (getline (txtStream, item, ':')) {
            // Range chaque nom de fichier dans un tableau de string
            fichiers.push_back(item);
        }
    }
    ihm.matchInit(fichiers);

    while (1) {
      if (ihm.departClicked()) {
        if (ihm.choixCouleur() == Ihm::VERT) {
          printf("Départ vert ");
        } else {
          printf("Départ bleu ");
        }
        int choix = ihm.choixStrategie();
        if (choix == -1) {
          printf("par défaut\n");
        } else {
          printf("%s\n", fichiers[choix].c_str());
        }
      }
      ThisThread::sleep_for(10ms);
    }

// ifstream monFlux("/sd/main.cpp");  //Ouverture d'un fichier en lecture

// if(monFlux)
// {
//     //Tout est prêt pour la lecture.
//     string ligne;
//     while (getline(monFlux, ligne)) {//On lit une ligne complète
//         printf("%s\n", ligne.c_str());
//     }
//     monFlux.close();
// }
// else
// {
//     printf("ERREUR: Impossible d'ouvrir le fichier en lecture.\n");
// }
// */

//     while (1) {
//         ThisThread::sleep_for(1s);
// //        automate_etat_ihm();
// //         Strategie();//Boucle dans l'automate principal
// //         //gestion_Message_CAN();   
// //         canProcessRx();  
//     }
}

/*
// Blinking rate in milliseconds
#define BLINKING_RATE 500ms

int main() {
  // Initialise the digital pin LED1 as an output
  DigitalOut led(LED1);

  printf("start\n");
  SDIOBlockDevice *bd = nullptr;
  FATFileSystem m_fs("sd");
  if (bd == nullptr)
    bd = new SDIOBlockDevice();
  int i = 0;
  while (1) {
    printf("Tentative mount %3d\n", i++);
    int m_error = m_fs.mount(bd);
    if (m_error) {
      printf("Erreur\n");
      m_fs.unmount();
      delete bd;
      bd = nullptr;
      ThisThread::sleep_for(1s);
    } else {
      printf("Mount ok\n");
      ThisThread::sleep_for(500ms);
      break;
    }
  }

  Dir dir;
  int error;

  error = dir.open(&m_fs, "/");
  if (!error) {
    struct dirent de;
    while (dir.read(&de) > 0) {
      if (de.d_type == DT_DIR) {
        printf("Dir : %s\n", de.d_name);
      } else {
        printf("Fic : %s\n", de.d_name);
      }
    }
    //        dir.close();
  }

  File fichier;
  if (fichier.open(&m_fs, "main.cpp")) {
    printf("Impossible d'ouvrir le fichier !\n");
    while (1)
      ;
  }
  while (1) {
    char c;
    if (fichier.read(&c, 1) != 1)
      break;
    printf("%c", c);
  }
  fichier.close();

  while (true) {
    led = !led;
    ThisThread::sleep_for(BLINKING_RATE);
  }
}
*/