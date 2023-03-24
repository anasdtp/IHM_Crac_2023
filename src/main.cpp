/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

#include <FATFileSystem.h>

#include "SDIOBlockDevice.h"

#include "threadCAN.h"
#include "threadSD.h"
#include "threadLvgl.h"

#include <sstream>
#include <fstream>

#include "lvgl.h"
#include "lv_demo_widgets.h"

ThreadCAN threadCAN(false);
ThreadSD threadSD;

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void lv_example_btn_1(void)
{
    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
}

int main ()
{
    threadSD.registerCANControl(threadCAN);

    ThreadLvgl threadLvgl;

    threadLvgl.lock();
//    lv_example_btn_1();
    lv_demo_widgets();
    threadLvgl.unlock();

    while (1) {
      ThisThread::sleep_for(1s);
    }

//     char buf[100];

//     lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"DETECTION CARTE SD", CENTER_MODE);
//     lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"SD INIT", CENTER_MODE);

//     int secReboot = 60;
//     while (1) {
//         int flag;
//         flag = threadSD.status();
//         sprintf(buf, "flag = %04X", flag);
//         lcd.DisplayStringAt(0, LINE(4), (uint8_t *)buf, CENTER_MODE);
//         if (flag & ThreadSD::FLAG_NO_CARD) sprintf(buf, "Carte SD absente"); else sprintf(buf, "Carte SD presente");
//         lcd.DisplayStringAt(0, LINE(6), (uint8_t *)buf, CENTER_MODE);
//         sprintf(buf, "Reboot dans %2ds", secReboot);
//         lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
//         if (flag & ThreadSD::FLAG_READY) break;
//         ThisThread::sleep_for(1s);
//         if (secReboot-- <= 0) NVIC_SystemReset();
//     }

//     sprintf(buf, "Liste des fichiers");
//     lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
//     // Attend que la carte SD soit prête
//     threadSD.waitReady();
//     // Liste les dossiers et fichiers présents sur la carte
//     threadSD.ls();
//     // Récupère le résultat sous la forme *dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
//     string txt(threadSD.getReply());
//     lcd.DisplayStringAt(0, LINE(9), (uint8_t *)txt.c_str(), CENTER_MODE);
//     // Enlève le ? à la fin
//     if (!txt.empty()) txt.pop_back();
//     vector <string> fichiers;
//     stringstream txtStream(txt);
//     string item;
//     // Ignore tous les dossiers
//     if (getline (txtStream, item, ':')) {
//         while (getline (txtStream, item, ':')) {
//             // Range chaque nom de fichier dans un tableau de string
//             fichiers.push_back (item);
//         }
//     }

//     int line = 10;
//     for (string fic : fichiers) {
//         lcd.DisplayStringAt(0, LINE(line++), (uint8_t *)fic.c_str(), CENTER_MODE);
//     }

//     if (fichiers.empty()) {
//         sprintf(buf, "Aucun fichier sur la carte");
//         lcd.DisplayStringAt(0, LINE(line++), (uint8_t *)buf, CENTER_MODE);
//         while (1) {
//             sprintf(buf, "Reboot dans %2ds", secReboot);
//             lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
//             ThisThread::sleep_for(1s);
//             if (secReboot-- <= 0) NVIC_SystemReset();
//         }
//     }
// /*
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