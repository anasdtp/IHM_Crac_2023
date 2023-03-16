#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include <cstring>
#include "threadSD.h"
#include "threadCAN.h"
//#include "global.h"
#include "ihm.h"

LCD_DISCO_F469NI lcd(LCD_ORIENTATION_LANDSCAPE);
TS_DISCO_F469NI ts;

DigitalOut led (LED1);

/*
void return_error (int ret_val)
{
    char buf[30];
    if (ret_val) {
        sprintf(buf, "Fail %d", ret_val);
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
    } else {
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Done", CENTER_MODE);
    }
}

void errno_error(void* ret_val)
{
    char buf[30];
    if (ret_val == NULL) {
        sprintf(buf, "Fail %d", errno);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)buf, CENTER_MODE);
    } else {
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"Done", CENTER_MODE);
    }
}
*/

ThreadCAN threadCAN(false);
ThreadSD threadSD;

int main ()
{
//    threadSD.registerCANControl(threadCAN);

    char buf[100];

    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"DETECTION CARTE SD", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"SD INIT", CENTER_MODE);

    int secReboot = 60;
    while (1) {
        int flag;
        flag = threadSD.status();
        sprintf(buf, "flag = %04X", flag);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)buf, CENTER_MODE);
        if (flag & ThreadSD::FLAG_NO_CARD) sprintf(buf, "Carte SD absente"); else sprintf(buf, "Carte SD presente");
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)buf, CENTER_MODE);
        sprintf(buf, "Reboot dans %2ds", secReboot);
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
        if (flag & ThreadSD::FLAG_READY) break;
        ThisThread::sleep_for(1s);
        if (secReboot-- <= 0) NVIC_SystemReset();
    }

    sprintf(buf, "Liste des fichiers");
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
    // Attend que la carte SD soit prête
    threadSD.waitReady();
    // Liste les dossiers et fichiers présents sur la carte
    threadSD.ls();
    // Récupère le résultat sous la forme |dossier1|dossier2:fichier1:fichier2:fichier3?   | pour dossier  : pour fichier  ? pour fin
    string txt(threadSD.getReply());
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)txt.c_str(), CENTER_MODE);
    // Enlève le ? à la fin
    if (!txt.empty()) txt.pop_back();
    vector <string> fichiers;
    stringstream txtStream(txt);
    string item;
    // Ignore tous les dossiers
    if (getline (txtStream, item, ':')) {
        while (getline (txtStream, item, ':')) {
            // Range chaque nom de fichier dans un tableau de string
            fichiers.push_back (item);
        }
    }

    int line = 10;
    for (string fic : fichiers) {
        lcd.DisplayStringAt(0, LINE(line++), (uint8_t *)fic.c_str(), CENTER_MODE);
    }

    if (fichiers.empty()) {
        sprintf(buf, "Aucun fichier sur la carte");
        lcd.DisplayStringAt(0, LINE(line++), (uint8_t *)buf, CENTER_MODE);
        while (1) {
            sprintf(buf, "Reboot dans %2ds", secReboot);
            lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
            ThisThread::sleep_for(1s);
            if (secReboot-- <= 0) NVIC_SystemReset();
        }
    }

    while (1) {
        ThisThread::sleep_for(1s);
//        automate_etat_ihm();
//         Strategie();//Boucle dans l'automate principal
//         //gestion_Message_CAN();   
//         canProcessRx();  
    }

    /*
        sprintf(buf, "Format\r\n");
        pc.write(buf, strlen(buf));
        error = FATFileSystem::format(&bd);
        return_error(error);
        sprintf(buf, "  Error = %d\r\n", error);
        pc.write(buf, strlen(buf));
    */
//
    /*
        lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"TOUCHSCREEN SD DEMO", CENTER_MODE);
        lcd.SetFont(&Font12);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"Start", CENTER_MODE);
        wait(1);
    */
    /*
        // Format
        error = FATFileSystem::format(&bd);
        return_error(error);

    // Mount
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"Mounting the filesystem on \"/fs\". ", CENTER_MODE);
    error = fs.mount(&bd);
    return_error(error);

        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"Opening a new file, numbers.txt.", CENTER_MODE);
        fd = fopen("/fs/numbers.txt", "w");
        errno_error(fd);

        for (int i = 0; i < 20; i++) {
            sprintf(buf, "Writing decimal numbers to a file (%2d/20)", i+1);
            lcd.DisplayStringAt(0, LINE(5), (uint8_t *)buf, CENTER_MODE);
            fprintf(fd, "%d\r\n", i);
        }

        fclose(fd);
        printf(" done.\r\n");
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"File closed", CENTER_MODE);
    */
    /*    fd = fopen("/fs/numbers.txt", "r");
        errno_error(fd);

        printf("Dumping file to screen.\r\n");
        char buff[16] = { 0 };
        while(!feof (fd)) {
            int size = fread(buff, 1, 1, fd);
    //        pc.putc(buff[0]);
    //        fwrite(&buff[0], 1, size, stdout);
        }
        pc.printf("EOF.\r\n");

        pc.printf("Closing file.");
        fclose(fd);
        pc.printf(" done.\r\n");

        pc.printf("Opening root directory.");
        DIR* dir = opendir("/fs/");
        errno_error(fd);

        struct dirent* de;
        pc.printf("Printing all filenames:\r\n");
        while((de = readdir (dir)) != NULL) {
            pc.printf("  %s\r\n", de->d_name);
        }

        pc.printf("Closeing root directory. ");
        error = closedir(dir);
        return_error(error);
        pc.printf("Filesystem Demo complete.\r\n");

        // Blink led with 2 Hz
        while(true) {
            led = !led;
            wait (0.5);
        }*/
}



// CAN can1(PB_8,PB_9); // Rx&Tx pour le CAN
// CAN can2(PB_5, PB_13);


// CANMessage msgRxBuffer[SIZE_FIFO];
// unsigned char FIFO_ecriture;
// char strat_sd[10][SIZE+8];
// char PATH[10][SIZE+8];
// Serial pc(USBTX,USBRX);
// char cheminFileStart[SIZE+8]; //Le chemin du fichier de strat, utiliser strcpy(cheminFileStart,"/local/strat.txt");

// struct S_Instruction strat_instructions[SIZE_BUFFER_FILE]; //La liste des instruction chargé en mémoire
// unsigned char nb_instructions; //Le nombre d'instruction dans le fichier de strategie
// unsigned char actual_instruction;//La ligne de l'instruction en cours d'execution
// unsigned char InversStrat = 1;//Si à 1, indique que l'on part de l'autre cote de la table(inversion des Y)
// unsigned short waitingAckID=0;//L'id du ack attendu
// unsigned short waitingAckFrom=0;//La provenance du ack attendu
// unsigned short waitingId=0;
//     char can_present=1;

// char modeTelemetre; // Si à 1, indique que l'on attend une reponse du telemetre
// DigitalOut led1(LED1);//Led d'indication de problème, si elle clignote, c'est pas bon
// DigitalOut led2(LED2);//Led d'indication de problème, si elle clignote, c'est pas bon
// DigitalOut led3(LED3);//Led d'indication de problème, si elle clignote, c'est pas bon
// DigitalOut led4(LED4);//Led d'indication de problème, si elle clignote, c'est pas bon
// void bluetooth_init(void);
// Serial rn42_pr(PG_14, PG_9);
// //Serial rn42(PA_1,PA_2);
// Serial rn42_Tx(PA_2,NC);
// Serial rn42_Rx(NC,PA_1);
// LiaisonBluetooth liaison_Rx(&rn42_Rx,&pc);
// LiaisonBluetooth liaison_Tx(&rn42_Tx,&pc);
// LiaisonBluetooth liaison_pr(&rn42_pr,&pc);
// /****************************************************************************************/
// /* FUNCTION NAME: canRx_ISR                                                             */
// /* DESCRIPTION  : Interruption en réception sur le CAN                                  */
// /****************************************************************************************/
// void canRx_ISR (void)
// {
//     if (can2.read(msgRxBuffer[FIFO_ecriture])) {
//         //if(msgRxBuffer[FIFO_ecriture].id==RESET_STRAT) mbed_reset();
//         /*else*/ FIFO_ecriture=(FIFO_ecriture+1)%SIZE_FIFO;
//         //canProcessRx();
//     }
// }


// /**********************************************************************************/
// /* FUNCTION NAME: main                                                            */
// /* DESCRIPTION  : Fonction principal du programme                                 */
// /**********************************************************************************/
// int main() {
//     can1.frequency(1000000); // fréquence de travail 1Mbit/s
//     can2.attach(&canRx_ISR); // création de l'interrupt attachée à la réception sur le CAN
//     can2.frequency(1000000);
//     bluetooth_init();
//     SendRawId(ALIVE_IHM);
//     lcd.DisplayStringAt(0, 0,(uint8_t *)"Initialisation", LEFT_MODE);
//     led1 = 1;
//     lecture_fichier(); //bloquant si pas de carte SD
//     led1 = 0;
//     wait_ms(2000);//Attente pour que toutes les cartes se lancent et surtout le CANBlue
//     while(true) {
//         automate_etat_ihm();
//         Strategie();//Boucle dans l'automate principal
//         //gestion_Message_CAN();   
//         canProcessRx();  
//     }
// }
// void bluetooth_init(void){
//     rn42_Tx.baud(115200);
//     rn42_Rx.baud(115200);
//     rn42_pr.baud(115200);
//     pc.baud(115200);
//     pc.printf("ok1");
//     /*while(1){ // sert au paramètrage des module RN42
//         while(pc.readable()){
//             rn42_Tx.putc(pc.getc());
            
//         }
//         while(rn42_Rx.readable()){
//             pc.putc(rn42_Rx.getc());
//         }
//     }*/
// }
