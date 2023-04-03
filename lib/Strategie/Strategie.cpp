#include <global.h>
#include "Strategie.h"
#include <Instruction.h>

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

unsigned char screenChecktry = 0;
unsigned char test[32] = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32};

signed char asser_stop_direction=0;
char counter = 0;
char check;
char Jack = 1;
short SCORE_GLOBAL=0;
short SCORE_GR=0;
short SCORE_PR=0;
unsigned short distance_recalage;
unsigned short distance_revenir;

unsigned short x;
unsigned short y;
unsigned char isStopEnable = 1;//Permet de savoir si il faut autoriser le stop via les balises
unsigned short flag_check_carte = 0, flag_strat = 0, flag_timer;
int flagReceptionTelemetres = 0, flagNonRepriseErrorMot = 0;

int Flag_Bras_Re = 0, Flag_Manche_Bas = 0, Flag_Manche_Moy = 0, Flag_pavillon = 0, Flag_bon_port = 0; //Flag utilisé pour compter le score des bras / manches / pavillon
unsigned short Flag_num_bras;


signed short x_robot,y_robot,theta_robot;//La position du robot
signed short target_x_robot, target_y_robot, target_theta_robot;
signed short avant_gauche, avant_droit;
//E_InstructionType actionPrecedente;
//unsigned char FIFO_ecriture=0; //Position du fifo pour la reception CAN
int flagSendCan=1;
unsigned char Cote = 0; //0 -> JAUNE | 1 -> VIOLET
unsigned char Hauteur = 0; //Robot du haut -> 1, du bas -> 0
unsigned short angleRecalage = 0;
unsigned char checkCurrent = 0;
unsigned char countAliveCard = 0;
unsigned char ligne=0;
int Fevitement=0;
int EvitEtat= 0;
int stop_evitement=0;
signed char nbStrat = 0; //N° de la strategie (1-10)
unsigned char ModeDemo = 0; // Si à 1, indique que l'on est dans le mode demo
unsigned char countRobotNear = 0;//Le nombre de robot à proximité
unsigned char ingnorBaliseOnce = 0;//une fois détecté réinitialise
unsigned char ingnorBalise = 0;//0:balise ignore 1:on ecoute la balise
short direction;
char val_girou ;

unsigned char debug_bon_port = 0;

unsigned char ingnorInversionOnce = 0;//Pour ignorer l'inversion des instruction une fois

// struct S_Instruction instruction;
struct S_Dodge_queue dodgeq;

char couleur1, couleur2, couleur3;
float cptf;
int cpt,cpt1;



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
            case ALIVE_ACTIONNEURS_ARRIERE:
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

            }
            case ACKNOWLEDGE_BALISE:    //pas de break donc passe directement dans ACK_FIN_ACTION mais conserve l'ident initial
            {

            }
            case ACKNOWLEDGE_TELEMETRE:{

            }
            /////////////////////////////////////////////Acknowledges de la fin d'action/////////////////////////////////////////////////
            case ACKNOWLEDGE_MOTEUR:{
                printf("ACKNOWLEDGE_MOTEUR\n");
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
                break;
            }
            case INSTRUCTION_END_BALISE:{

            }
            case ACK_FIN_ACTION:{

            }
            case INSTRUCTION_END_MOTEUR:
            {
                unsigned short recieveAckID = (unsigned short)rxMsg->data[0]  | ( ((unsigned short)rxMsg->data[1]) <<8);
                printf("INSTRUCTION_END_MOTEUR\n");
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

