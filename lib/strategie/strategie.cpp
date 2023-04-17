#include <global.h>
#include <strategie.h>
#include <instruction.h>
#include <herkulex.h>
#include <debug.h>

extern Herkulex herkulex;
extern int recalageErreur;  // 0 si recalage réussi, valeur négative sinon

// // E_stratGameEtat gameEtat = ETAT_CHECK_CARTES;
// T_etat strat_etat_s = INIT;


    
// int waitingAckID_FIN;
// int waitingAckFrom_FIN;

// Ticker ticker;


// Ticker chrono;
// Timeout AffTime;
// Ticker timer_strat;
// Timer cartesCheker;//Le timer pour le timeout de la vérification des cartes
// Timer gameTimer;
// Timer debugetatTimer;
// Timer timeoutWarning;
// Timer timeoutWarningWaitEnd;
// Timeout chronoEnd;//permet d'envoyer la trame CAN pour la fin

// unsigned char screenChecktry = 0;
// unsigned char test[32] = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32};

signed char asser_stop_direction=0;
// char counter = 0;
// char check;
// char Jack = 1;
short SCORE_GLOBAL=0;
// short SCORE_GR=0;
// short SCORE_PR=0;
// unsigned short distance_recalage;
// unsigned short distance_revenir;

// unsigned short x;
// unsigned short y;
// unsigned char isStopEnable = 1;//Permet de savoir si il faut autoriser le stop via les balises
unsigned short flag_check_carte = 0; //, flag_strat = 0, flag_timer;
// int flagReceptionTelemetres = 0, flagNonRepriseErrorMot = 0;

// int Flag_Bras_Re = 0, Flag_Manche_Bas = 0, Flag_Manche_Moy = 0, Flag_pavillon = 0, Flag_bon_port = 0; //Flag utilisé pour compter le score des bras / manches / pavillon
// unsigned short Flag_num_bras;


signed short x_robot,y_robot,theta_robot;//La position du robot
signed short target_x_robot, target_y_robot, target_theta_robot;
// signed short avant_gauche, avant_droit;
// //E_InstructionType actionPrecedente;
// //unsigned char FIFO_ecriture=0; //Position du fifo pour la reception CAN
// int flagSendCan=1;
unsigned char Cote = 0; //0 -> JAUNE | 1 -> VIOLET
unsigned char Hauteur = 0; //Robot du haut -> 1, du bas -> 0
// unsigned short angleRecalage = 0;
// unsigned char checkCurrent = 0;
// unsigned char countAliveCard = 0;
// unsigned char ligne=0;
// int Fevitement=0;
// int EvitEtat= 0;
// int stop_evitement=0;
// signed char nbStrat = 0; //N° de la strategie (1-10)
// unsigned char ModeDemo = 0; // Si à 1, indique que l'on est dans le mode demo
// unsigned char countRobotNear = 0;//Le nombre de robot à proximité
// unsigned char ingnorBaliseOnce = 0;//une fois détecté réinitialise
// unsigned char ingnorBalise = 0;//0:balise ignore 1:on ecoute la balise
short direction;
// char val_girou ;

// unsigned char debug_bon_port = 0;

// unsigned char ingnorInversionOnce = 0;//Pour ignorer l'inversion des instruction une fois

// // S_Instruction instruction;
struct S_Dodge_queue dodgeq;

// char couleur1, couleur2, couleur3;
// float cptf;
// int cpt,cpt1;



// E_stratGameEtat     memGameEtat= gameEtat;
// E_stratGameEtat     lastEtat  = ETAT_CHECK_CARTES;
// E_Stratposdebut etat_pos=RECALAGE_1;


//void //SendRawId (unsigned short id);
//void can2Rx_ISR(void);

// signed char blocage_balise;
// void print_segment(int nombre, int decalage);
// void affichage_compteur (int nombre);
// void effacer_segment(long couleur);
// unsigned char doAction(unsigned char id, unsigned short arg1, short arg2);
// unsigned short telemetreDistance=0;
// unsigned short telemetreDistance_avant_gauche=0;
// unsigned short telemetreDistance_avant_droite=0;
// unsigned short telemetreDistance_arriere_gauche=0;
// unsigned short telemetreDistance_arriere_droite=0;

// unsigned char DT_AVD_interrupt=0;
// unsigned char DT_AVG_interrupt=0;
// unsigned char DT_ARD_interrupt=0;
// unsigned char DT_ARG_interrupt=0;



// unsigned short id_check[NOMBRE_CARTES]= {CHECK_MOTEUR,CHECK_BALISE};
// unsigned short id_alive[NOMBRE_CARTES]= {ALIVE_MOTEUR,ALIVE_BALISE};
// InterruptIn jackB1(PG_11); //  entrée numerique en interruption pour le jack (JackB1 sur la carte esclave)
// InterruptIn jackA1(PA_6);





/****************************************************************************************/
/* FUNCTION NAME: canProcessRx                                                          */
/* DESCRIPTION  : Fait évoluer l'automate de l'IHM en fonction des receptions sur le CAN*/
/****************************************************************************************/
void canProcessRx(CANMessage *rxMsg)
{
        int identifiant=rxMsg->id;
        //printCANMsg(*rxMsg);
        if (waitingId == identifiant) {waitingId = 0;}
        switch(identifiant) {
            case ALIVE_MOTEUR:{

            }
                
                break;

            case ALIVE_BALISE:
                {
                    
                }
                break;

            case RESET_IHM:
                // {strat_etat_s = CHOIX;}
                break;

            case DEBUG_FAKE_JAKE://Permet de lancer le match à distance
            case GLOBAL_JACK:{
                if(gameEtat == ETAT_GAME_WAIT_FOR_JACK) {
                    gameEtat = ETAT_GAME_START;
                    //SendRawId(ACKNOWLEDGE_JACK);
                }
            }
                break;

            case ALIVE_ACTIONNEURS_AVANT:    //pas de break donc passe directement dans ECRAN_ALL_CHECK mais conserve l'ident initial
            //case ALIVE_ACTIONNEURS_ARRIERE:
            case ALIVE_HERKULEX:
            case ECRAN_ALL_CHECK:{
                if(waitingAckFrom == rxMsg->id) {
                    waitingAckFrom = 0;//C'est la bonne carte qui indique qu'elle est en ligne
                }
                flag_check_carte=1;
            }
                break;
            case ASSERVISSEMENT_ERROR_MOTEUR: { //erreur asservissement

                //unsigned short recieveAckID= (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                //memcpy(&recieveAckID, rxMsg->data, 2);
                // if(recieveAckID == waitingAckID_FIN && waitingAckFrom_FIN == INSTRUCTION_END_MOTEUR) {
                //     if(flagNonRepriseErrorMot) {
                //         actual_instruction = instruction.nextLineError;
                //         gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;
                //         flagNonRepriseErrorMot = 0;
                //     } else {
                //         flagNonRepriseErrorMot = 1;
                //         timeoutWarningWaitEnd.reset();
                //         timeoutWarningWaitEnd.start();
                //         //gameEtat = ETAT_WARING_END_BALISE_WAIT;
                //     }
                // }
                /*
                if(flagNonRepriseErrorMot) {
                    actual_instruction = instruction.nextLineError;
                    gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;
                    flagNonRepriseErrorMot = 0;
                } else {
                    flagNonRepriseErrorMot = 1;
                    gameEtat = ETAT_WARNING_END_LAST_INSTRUCTION;
                }*/
            }
            break;

            /////////////////////////////////////Acknowledges de Reception de la demande d'action////////////////////////////////////////
            case ACKNOWLEDGE_HERKULEX:{

            }break;
            case ACKNOWLEDGE_BALISE:    //pas de break donc passe directement dans ACK_FIN_ACTION mais conserve l'ident initial
            {

            }break;
            case ACKNOWLEDGE_ACTIONNEURS:{
                unsigned short recieveAckID = (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                if( (waitingAckFrom == identifiant )&& (recieveAckID == waitingAckID) ) 
                {
                    printf(" ack de debut recu \n");
                    waitingAckFrom = 0;
                    waitingAckID = 0;
                    flag.set(AckFrom_FLAG);
                }
                if( (waitingAckFrom_FIN == identifiant ) && (recieveAckID == waitingAckID_FIN) ) {
                    printf(" ack de fin recu \n");
                    waitingAckFrom_FIN = 0;
                    waitingAckID_FIN = 0;
                    flag.set(AckFrom_FIN_FLAG);
                }
            }break;
            /////////////////////////////////////////////Acknowledges de la fin d'action/////////////////////////////////////////////////
            case ACKNOWLEDGE_MOTEUR:{
                ////printf("ACKNOWLEDGE_MOTEUR\n");
                unsigned short recieveAckID = (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                if( (waitingAckFrom == identifiant )&& (recieveAckID == waitingAckID) ) 
                {
                    ////printf(" ack de debut recu \n");
                    waitingAckFrom = 0;
                    waitingAckID = 0;
                    flag.set(AckFrom_FLAG);
                }
                if( (waitingAckFrom_FIN == identifiant ) && (recieveAckID == waitingAckID_FIN) ) {
                    //printf(" ack de fin recu \n");
                    waitingAckFrom_FIN = 0;
                    waitingAckID_FIN = 0;
                    flag.set(AckFrom_FIN_FLAG);
                }
            }break;
            case INSTRUCTION_END_PINCE:{
                unsigned short recieveAckID = (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                if( (waitingAckFrom == identifiant )&& (recieveAckID == waitingAckID) ) 
                {
                    printf(" ack de debut recu \n");
                    waitingAckFrom = 0;
                    waitingAckID = 0;
                    flag.set(AckFrom_FLAG);
                }
                if( (waitingAckFrom_FIN == identifiant ) && (recieveAckID == waitingAckID_FIN) ) {
                    printf(" ack de fin recu \n");
                    waitingAckFrom_FIN = 0;
                    waitingAckID_FIN = 0;
                    flag.set(AckFrom_FIN_FLAG);
                }
            }break;
            case ACK_FIN_ACTION:{

            }break;
            case INSTRUCTION_END_MOTEUR:
            {
                unsigned short recieveAckID = (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                //printf("INSTRUCTION_END_MOTEUR\n");
                //memcpy(&recieveAckID, rxMsg->data, 2);
                /*
                //on desactive la balise dans les rotations XYT
                if(rxMsg->id==ACKNOWLEDGE_MOTEUR && ASSERVISSEMENT_XYT==recieveAckID)ingnorBalise=1;
                if(rxMsg->id==INSTRUCTION_END_MOTEUR && ASSERVISSEMENT_XYT_ROTATE==recieveAckID)ingnorBalise=0;

                //on desactive la balise dans les rotations
                if(rxMsg->id==ACKNOWLEDGE_MOTEUR && ASSERVISSEMENT_ROTATION==recieveAckID)ingnorBalise=1;
                if(rxMsg->id==INSTRUCTION_END_MOTEUR && ASSERVISSEMENT_ROTATION==recieveAckID)ingnorBalise=0;
                */

                // SendMsgCan(0x666,&ingnorBalise,1);
                if( (waitingAckFrom == identifiant )&& (recieveAckID == waitingAckID) ) 
                {
                    //printf(" ack de debut recu \n");
                    waitingAckFrom = 0;
                    waitingAckID = 0;
                    flag.set(AckFrom_FLAG);
                }
                if( (waitingAckFrom_FIN == identifiant ) && (recieveAckID == waitingAckID_FIN) ) {
                    //printf(" ack de fin recu \n");
                    waitingAckFrom_FIN = 0;
                    waitingAckID_FIN = 0;
                    flag.set(AckFrom_FIN_FLAG);
                }

                /*
                               if((waitingAckFrom == rxMsg->id) &&
                               ((unsigned short)rxMsg->data[0]  |  (((unsigned short)rxMsg->data[1])<<8) == waitingAckID)  )
                               {
                                   waitingAckFrom = 0;
                                   waitingAckID = 0;
                               }
                               if(waitingAckFrom_FIN == rxMsg->id && ((unsigned short)rxMsg->data[0]
                               |(((unsigned short)rxMsg->data[1])<<8) == waitingAckID_FIN))
                               {
                                   waitingAckFrom_FIN = 0;
                                   waitingAckID_FIN = 0;
                               }
                     */
            }
                
                break;
            case ODOMETRIE_BIG_POSITION:{
                x_robot=rxMsg->data[0]|((unsigned short)(rxMsg->data[1])<<8);
                y_robot=rxMsg->data[2]|((unsigned short)(rxMsg->data[3])<<8);
                theta_robot=rxMsg->data[4]|((signed short)(rxMsg->data[5])<<8);
            }
                break;
            case ODOMETRIE_SMALL_POSITION:{
                x_robot=rxMsg->data[0]|((unsigned short)(rxMsg->data[1])<<8);
                y_robot=rxMsg->data[2]|((unsigned short)(rxMsg->data[3])<<8);
                theta_robot=rxMsg->data[4]|((signed short)(rxMsg->data[5])<<8);
            }
                break;

            case ACK_ACTION:{
                if(waitingAckID == rxMsg->id) {
                    waitingAckFrom = 0;
                    waitingAckID = 0;
                }
            }
                break;
            default:
                break;
        }
}

void printCANMsg(CANMessage& msg) {
    printf("  ID      = 0x%.3x\r\n", msg.id);
    printf("  Type    = %d\r\n", msg.type);
    printf("  format  = %d\r\n", msg.format);
    printf("  Length  = %d\r\n", msg.len);
    printf("  Data    =");            
    for(int i = 0; i < msg.len; i++){printf(" 0x%.2X", msg.data[i]);}
    printf("\r\n");
 }

void remplirStruct(CANMessage &theDATA, int idf, char lenf, char dt0f, char dt1f, char dt2f, char dt3f, char dt4f, char dt5f, char dt6f, char dt7f){
  theDATA.type = CANData;
  if(idf>0x7FF){theDATA.format = CANExtended;}
  else{theDATA.format = CANStandard;}
  theDATA.id = idf;
  theDATA.len = lenf;
  theDATA.data[0] = dt0f;
  theDATA.data[1] = dt1f;
  theDATA.data[2] = dt2f;
  theDATA.data[3] = dt3f;
  theDATA.data[4] = dt4f;
  theDATA.data[5] = dt5f;
  theDATA.data[6] = dt6f;
  theDATA.data[7] = dt7f;
}

void process_instructions(S_Instruction instruction) {
    switch (instruction.order) {
        case MV_RECALAGE: {
            if (instruction.nextActionType == MECANIQUE) {
                instruction.nextActionType = WAIT;
                int16_t distance = (((instruction.direction == FORWARD) ? 1 : -1) * 1000);  // On indique une distance de 3000 pour etre sur que le robot va ce recaler
                uint8_t coordonnee = 0;
                uint16_t val_recalage;
                if (instruction.precision == RECALAGE_Y) {
                    coordonnee = 2;
                    // if (InversStrat == 1 && ingnorInversionOnce == 0)
                    // {
                    //   val_recalage = 3000 - instruction.arg1; // Inversion du Y
                    // }
                    // else
                    // {
                    val_recalage = instruction.arg1;
                    // }
                } else {
                    coordonnee = 1;
                    val_recalage = instruction.arg1;
                }
                deplacement.recalage(distance, coordonnee, val_recalage);
                waitingAckID = ASSERVISSEMENT_RECALAGE;
                waitingAckFrom = ACKNOWLEDGE_MOTEUR;
                flag.wait_all(AckFrom_FLAG, 20000);  // si jamais il y a un timeout, qu'est ce qu'il faut faire? retenter? arreter le match?

                waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
                waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
                flag.wait_all(AckFrom_FIN_FLAG, 20000);
            } else {
            }
        } break;
        case MV_TURN: {
            int16_t angle = instruction.arg3;
            target_x_robot = x_robot;
            target_y_robot = y_robot;
            target_theta_robot = theta_robot + angle;

            // if (InversStrat == 1 && ingnorInversionOnce == 0)
            // {
            //   angle = -angle;
            // }

            if (instruction.direction == ABSOLUTE) {
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
        } break;
        case MV_LINE: {
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
        } break;
        case MV_XYT: {
            // on effectue XYT normalement selon les instructions
            //    uint16_t x;
            uint16_t y;
            int16_t theta;
            uint8_t sens;
            if (instruction.direction == BACKWARD) {
                sens = -1;
                // asser_stop_direction = -1;
            } else {
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
            if ((instruction.arg1 <= 0) || (y <= 0)) {
                deplacement.positionXYTheta(target_x_robot, target_y_robot, target_theta_robot, sens);
            } else {
                deplacement.positionXYTheta(instruction.arg1, y, theta, sens);
            }

            target_x_robot = instruction.arg1;
            target_y_robot = y;
            target_theta_robot = theta;

            flag.wait_all(AckFrom_FLAG, 20000);

            waitingAckID_FIN = ASSERVISSEMENT_XYT;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            flag.wait_all(AckFrom_FIN_FLAG, 20000);
        } break;
        case MV_COURBURE: {
            //    int16_t rayon;
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
            if (angle > 0) {
                direction = 1;
            } else {
                direction = -1;
            }
            if (angle > 0) {
                directionxyt = FORWARD;
                asser_stop_direction = 1;
            } else {
                directionxyt = BACKWARD;
                asser_stop_direction = -1;
            }
            alpha = angle * M_PI / 1800.0f;
            theta = theta_robot * M_PI / 1800.0f;
            int nbre;
            nbre = abs(alph) / 450;
            for (int c = 0; c < nbre + 1; c++) {
                dodgeq.inst[c].order = MV_XYT;
                dodgeq.inst[c].direction = directionxyt;
                if (instruction.direction == LEFT) {                                                  //-------------LEFT
                    target_x_robot = x_robot + instruction.arg1 * (sin(theta + alpha) - sin(theta));  // X
                    target_y_robot = y_robot + instruction.arg1 * (cos(theta) - cos(theta + alpha));  // Y
                    target_theta_robot = theta_robot + alph;                                          // T
                    dodgeq.inst[c].arg1 = target_x_robot;                                             // X
                    dodgeq.inst[c].arg2 = target_y_robot;                                             // Y
                    dodgeq.inst[c].arg3 = target_theta_robot;                                         // T
                } else {
                    target_x_robot = x_robot + instruction.arg1 * (sin(theta) - sin(theta - alpha));  // X
                    target_y_robot = y_robot + instruction.arg1 * (cos(theta - alpha) - cos(theta));  // Y
                    target_theta_robot = theta_robot - alph;                                          // T
                    dodgeq.inst[c].arg1 = target_x_robot;                                             // X
                    dodgeq.inst[c].arg2 = target_y_robot;                                             // Y
                    dodgeq.inst[c].arg3 = target_theta_robot;                                         // T
                }
                alpha -= alpha / ((float)nbre);
                alph -= alph / (nbre + 1);
            }

            flag.wait_all(AckFrom_FLAG, 20000);

            waitingAckID_FIN = ASSERVISSEMENT_COURBURE;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;

            flag.wait_all(AckFrom_FIN_FLAG, 20000);
        } break;
        // case MV_BEZIER:
        // {
        // }
        // break;
        case PINCE: {
            uint8_t Etage = (instruction.arg1 & 0xFF);
            uint8_t etatHerkulex = ((instruction.arg2 == 1) ? 1 : 0);
            uint8_t sens = (instruction.arg3 & 0xFF);
            waitingAckID = IDCAN_PINCE;
            waitingAckFrom = ACKNOWLEDGE_ACTIONNEURS;
            // printf("Herkulex.controlePince(Etage : %d, etatHerkulex : %d, sens : %d);\n",Etage,etatHerkulex, sens);

            herkulex.controlePince(Etage, etatHerkulex, sens);
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
        } break;
            // case POSITION_DEBUT:
            // {
            // }
            // break;

        default:
            break;
    }

    listeInstructions.suivante();
    // if (!instruction.nextLineOK) {
    //     actual_instruction += 1;
    // } else {
    //     actual_instruction = instruction.nextLineOK;
    // }

    gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;
}

E_Stratposdebut etat_pos = RECALAGE_1;

bool machineRecalageInit()
{
    etat_pos = RECALAGE_1;
    recalageErreur = 0;
    deplacement.asservOn();
    herkulex.stepMotorMode(1);
    ThisThread::sleep_for(50ms);
    listeInstructions.debut();
    // deplacement.asservOn();
    return true;
}

bool machineRecalage() {
    const S_Instruction &instruction = listeInstructions.enCours();
    switch (etat_pos) {
        case RECALAGE_1: {
            threadCAN.sendAck(RECALAGE_START, 0);
            int16_t distance = 1000;
            uint16_t val_recalage;

            if (Hauteur == ROBOT_EN_BAS) {
                val_recalage = 2000 - (MOITIEE_ROBOT);
            } else {
                val_recalage = MOITIEE_ROBOT;
                distance *= -1;
            }

            waitingAckID = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            deplacement.recalage(distance, 1, val_recalage);
            // printf("deplacement.recalage(distance : %d, 1, val_recalage : %d);\n", distance, val_recalage);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECALAGE_1, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -1;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECALAGE_1, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -2;
                return false;
            }

            etat_pos = RECULER_1;
        } break;
        case RECULER_1: {
            waitingAckID = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            int16_t distance = 100;
            if (Hauteur == ROBOT_EN_BAS) {
                distance *= -1;
            }
            deplacement.toutDroit(distance);
            // printf("deplacement.toutDroit(distance : %d);\n", distance);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECULER_1, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -3;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECULER_1, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -4;
                return false;
            }
            etat_pos = TOURNER;
        } break;
        case TOURNER: {
            waitingAckID = ASSERVISSEMENT_ROTATION;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            int16_t angle = 900;
            if (Cote == ROBOT_A_GAUCHE) {
                angle = 900;
            } else if (Cote == ROBOT_A_DROITE) {
                angle = -900;
            }

            deplacement.rotation(angle);
            // printf("deplacement.rotation(angle : %d);\n", angle);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, TOURNER, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -5;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_ROTATION;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, TOURNER, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -6;
                return false;
            }
            etat_pos = RECALAGE_2;
        } break;
        case RECALAGE_2: {
            int16_t distance = -1000;
            uint16_t val_recalage;

            if (Cote == ROBOT_A_DROITE) {
                val_recalage = 3000 - (MOITIEE_ROBOT);
            }  // par devant
            else {
                val_recalage = MOITIEE_ROBOT;
            }  // par derriere

            waitingAckID = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            deplacement.recalage(distance, 2, val_recalage);
            // printf("deplacement.recalage(distance : %d, 2, val_recalage : %d);\n", distance, val_recalage);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECALAGE_2, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -7;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECALAGE_2, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -8;
                return false;
            }

            etat_pos = RECULER_2;
        } break;
        case RECULER_2: {
            waitingAckID = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            int16_t distance = 100;
            deplacement.toutDroit(distance);
            // printf("deplacement.toutDroit(distance : %d);\n", distance);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECULER_2, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -9;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_RECALAGE;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage fail, RECULER_2, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -10;
                return false;
            }
            etat_pos = GOTOPOS;
        } break;
        case GOTOPOS: {
            waitingAckID = ASSERVISSEMENT_XYT;
            waitingAckFrom = ACKNOWLEDGE_MOTEUR;
            deplacement.positionXYTheta(instruction.arg1, instruction.arg2, instruction.arg3, 0);
            // printf("deplacement.positionXYTheta(instruction.arg1 : %d, instruction.arg2 : %d, instruction.arg3 : %d, 0);\n", instruction.arg1, instruction.arg2, instruction.arg3);
            if (flag.wait_all(AckFrom_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage, GOTOPOS, waitingAckID\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -11;
                return false;
            }

            waitingAckID_FIN = ASSERVISSEMENT_XYT;
            waitingAckFrom_FIN = INSTRUCTION_END_MOTEUR;
            if (flag.wait_all(AckFrom_FIN_FLAG, 20000) == osFlagsErrorTimeout) {
                // printf("osErrorTimeout, recalage, GOTOPOS, waitingAckID_FIN\n");
                gameEtat = ETAT_GAME_INIT;
                recalageErreur = -12;
                return false;
            }
            etat_pos = FIN_POS;
        } break;
        case FIN_POS: {
            // actual_instruction = instruction.nextLineOK;
            listeInstructions.vaLigne(instruction.nextLineOK);
            target_x_robot = x_robot;
            target_y_robot = y_robot;
            target_theta_robot = theta_robot;

            gameEtat = ETAT_GAME_WAIT_FOR_JACK;
            return false;
        } break;

        default:
            break;
    }
    return true;
}

bool machineStrategie() {
    S_Instruction instruction;
    switch (gameEtat) {
        case ETAT_GAME_LOAD_NEXT_INSTRUCTION:
            // printf("load next instruction\n");
            // if (actual_instruction >= nb_instructions || actual_instruction == 255) {
            if (listeInstructions.fin()) {
                gameEtat = ETAT_END;
                // Il n'y a plus d'instruction, fin du jeu
            } else {
                // instruction = strat_instructions[actual_instruction];
                instruction = listeInstructions.enCours();
                // On effectue le traitement de l'instruction
                gameEtat = ETAT_GAME_PROCESS_INSTRUCTION;
            }
            break;

        case ETAT_GAME_PROCESS_INSTRUCTION: {
            //      Traitement de l'instruction, envoie de la trame CAN
            debug_Instruction(instruction);
            process_instructions(instruction);
        } break;

        case ETAT_END: {
            // printf("GAME ENDED\n");
            return false;
        } break;

        default:
            break;
    }
    return true;
}
