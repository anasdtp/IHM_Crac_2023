// #include "global.h"
// #include "asservissement.h"

// #define M_PI 3.14159265358979323846
 
// #define VERT 0xFF00FF00
// #define ROUGE 0xFFFF0000
// #define BLEU 0xFF0000FF
// #define JAUNE 0xFFFDD835//FEFE00
// #define BLANC 0xFF000000
// #define ORANGE 0xFFFFA500
// #define NOIR 0xFF000000
// #define DIY_GREY 0xFFDFDFDF
// #define VIOLET 0xFF4527A0
// #define VERT_F 0xFF00C400
// #define VIOLET_F 0x788000FF

// TS_StateTypeDef TS_State;

// /////////////////DEFINITION DES BOUTONS////////////////////
// Button COTE_JAUNE(0, 25, 400, 300, "JAUNE");
// Button COTE_VIOLET(0, 350, 400, 300, "BLEU");
// Button COTE_HAUT(0, 25, 400, 300, "ROBOT DU HAUT");
// Button COTE_BAS(0, 350, 400, 300, "ROBOT DU BAS");
// Button RETOUR  (0, 680, 400, 110, "--Precedent--");
// Button LANCER  (0, 200, 400, 200, "--LANCER--");
// Button CHECK (0, 420, 400, 200, "Valider");
// Button MATCH (0, 50, 400, 320, "Match");
// Button DEMONSTRATION (0, 400, 400, 320, "Demo");
// //menu demo
// Button TEST_VENT(0, 25, 400, 100, "Test ventouses");
// Button TEST_MOT(0, 135, 400, 100, "Test moteur");
// Button TEST_COUL(0,245,400,100,"Test capteurs couleur");
// Button TEST_BRAS(0,355,400,100,"Test Bras");
// Button TEST_AUDIO(0,465,400,100,"Test Audio");

// Button TEST_NEXT(0,575,400,100,"------>");
// Button TEST_DIV(0,25,400,100,"Test Divers");
// Button TEST_ASSERV(0, 135, 400, 100, "Test asserv");
// Button TEST_MANCHE(0,245,400,100,"Test manches a air");
// Button TEST_GIROU(0,355,400,100,"Test capteur girouette");


// //menu demo/demo/
// Button TEST_A(0,25,195,100,"A");
// Button TEST_B(205,25,195,100,"B");
// Button TEST_C(0,135,195,100,"C");
// Button TEST_D(205,135,195,100,"D");
// Button TEST_NUM(0,245,400,100,"Num");
// Button TEST_ATTRAPER(0,300,400,100,"Attraper");
// Button TEST_RELACHER(0,450,400,100,"Relacher");

// Button TEST_PLUS(205,355,195,100,"+");
// Button TEST_MOINS(0,355,195,100,"-");
// Button TEST_SERVO_0(0,160,120,100,"Bras 0");
// Button TEST_SERVO_1(140,160,120,100,"Bras 1");
// Button TEST_SERVO_2(280,160,120,100,"Bras 2");
// Button TEST_SERVO_3(280,50,120,100,"Bras 3");
// Button TEST_SERVO_4(140,50,120,100,"Bras 4");
// Button TEST_SERVO_5(0,50,120,100,"Bras 5");

// Button TEST_VENT_0(0,160,120,100,"Vent 0");
// Button TEST_VENT_1(140,160,120,100,"Vent 1");
// Button TEST_VENT_2(280,160,120,100,"Vent 2");
// Button TEST_VENT_3(280,50,120,100,"Vent 3");
// Button TEST_VENT_4(140,50,120,100,"Vent 4");
// Button TEST_VENT_5(0,50,120,100,"Vent 5");

// Button FORCE_LAUNCH(0, 600, 400, 100, "Force Launch");
// Button SUIVANT(0,380,200,100,"Suivant");
// ////////////////////////////////////////////////////////////

// signed char Bouton_Strat (void);
// void DrawC (int l,int h,int x, int y, unsigned char fill);
// void DrawR (int l,int h,int x, int y, unsigned char fill);
// void DrawA (int l,int h,int x, int y, unsigned char fill);

// void SelectionStrat (unsigned char numeroStrat);

// /****************************************************************************************/
// /* FUNCTION NAME: automate_etat_ihm                                                     */
// /* DESCRIPTION  : Automate de gestion de l'affichage                                    */
// /****************************************************************************************/
// void automate_etat_ihm(void)
// {            
//     char toto[12]; 
//     int j, old_time=0, actual_time=0, old_score=0, actual_score=0, unique=0;
//     unsigned char maximilien=1, choix_groupe;
//     if (j==0) {
//         ts.Init(lcd.GetXSize(), lcd.GetYSize());
//         j++;
//     }
//     ts.GetState(&TS_State);
//     switch (strat_etat_s) {
//         case INIT : //intialise l'écran et passe à l'attente d'initialisation des cartes
//             ts.GetState(&TS_State);
//             canProcessRx();
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             wait(0.15);
//             DrawC(95,150,0,50,0);
//             DrawR(95,150,105,50,0);
//             DrawA(95,150,210,50,0);
//             DrawC(95,150,315,50,0);
//             DrawCheck(30,30,50,243,0);
//             DrawCheck(30,30,50,293,0);
//             DrawCheck(30,30,50,343,0);
//             DrawCheck(30,30,50,393,0);
//             lcd.DisplayStringAt(100, 250, (uint8_t *)"Init CAN", LEFT_MODE);
//             DrawCheck(30,30,50,243,can_present);
//             lcd.DisplayStringAt(100, 300, (uint8_t *)"Micro SD", LEFT_MODE);
//             DrawCheck(30,30,50,293,sd_present);
//             lcd.DisplayStringAt(100, 350, (uint8_t *)"Carte Moteur", LEFT_MODE);
//             lcd.DisplayStringAt(100, 400, (uint8_t *)"Balise IR", LEFT_MODE);
//             FORCE_LAUNCH.Draw(0xFFFF0000, 0);

//             strat_etat_s=ATT;
//             break;

//         case ATT :  //Si les cartes sont présentes passe directement à choix sinon attente de force Launch (cette partie est encore buggée mais les cartes affichent bien leur présence donc faut juste force launch tout le temps...)
//             if (flag_check_carte==1) {
//                 strat_etat_s = CHOIX;
//                 gameEtat = ETAT_CONFIG;
//             } else if (FORCE_LAUNCH.Touched()) {
//                 strat_etat_s = CHOIX;
//                 gameEtat = ETAT_CONFIG;
//                 while(FORCE_LAUNCH.Touched());
//             }

//             break;


//         case CHOIX :    //Match ou DEMO
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Match ou demonstration ?", LEFT_MODE);
//             DEMONSTRATION.Draw(LCD_COLOR_LIGHTGREEN, 0);
//             MATCH.Draw(0xFFF01010, 0);
//             while(strat_etat_s == CHOIX) {
//                 canProcessRx();
//                 if(DEMONSTRATION.Touched()) {
//                     strat_etat_s = DEMO;
//                     while(DEMONSTRATION.Touched());
//                 }

//                 if(MATCH.Touched()) {
//                     strat_etat_s = SELECT_SIDE;
//                     while(MATCH.Touched());
//                 }

//             }
//             break;

//         case DEMO :
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"DEMO 1/2", LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000, 0);
//             TEST_VENT.Draw(VERT, 0);
//             TEST_MOT.Draw(VERT, 0);
//             TEST_COUL.Draw(VERT, 0);
//             TEST_BRAS.Draw(VERT, 0);
//             TEST_AUDIO.Draw(VERT, 0);
//             TEST_NEXT.Draw(BLEU, 0);
//             maximilien=0;
//             while (strat_etat_s == DEMO) {
//                 canProcessRx();
//                 if(TEST_VENT.Touched()) 
//                 {
//                     while(TEST_VENT.Touched());
//                     strat_etat_s = TEST_VENTOUSE;
//                 } 
//                 else if(TEST_MOT.Touched()) 
//                 {
//                     while(TEST_MOT.Touched());
//                     strat_etat_s = TEST_MOTEUR;
//                 } 
//                 else if(TEST_NEXT.Touched()) 
//                 {
//                     while(TEST_NEXT.Touched());
//                     strat_etat_s = DEMO2;
//                 } 
//                 else if (TEST_COUL.Touched()) 
//                 {
//                     while(TEST_COUL.Touched());
//                     strat_etat_s =TEST_COULEUR ;
//                 } 
//                 else if (TEST_BRAS.Touched()) 
//                 {
//                     while(TEST_BRAS.Touched());
//                     strat_etat_s =TEST_SERVO_BRAS ;
//                 } 
//                 else if(TEST_AUDIO.Touched()) 
//                 {
//                     while(TEST_AUDIO.Touched());
//                     strat_etat_s=TEST_AUD;
//                 } 
//                 else if(RETOUR.Touched()) 
//                 {
//                     while(RETOUR.Touched());
//                     strat_etat_s = CHOIX;
//                 }
//             }
//             break;
        
//         case DEMO2 :
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"DEMO 2/2", LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000, 0);
//             TEST_DIV.Draw(VERT, 0);
//             TEST_ASSERV.Draw(VERT, 0);
//             TEST_MANCHE.Draw(VERT, 0);
//             TEST_GIROU.Draw(VERT, 0);
//             TEST_NEXT.Draw(BLEU, 0);
//             maximilien=0;
//             while (strat_etat_s == DEMO2) {
//                 canProcessRx();
//                 if(TEST_NEXT.Touched()) 
//                 {
//                     while(TEST_NEXT.Touched());
//                     strat_etat_s = DEMO;
//                 } 
//                 else if(TEST_DIV.Touched()) 
//                 {
//                     while(TEST_DIV.Touched());
//                     strat_etat_s = TEST_DIVE;
//                 } 
//                 else if(TEST_ASSERV.Touched()) 
//                 {
//                     while(TEST_ASSERV.Touched());
//                     strat_etat_s = TEST_ASSERVE;
//                 } 
//                 else if(TEST_MANCHE.Touched()) 
//                 {
//                     while(TEST_MANCHE.Touched());
//                     strat_etat_s = TEST_MANCHES;
//                 }
//                 else if(TEST_GIROU.Touched()) 
//                 {
//                     while(TEST_GIROU.Touched());
//                     strat_etat_s = TEST_GIROUS;
//                 }
                
//                 else if(RETOUR.Touched()) 
//                 {
//                     while(RETOUR.Touched());
//                     strat_etat_s = CHOIX;
//                 }
//             }
//         break;
        
//         case TEST_MANCHES:
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Manches a air", LEFT_MODE);
            
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_B.Draw(BLEU, BLANC);   
//             TEST_C.Draw(BLEU, BLANC);  
//             TEST_D.Draw(BLEU, BLANC) ;
            
//             while(strat_etat_s==TEST_MANCHES) 
//             {
//                 if(RETOUR.Touched()) 
//                 {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO2;
//                 }
//                 else if(TEST_A.Touched()) 
//                 {
//                     while (TEST_A.Touched());
//                     TEST_A.Draw(BLEU, BLANC);
//                     SendRawId(TEST_BRAS_1);
//                 }  
//                 else if(TEST_B.Touched()) 
//                 {
//                     while (TEST_B.Touched());
//                     TEST_B.Draw(BLEU, BLANC); 
//                     choix_groupe = 0;
//                     SendMsgCan(AUTOMATE_MANCHE_MOY, &choix_groupe, sizeof(choix_groupe)) ;   
//                     wait_ms(500);
//                     SendMsgCan(AUTOMATE_MANCHE_BAS, &choix_groupe, sizeof(choix_groupe)) ;   
//                     wait_ms(500);
                    
//                     choix_groupe = 1;
//                     SendMsgCan(AUTOMATE_MANCHE_MOY, &choix_groupe, sizeof(choix_groupe)) ;   
//                     wait_ms(500);
//                     SendMsgCan(AUTOMATE_MANCHE_BAS, &choix_groupe, sizeof(choix_groupe)) ;   
//                     wait_ms(500);
                    
//                 }
//                  else if(TEST_C.Touched()) 
//                 {
//                     while (TEST_A.Touched());
//                     TEST_C.Draw(BLEU, BLANC);
//                     SendRawId(PAVILLON_DEPLOYE);
//                 }  
                
//             }
//             break;
                    
//         case TEST_GIROUS :
//             SendRawId(LECTURE_GIROUETTE) ;
//             wait_ms(5); 
//             canProcessRx();
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Girouette", LEFT_MODE);
//             sprintf(toto,"%hd",val_girou);
//             lcd.DisplayStringAt(0, LINE(10), (unsigned char *)"Valeur Girou :", LEFT_MODE);
//             lcd.DisplayStringAt(0, LINE(11), (unsigned char *)toto, LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO2;
//                 } 
//             wait_ms(200);
//             break ;
            
//         case TEST_ASSERVE:
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Asservissements", LEFT_MODE);
               
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_B.Draw(BLEU, BLANC);                
//             TEST_C.Draw(BLEU, BLANC) ;
                
//             while(strat_etat_s==TEST_ASSERVE) 
//             {
//                 if(RETOUR.Touched()) 
//                 {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO2;
//                 } 
//                 else if(TEST_A.Touched()) 
//                 {
//                     while (TEST_A.Touched());
//                     GoStraight(2000,0,0,0);
//                     TEST_A.Draw(BLEU, BLANC);  
//                 }  
//                 else if(TEST_B.Touched()) 
//                 {
//                     while (TEST_B.Touched());
//                     Rotate(3600*5);
//                     TEST_B.Draw(BLEU, BLANC);
//                 }  
//                 else if(TEST_C.Touched()) 
//                 {
//                     while (TEST_C.Touched());
//                     BendRadius(1000,900,1,0);
//                     TEST_B.Draw(BLEU, BLANC);
//                 }  
//             }
//         break; 

//         case TEST_DIVE:
            
//             while(strat_etat_s==TEST_DIVE) {
//                 canProcessRx();
//                 lcd.SetBackColor(LCD_COLOR_WHITE);
//                 lcd.SetTextColor(LCD_COLOR_BLACK);
//                 lcd.Clear (LCD_COLOR_WHITE);
//                 lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Coordonnees", LEFT_MODE);
//                 sprintf(toto,"%hd",x_robot);
//                 lcd.DisplayStringAt(0, LINE(10), (unsigned char *)"X :", LEFT_MODE);
//                 lcd.DisplayStringAt(50, LINE(10), (unsigned char *)toto, LEFT_MODE);
//                 sprintf(toto,"%hd",y_robot);
//                 lcd.DisplayStringAt(0, LINE(11), (unsigned char *)"Y :", LEFT_MODE);
//                 lcd.DisplayStringAt(50, LINE(11), (unsigned char *)toto, LEFT_MODE);
//                 sprintf(toto,"%hd",theta_robot);
//                 lcd.DisplayStringAt(0, LINE(12), (unsigned char *)"T :", LEFT_MODE);
//                 lcd.DisplayStringAt(50, LINE(12), (unsigned char *) toto, LEFT_MODE);
//                 RETOUR.Draw(0xFFFF0000,0);
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO2;
//                 } 
//                 wait(0.2);
//             }
//         break;

//         case TEST_VENTOUSE:   
//             char vent_select_0 = 7, vent_select_1 = 0, vent_select_2 = 0, vent_select_3 = 0, vent_select_4 = 0, vent_select_5 = 0 ;
//             unsigned char choix_vent;
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);   
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Ventouse", LEFT_MODE);
//             sprintf(toto,"%hd",maximilien);
//             lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_ATTRAPER.Draw(BLEU, BLANC);
//             TEST_RELACHER.Draw(BLEU, BLANC);
//             TEST_VENT_0.Draw(BLEU, BLANC);
//             TEST_VENT_1.Draw(BLEU, BLANC);
//             TEST_VENT_2.Draw(BLEU, BLANC);
//             TEST_VENT_3.Draw(BLEU, BLANC);
//             TEST_VENT_4.Draw(BLEU, BLANC);
//             TEST_VENT_5.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_VENTOUSE) 
//             {
//                 if(RETOUR.Touched()) 
//                 {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 }  
//                 else if(TEST_VENT_0.Touched()) 
//                 {
//                     while (TEST_VENT_0.Touched());
//                     if(vent_select_0 == 0)
//                     {
//                         vent_select_0 = 7 ;
//                         TEST_VENT_0.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_0 = 0 ;
//                         TEST_VENT_0.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_VENT_1.Touched()) 
//                 {
//                     while (TEST_VENT_1.Touched());
//                     if(vent_select_1 == 1)
//                     {
//                         vent_select_1 = 0 ;
//                         TEST_VENT_1.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_1 = 1 ;
//                         TEST_VENT_1.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_VENT_2.Touched()) 
//                 {
//                     while (TEST_VENT_2.Touched());
//                     if(vent_select_2 == 2)
//                     {
//                         vent_select_2 = 0 ;
//                         TEST_VENT_2.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_2 = 2 ;
//                         TEST_VENT_2.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_VENT_3.Touched()) 
//                 {
//                     while (TEST_VENT_3.Touched());
//                     if(vent_select_3 == 3)
//                     {
//                         vent_select_3 = 0 ;
//                         TEST_VENT_3.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_3 = 3 ;
//                         TEST_VENT_3.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_VENT_4.Touched()) 
//                 {
//                     while (TEST_VENT_4.Touched());
//                     if(vent_select_4 == 4)
//                     {
//                         vent_select_4 = 0 ;
//                         TEST_VENT_4.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_4 = 4 ;
//                         TEST_VENT_4.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_VENT_5.Touched()) 
//                 {
//                     while (TEST_VENT_5.Touched());
//                     if(vent_select_5 == 5)
//                     {
//                         vent_select_5 = 0 ;
//                         TEST_VENT_5.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         vent_select_5 = 5 ;
//                         TEST_VENT_5.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if (TEST_ATTRAPER.Touched())
//                 {
//                     while(TEST_ATTRAPER.Touched()) ;
//                     unsigned char message_droit_at, message_gauche_at, nombre_de_chiffre_droite, nombre_de_chiffre_gauche;  //chager pour un short et remettre les valeurs correct (char utiliser pour les test)
//                     message_droit_at = 100 * vent_select_2 + 10 * vent_select_1 + vent_select_0 ;
//                     message_gauche_at = 100 * vent_select_5 + 10 * vent_select_4 + vent_select_3 ;
                    
//                     switch (message_gauche_at)
//                     {
//                         case 31 :           //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement 
//                             choix_vent = 66 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ; 
//                         case 28 :          //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_vent = 54 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 247 :         //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_vent = 53 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 43 :
//                             choix_vent = 43 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 244 :          //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_vent = 5 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 40 :
//                             choix_vent = 4 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 3 : 
//                             choix_vent = 3 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                     }
                    
//                     switch (message_droit_at)
//                     {
//                         case 210 :
//                             choix_vent = 210 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 217 :
//                             choix_vent = 21 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 200 :
//                             choix_vent = 20 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 10 :
//                             choix_vent = 10 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 207 :
//                             choix_vent = 2 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 17 :
//                             choix_vent = 1 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 0 : 
//                             choix_vent = 0 ;
//                             SendMsgCan(VENT_AT, &choix_vent,sizeof(choix_vent));
//                             break ;
//                     }
                    
//                     message_droit_at = 0 ;
//                     message_gauche_at = 0 ;
                    
//                     TEST_ATTRAPER.Draw(BLEU, BLANC);
//                 }
//                 else if (TEST_RELACHER.Touched())
//                 {
//                     while(TEST_RELACHER.Touched());
//                     unsigned char message_droit_re, message_gauche_re;
//                     message_droit_re = 100 * vent_select_2 + 10 * vent_select_1 + vent_select_0 ;
//                     message_gauche_re = 100 * vent_select_5 + 10 * vent_select_4 + vent_select_3 ;
//                     switch (message_droit_re)
//                     {
//                         case 210 :
//                             choix_vent = 210 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 217 :
//                             choix_vent = 21 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 200 :
//                             choix_vent = 20 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 10 :
//                             choix_vent = 10 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 207 :
//                             choix_vent = 2 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 17 :
//                             choix_vent = 1 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 0 : 
//                             choix_vent = 0 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                     }
                    
//                     switch (message_gauche_re)
//                     {
//                         case 31 :
//                             choix_vent = 66 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 28 :
//                             choix_vent = 54 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 247 :
//                             choix_vent = 53 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 43 :
//                             choix_vent = 43 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 244 :
//                             choix_vent = 5 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 40 :
//                             choix_vent = 4 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                         case 3 : 
//                             choix_vent = 3 ;
//                             SendMsgCan(VENT_RE, &choix_vent,sizeof(choix_vent));
//                             break ;
//                     }
//                     message_droit_re = 0 ;
//                     message_gauche_re = 0 ;     //on remet les variables de calcul à 0 pour qu'on puisse recalculer correctement
                    
//                     TEST_RELACHER.Draw(BLEU, BLANC);
                    
//                 }
//             }
//             break;
            
//             /*
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_NUM.Draw(BLEU, BLANC);
//             TEST_PLUS.Draw(BLEU, BLANC);
//             TEST_MOINS.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_VENTOUSE) {
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 } else if(TEST_A.Touched()) {
//                     while (TEST_A.Touched());
//                     SendRawId(VENT_ETAT);
//                     TEST_A.Draw(BLEU, BLANC);
//                 }  else if(TEST_NUM.Touched()) {
//                     while (TEST_NUM.Touched());
//                     TEST_NUM.Draw(BLEU, BLANC);
//                     switch(maximilien){
//                     case 1:
//                     choix_groupe = 0 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 2:
//                     choix_groupe = 0 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 3:
//                     choix_groupe = 1 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 4:
//                     choix_groupe = 1 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 5:
//                     choix_groupe = 2 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 6:
//                     choix_groupe = 2 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 7:
//                     choix_groupe = 3 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 8:
//                     choix_groupe = 3 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 9:
//                     choix_groupe = 4 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 10:
//                     choix_groupe = 4 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 11:
//                     choix_groupe = 5 ;
//                     SendMsgCan(VENT_AT, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     case 12:
//                     choix_groupe = 5 ;
//                     SendMsgCan(VENT_RE, &choix_groupe, sizeof(choix_groupe)) ;
//                     break;
//                     }
//                 } else if(TEST_PLUS.Touched()) {
//                     while (TEST_PLUS.Touched());
//                     TEST_PLUS.Draw(BLEU, BLANC);
//                     if(maximilien!=6)maximilien++;
//                     sprintf(toto,"%hd",maximilien);
//                     lcd.SetBackColor(LCD_COLOR_WHITE);
//                     lcd.SetTextColor(LCD_COLOR_BLACK);
//                     lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//                 } else if(TEST_MOINS.Touched()) {
//                     while (TEST_MOINS.Touched());
//                     TEST_MOINS.Draw(BLEU, BLANC);
//                     if(maximilien!=1)maximilien--;
//                     sprintf(toto,"%hd",maximilien);
//                     lcd.SetBackColor(LCD_COLOR_WHITE);
//                     lcd.SetTextColor(LCD_COLOR_BLACK);
//                     lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//                 }
//             }
//             break;*/
        
//         case TEST_ELECTROV:
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);   
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"A : Manches B : Pavillon", LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_B.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_ELECTROV) {
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 } else if(TEST_A.Touched()) {
//                     while (TEST_A.Touched());
//                     //SendRawId(TEST_ELECTROV_A);
//                     TEST_A.Draw(BLEU, BLANC);
//                 }  else if(TEST_B.Touched()) {
//                     while (TEST_B.Touched());
//                     //SendRawId(TEST_ELECTROV_A);
//                     TEST_B.Draw(BLEU, BLANC);
//                 }  
//             }
//             break;
            
//         case TEST_AUD:  
              
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);   
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Audio", LEFT_MODE);
//             sprintf(toto,"%hd",maximilien);
//             lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_NUM.Draw(BLEU, BLANC);
//             TEST_PLUS.Draw(BLEU, BLANC);
//             TEST_MOINS.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_AUD) {
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 }  else if(TEST_NUM.Touched()) {
//                     while (TEST_NUM.Touched());
//                     TEST_NUM.Draw(BLEU, BLANC);
//                     Debug_Audio(3,maximilien);
//                 } else if(TEST_PLUS.Touched()) {
//                     while (TEST_PLUS.Touched());
//                     TEST_PLUS.Draw(BLEU, BLANC);
//                     if(maximilien!=20)maximilien++;
//                     sprintf(toto,"%hd",maximilien);
//                     lcd.SetBackColor(LCD_COLOR_WHITE);
//                     lcd.SetTextColor(LCD_COLOR_BLACK);
//                     lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//                 } else if(TEST_MOINS.Touched()) {
//                     while (TEST_MOINS.Touched());
//                     TEST_MOINS.Draw(BLEU, BLANC);
//                     if(maximilien!=1)maximilien--;
//                     sprintf(toto,"%hd",maximilien);
//                     lcd.SetBackColor(LCD_COLOR_WHITE);
//                     lcd.SetTextColor(LCD_COLOR_BLACK);
//                     lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//                 }
//             }
//             break;
            
//         case TEST_SERVO_BRAS:  
//             char bras_select_0 = 7, bras_select_1 = 0, bras_select_2 = 0, bras_select_3 = 0, bras_select_4 = 0, bras_select_5 = 0 ;
//             unsigned char choix_bras;
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);   
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Bras", LEFT_MODE);
//             sprintf(toto,"%hd",maximilien);
//             lcd.DisplayStringAt(50, LINE(20), (unsigned char *)toto, LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_ATTRAPER.Draw(BLEU, BLANC);
//             TEST_RELACHER.Draw(BLEU, BLANC);
//             TEST_SERVO_0.Draw(BLEU, BLANC);
//             TEST_SERVO_1.Draw(BLEU, BLANC);
//             TEST_SERVO_2.Draw(BLEU, BLANC);
//             TEST_SERVO_3.Draw(BLEU, BLANC);
//             TEST_SERVO_4.Draw(BLEU, BLANC);
//             TEST_SERVO_5.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_SERVO_BRAS) 
//             {
//                 if(RETOUR.Touched()) 
//                 {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 }  
//                 else if(TEST_SERVO_0.Touched()) 
//                 {
//                     while (TEST_SERVO_0.Touched());
//                     if(bras_select_0 == 0)
//                     {
//                         bras_select_0 = 7 ;
//                         TEST_SERVO_0.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_0 = 0 ;
//                         TEST_SERVO_0.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_SERVO_1.Touched()) 
//                 {
//                     while (TEST_SERVO_1.Touched());
//                     if(bras_select_1 == 1)
//                     {
//                         bras_select_1 = 0 ;
//                         TEST_SERVO_1.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_1 = 1 ;
//                         TEST_SERVO_1.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_SERVO_2.Touched()) 
//                 {
//                     while (TEST_SERVO_2.Touched());
//                     if(bras_select_2 == 2)
//                     {
//                         bras_select_2 = 0 ;
//                         TEST_SERVO_2.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_2 = 2 ;
//                         TEST_SERVO_2.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_SERVO_3.Touched()) 
//                 {
//                     while (TEST_SERVO_3.Touched());
//                     if(bras_select_3 == 3)
//                     {
//                         bras_select_3 = 0 ;
//                         TEST_SERVO_3.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_3 = 3 ;
//                         TEST_SERVO_3.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_SERVO_4.Touched()) 
//                 {
//                     while (TEST_SERVO_4.Touched());
//                     if(bras_select_4 == 4)
//                     {
//                         bras_select_4 = 0 ;
//                         TEST_SERVO_4.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_4 = 4 ;
//                         TEST_SERVO_4.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if(TEST_SERVO_5.Touched()) 
//                 {
//                     while (TEST_SERVO_5.Touched());
//                     if(bras_select_5 == 5)
//                     {
//                         bras_select_5 = 0 ;
//                         TEST_SERVO_5.Draw(BLEU, BLANC);
//                     }
//                     else
//                     {
//                         bras_select_5 = 5 ;
//                         TEST_SERVO_5.Draw(DIY_GREY, BLANC);
//                     }
//                 }
//                 else if (TEST_ATTRAPER.Touched())
//                 {
//                     while(TEST_ATTRAPER.Touched()) ;
//                     unsigned char message_droit_at, message_gauche_at, nombre_de_chiffre_droite, nombre_de_chiffre_gauche;
//                     message_droit_at = 100 * bras_select_2 + 10 * bras_select_1 + bras_select_0 ;
//                     message_gauche_at = 100 * bras_select_5 + 10 * bras_select_4 + bras_select_3 ;
                    
//                     switch (message_gauche_at)
//                     {
//                         case 31 :           //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_bras = 66 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ; 
//                         case 28 :          //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_bras = 54 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 247 :         //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_bras = 53 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 43 :
//                             choix_bras = 43 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 244 :          //on dépasse la taille d'un char donc on met la valeur en comptant le dépassement
//                             choix_bras = 5 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 40 :
//                             choix_bras = 4 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 3 : 
//                             choix_bras = 3 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                     }
                    
//                     switch (message_droit_at)
//                     {
//                         case 210 :
//                             choix_bras = 210 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 217 :
//                             choix_bras = 21 ;
//                             GoStraight(-40, 0, 0, 0);
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 200 :
//                             choix_bras = 20 ;
//                             GoStraight(-50, 0, 0, 0);
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 10 :
//                             choix_bras = 10 ;
//                             GoStraight(-45, 0, 0, 0);
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 207 :
//                             choix_bras = 2 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 17 :
//                             choix_bras = 1 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 0 : 
//                             choix_bras = 0 ;
//                             SendMsgCan(BRAS_AT, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                     }
                    
//                     message_droit_at = 0 ;
//                     message_gauche_at = 0 ;
                    
//                     TEST_ATTRAPER.Draw(BLEU, BLANC);
//                 }
//                 else if (TEST_RELACHER.Touched())
//                 {
//                     while(TEST_RELACHER.Touched());
//                     unsigned char message_droit_re, message_gauche_re;
//                     message_droit_re = 100 * bras_select_2 + 10 * bras_select_1 + bras_select_0 ;
//                     message_gauche_re = 100 * bras_select_5 + 10 * bras_select_4 + bras_select_3 ;
//                     switch (message_droit_re)
//                     {
//                         case 210 :
//                             choix_bras = 210 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 217 :
//                             choix_bras = 21 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 200 :
//                             choix_bras = 20 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 10 :
//                             choix_bras = 10 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 207 :
//                             choix_bras = 2 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 17 :
//                             choix_bras = 1 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 0 : 
//                             choix_bras = 0 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                     }
                    
//                     switch (message_gauche_re)
//                     {
//                         case 31 :
//                             choix_bras = 66 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 28 :
//                             choix_bras = 54 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 247 :
//                             choix_bras = 53 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 43 :
//                             choix_bras = 43 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 244 :
//                             choix_bras = 5 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 40 :
//                             choix_bras = 4 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                         case 3 : 
//                             choix_bras = 3 ;
//                             SendMsgCan(BRAS_RE, &choix_bras,sizeof(choix_bras));
//                             waitingAckFrom = 0;
//                             waitingAckID =0;
//                             break ;
//                     }
//                     message_droit_re = 0 ;
//                     message_gauche_re = 0 ;
                    
//                     TEST_RELACHER.Draw(BLEU, BLANC);
                    
//                 }
//             }
//             break;
        
//         case TEST_COULEUR:      
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Couleur", LEFT_MODE);
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_B.Draw(BLEU, BLANC);
//             TEST_C.Draw(BLEU, BLANC);
//             TEST_D.Draw(BLEU, BLANC);
//             RETOUR.Draw(0xFFFF0000,0);
//             while(strat_etat_s==TEST_COULEUR) {
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                     lcd.Clear (LCD_COLOR_WHITE);
//                 }else if(TEST_A.Touched()) {
//                     while (TEST_A.Touched());
//                     SendRawId(TEST_BRAS_A);
//                     TEST_A.Draw(BLEU, BLANC);
//                 } else if(TEST_B.Touched()) {
//                     while (TEST_B.Touched());
//                     SendRawId(TEST_BRAS_B);
//                     TEST_B.Draw(BLEU, BLANC);
//                 } else if(TEST_C.Touched()) {
//                     while (TEST_C.Touched());
//                     SendRawId(TEST_BRAS_C);
//                     TEST_C.Draw(BLEU, BLANC);
//                 } else if(TEST_D.Touched()) {
//                     while (TEST_D.Touched());
//                     SendRawId(TEST_BRAS_D);
//                     TEST_D.Draw(BLEU, BLANC);
//                 } 
//             }
//             break;

//         case TEST_MOTEUR:
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.Clear (LCD_COLOR_WHITE);   
//             lcd.Clear (LCD_COLOR_WHITE);
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Test Moteur", LEFT_MODE);
//             RETOUR.Draw(0xFFFF0000,0);
//             TEST_A.Draw(BLEU, BLANC);
//             TEST_B.Draw(BLEU, BLANC);
//             while(strat_etat_s==TEST_MOTEUR) {
//                 if(RETOUR.Touched()) {
//                     while (RETOUR.Touched());
//                     strat_etat_s=DEMO;
//                 } else if(TEST_A.Touched()) {
//                     while (TEST_A.Touched());
//                     BendRadius(150,1800,1,0);
//                     TEST_A.Draw(BLEU, BLANC);
//                 } else if(TEST_B.Touched()) {
//                     while (TEST_B.Touched());
//                     BendRadius(150,1800,0,0);
//                     TEST_B.Draw(BLEU, BLANC);
//                 } 
//             }
//             break;      ///////////////////////////////////////////FIN DES DEMOS/////////////////////////////////////////////////


//         case SELECT_SIDE :      // CHOIX DU COTE DU TERRAIN + INVERSION DE LA STRAT SI COTE ORANGE+ ENVOI DU COTE A LA CARTE CAPTEUR/ACTIONNEURS
//             lcd.Clear(LCD_COLOR_WHITE);
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);

//             lcd.DisplayStringAt(70, LINE(0), (uint8_t *)"Choisir le cote", LEFT_MODE);
//             COTE_JAUNE.Draw(JAUNE, 0);
//             COTE_VIOLET.Draw(VIOLET, 0);
//             RETOUR.Draw(LCD_COLOR_RED, 0);


//             while (strat_etat_s == SELECT_SIDE) {
//                 canProcessRx();
//                 if(COTE_JAUNE.Touched()) {
//                     Cote = 0x1;
//                     InversStrat = Cote;
//                     strat_etat_s = SELECT_ROB;
//                     CANMessage trame_Tx = CANMessage();
//                     trame_Tx.len = 1;
//                     trame_Tx.format = CANStandard;
//                     trame_Tx.type = CANData;
//                     trame_Tx.id=CHOICE_COLOR;
//                     trame_Tx.data[0]=Cote;
//                     can2.write(trame_Tx);
//                     while(COTE_JAUNE.Touched());

//                 }

//                 if(COTE_VIOLET.Touched()) {
//                     Cote = 0x0;
//                     InversStrat= Cote;
//                     strat_etat_s = SELECT_ROB;
//                     CANMessage trame_Tx = CANMessage();
//                     trame_Tx.len = 1;
//                     trame_Tx.format = CANStandard;
//                     trame_Tx.type = CANData;
//                     trame_Tx.id=CHOICE_COLOR;
//                     trame_Tx.data[0]=Cote;
//                     can2.write(trame_Tx);
//                     while(COTE_VIOLET.Touched());
//                 }

//                 if(RETOUR.Touched()) {
//                     strat_etat_s = CHOIX;
//                     while(RETOUR.Touched());
//                 }
//             }

//             break;
        
//         case SELECT_ROB :       // CHOIX DU SENS DE RECALAGE EN HAUT OU EN BAS
//             lcd.Clear(LCD_COLOR_WHITE);
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             lcd.DisplayStringAt(70, LINE(0), (uint8_t *)"Choisir la hauteur", LEFT_MODE);
            
//             if(Cote==0)
//             {
//                 COTE_HAUT.Draw(VIOLET, 0);
//                 COTE_BAS.Draw(VIOLET, 0);
//             }
//             else if(Cote==1)
//             {
//                 COTE_HAUT.Draw(JAUNE, 0);
//                 COTE_BAS.Draw(JAUNE, 0);
//             }
//             RETOUR.Draw(LCD_COLOR_RED, 0);


//             while (strat_etat_s == SELECT_ROB) 
//             {
//                 canProcessRx();
//                 if(COTE_HAUT.Touched()) 
//                 {
//                     Hauteur = 0x1;
//                     strat_etat_s = TACTIQUE;
//                     while(COTE_HAUT.Touched());
//                 }
//                 if(COTE_BAS.Touched()) {
//                     Hauteur = 0x0;
//                     strat_etat_s = TACTIQUE;
//                     while(COTE_BAS.Touched());
//                 }

//                 if(RETOUR.Touched()) {
//                     strat_etat_s = SELECT_SIDE;
//                     while(RETOUR.Touched());
//                 }
//             }
//             break;
        
//         case TACTIQUE : //AFFICHE LA LISTE DES STRATS AFIN DE SELECTIONNER CELLE VOULUE
//             if (Cote == 1) {
//                 lcd.Clear(JAUNE);
//                 lcd.SetBackColor(JAUNE);
//             } else if (Cote == 0) {
//                 lcd.Clear(VIOLET);
//                 lcd.SetBackColor(VIOLET);
//             } else {
//                 lcd.Clear(BLEU);
//                 lcd.SetBackColor(BLEU);
//             }

//             lcd.SetTextColor(LCD_COLOR_BLACK);

//             lcd.DisplayStringAt(20, LINE(0), (uint8_t *)"Choisir une strategie", LEFT_MODE);

//             nbStrat = Bouton_Strat(); // retourne valeur de Strategie si bouton strat renvoi -1 on reviens en arriere
//             if (nbStrat == -1) {
//                 strat_etat_s = SELECT_SIDE;
//             } else {
//                 strat_etat_s = DETAILS;
//             }
//             wait(0.1);
//             break;

//         case DETAILS :  //SECONDE VALIDATION DE LA STRAT
//             lcd.Clear(LCD_COLOR_WHITE);
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);
//             CHECK.Draw(VERT);
//             RETOUR.Draw(LCD_COLOR_RED);

//             SelectionStrat(nbStrat); //affiche la stratégie selectionnée

//             while (strat_etat_s == DETAILS) {
//                 canProcessRx();
//                 if (CHECK.Touched()) {
//                     if(gameEtat == ETAT_CONFIG) {
//                         gameEtat = ETAT_GAME_INIT;
//                         strat_etat_s=LECTURE;

//                     }
//                     while(CHECK.Touched());
//                 }

//                 if(RETOUR.Touched()) {
//                     strat_etat_s = TACTIQUE;
//                     while(RETOUR.Touched());
//                 }
//             }
//             break;


//         case LECTURE :
//             break;
//         case AFF_WAIT_JACK : //FONCTIONS D'AFFICHAGE DE L'ATTENTE DU JACK
//             lcd.Clear(BLANC);
//             lcd.SetBackColor(LCD_COLOR_WHITE);
//             lcd.SetTextColor(LCD_COLOR_BLACK);

//             if (Cote == 0) {
//                 lcd.Clear(LCD_COLOR_WHITE);
//                 lcd.SetBackColor(LCD_COLOR_WHITE);
//             } else if (Cote == 1) {
//                 lcd.Clear(LCD_COLOR_WHITE);
//                 lcd.SetBackColor(LCD_COLOR_WHITE);
//             } else {
//                 lcd.Clear(VERT);
//                 lcd.SetBackColor(VERT);
//             }
//             canProcessRx();
//             lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"En attente du Jack", CENTER_MODE);
//             strat_etat_s=WAIT_JACK;
            
//             break;

//         case WAIT_JACK :
//             if(unique == 0)
//             {
//                 lcd.SetTextColor(LCD_COLOR_BLUE);
//                 lcd.DrawRect(0,50,400,320);
//                 lcd.DisplayStringAt(100, LINE(8), (unsigned char *)"Score :", LEFT_MODE);
//                 lcd.SetTextColor(LCD_COLOR_RED);
//                 lcd.DrawRect(0,400,400,320);
//                 lcd.DisplayStringAt(100, LINE(28), (unsigned char *)"Timer :", LEFT_MODE);
//                 unique = 1;
//             }
            
            
//             lcd.SetTextColor(LCD_COLOR_BLACK);
            
//             lcd.DisplayStringAt(100, LINE(15), (unsigned char *)"girou :", LEFT_MODE);
//             sprintf(toto,"%02d",val_girou);
//             lcd.DisplayStringAt(250, LINE(15), (unsigned char *)toto, LEFT_MODE);
            
//             lcd.DisplayStringAt(100, LINE(16), (unsigned char *)"debug :", LEFT_MODE);
//             sprintf(toto,"%02d",debug_bon_port);
//             lcd.DisplayStringAt(250, LINE(16), (unsigned char *)toto, LEFT_MODE);
            
            
//             lcd.DisplayStringAt(100, LINE(10), (unsigned char *)"g_vert :", LEFT_MODE);
//             sprintf(toto,"%04d",gobelet_vert);
//             lcd.DisplayStringAt(250, LINE(10), (unsigned char *)toto, LEFT_MODE);
            
//             lcd.DisplayStringAt(100, LINE(11), (unsigned char *)"g_rouge :", LEFT_MODE);
//             sprintf(toto,"%04d",gobelet_rouge);
//             lcd.DisplayStringAt(250, LINE(11), (unsigned char *)toto, LEFT_MODE);
            
//             lcd.DisplayStringAt(100, LINE(12), (unsigned char *)"g_port :", LEFT_MODE);
//             sprintf(toto,"%04d",gobelet_port);
//             lcd.DisplayStringAt(250, LINE(12), (unsigned char *)toto, LEFT_MODE);
            
    
//             actual_time = gameTimer.read();
// //            if(actual_time != old_time)
// //            {
//                 lcd.SetTextColor(LCD_COLOR_BLUE);
//                 sprintf(toto,"%hd",score_final);
//                 lcd.DisplayStringAt(250, LINE(8), (unsigned char *)toto, LEFT_MODE);
                
//                 lcd.SetTextColor(LCD_COLOR_RED);          
//                 sprintf(toto,"%hd",actual_time);
//                 lcd.DisplayStringAt(250, LINE(28), (unsigned char *)toto, LEFT_MODE);
// //                old_time = actual_time;
// //            }
//             break;
    
        
//         case FIN :  //AFFICHAGE DE FIN AVEC LE SCORE FINAL
// //            lcd.Clear(LCD_COLOR_WHITE);
// //            lcd.SetBackColor(LCD_COLOR_WHITE);


//             while(1); // force le redemarage du robot
//             //break;
//         }
//     }
// /****************************************************************************************/
// /* FUNCTION NAME: Bouton_Strat                                                          */
// /* DESCRIPTION  : Sélection de la strat sur le lcd puis envoie sur CAN (à modifier!)    */
// /****************************************************************************************/
// signed char Bouton_Strat (void)
// {
//     char Ack_strat = 0;
//     signed char Strat = 0;
//     Button STRAT_1 (0, 30, 190, 110, strat_sd[0]);
//     Button STRAT_2 (210, 30, 190, 110, strat_sd[1]);
//     Button STRAT_3 (0, 150, 190, 110, strat_sd[2]);
//     Button STRAT_4 (210, 150, 190, 110, strat_sd[3]);
//     Button STRAT_5 (0, 270, 190, 110,strat_sd[4]);
//     Button STRAT_6 (210, 270, 190, 110, strat_sd[5]);
//     Button STRAT_7 (0, 390, 190, 110, strat_sd[6]);
//     Button STRAT_8 (210, 390, 190, 110, strat_sd[7]);
//     Button STRAT_9 (0, 510, 190, 110, strat_sd[8]);
//     Button STRAT_10 (210, 510, 190, 110, strat_sd[9]);
//     Button RETOUR  (0, 680, 400, 110, "--Precedent--");
//     //Definition des boutons

//     Ack_strat = 0;
//     Strat = 0;
//     STRAT_1.Draw(0xFFF0F0F0, 0);
//     STRAT_2.Draw(0xFFF0F0F0, 0);
//     STRAT_3.Draw(0xFFF0F0F0, 0);
//     STRAT_4.Draw(0xFFF0F0F0, 0);
//     STRAT_5.Draw(0xFFF0F0F0, 0);
//     STRAT_6.Draw(0xFFF0F0F0, 0);
//     STRAT_7.Draw(0xFFF0F0F0, 0);
//     STRAT_8.Draw(0xFFF0F0F0, 0);
//     STRAT_9.Draw(0xFFF0F0F0, 0);
//     STRAT_10.Draw(0xFFF0F0F0, 0);
//     RETOUR.Draw(0xFFFF0000, 0);

//     while(Ack_strat == 0) {
//         canProcessRx();
//         CANMessage msgTx=CANMessage();
//         //msgTx.id=ECRAN_CHOICE_STRAT;
//         if (RETOUR.Touched())
//             return -1;
//         while(RETOUR.Touched());
//         //////////////////////////////STRATEGIE N°1
//         if (STRAT_1.Touched()) {
//             Strat = 0;
//             //msgTx.data[0] = 0x1;
//             //can2.write(msgTx);
//             while(STRAT_1.Touched());
//             Ack_strat =1;
//         }
//         /////////////////////////////STRATEGIE N°2
//         if (STRAT_2.Touched()) {
//             Strat = 1;
//             //msgTx.data[0] = 0x2;
//             //can2.write(msgTx);
//             while(STRAT_2.Touched());
//             Ack_strat =1;
//         }
//         //////////////////////////////STRATEGIE N°3
//         if (STRAT_3.Touched()) {
//             Strat = 2;
//             //msgTx.data[0] = 0x3;
//             //can2.write(msgTx);
//             while(STRAT_3.Touched());
//             Ack_strat =1;
//         }
//         /////////////////////////////STRATEGIE N°4
//         if (STRAT_4.Touched()) {
//             Strat = 3;
//             //msgTx.data[0] = 0x4;
//             //can2.write(msgTx);
//             while(STRAT_4.Touched());
//             Ack_strat =1;
//         }
//         ///////////////////////////////STRATEGIE N°5
//         if (STRAT_5.Touched()) {
//             Strat = 4;
//             //msgTx.data[0] = 0x5;
//             //can2.write(msgTx);
//             while(STRAT_5.Touched());
//             Ack_strat =1;
//         }
//         ////////////////////////////////STRATEGIE N°6
//         if (STRAT_6.Touched()) {
//             Strat = 5;
//             //msgTx.data[0] = 0x6;
//             //can2.write(msgTx);
//             while(STRAT_6.Touched());
//             Ack_strat =1;
//         }
//         /////////////////////////////////STRATEGIE N°7
//         if (STRAT_7.Touched()) {
//             Strat = 6;
//             //msgTx.data[0] = 0x7;
//             //can2.write(msgTx);
//             while(STRAT_7.Touched());
//             Ack_strat =1;
//         }
//         /////////////////////////////////STRATEGIE N°8
//         if (STRAT_8.Touched()) {
//             Strat = 7;
//             //msgTx.data[0] = 0x8;
//             //can2.write(msgTx);
//             while(STRAT_8.Touched());
//             Ack_strat =1;
//         }
//         /////////////////////////////////STRATEGIE N°9
//         if (STRAT_9.Touched()) {
//             Strat = 8;
//             //msgTx.data[0] = 0x9;
//             //can2.write(msgTx);
//             while(STRAT_9.Touched());
//             Ack_strat =1;
//         }
//         ///////////////////////////////////STRATEGIE N°10
//         if (STRAT_10.Touched()) {
//             Strat = 9;
//             //msgTx.data[0] = 0xA;
//             //can2.write(msgTx);
//             while(STRAT_10.Touched());
//             Ack_strat =1;
//         }

//     }
//     return Strat;

// }
// /****************************************************************************************/
// /* FUNCTION NAME: SelectionStrat                                                        */
// /* DESCRIPTION  : Affiche la Stratégie sélectionnée sur l'ihm                           */
// /****************************************************************************************/


// void SelectionStrat (unsigned char Strategie)
// {
//     lcd.SetBackColor(LCD_COLOR_WHITE);
//     lcd.SetTextColor(LCD_COLOR_BLACK);

//     switch (Strategie+1) {
//         case 0x1 :
//             //description de Strategie n°1
//             lcd.DisplayStringAt(150, 0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x2 :
//             //description de Strategie n°2
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x3 :
//             //description de Strategie n°3
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x4 :
//             //description de Strategie n°4
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x5 :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x6 :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x7 :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x8 :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0x9 :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;

//         case 0xA :
//             //description de Strategie n°5
//             lcd.DisplayStringAt(150,0, (uint8_t *)strat_sd[Strategie], LEFT_MODE);
//             break;
//     }
// }

// void DrawC (int l,int h,int x, int y, unsigned char fill){
//     Point c[8]={{l,0},{l,h/6},{l/5,h/3},{l/5,2*(h/3)},{l,5*(h/6)},{l,h},{0,5*(h/6)},{0,h/6}};
//     for(int a=0;a<8;a++){
//         c[a].X+=x;
//         c[a].Y+=y;
//     }
//     lcd.DrawPolygon(c, 8);
    
// }
// void DrawR (int l,int h,int x, int y, unsigned char fill){
//     Point r[10]={{0,0},{5*(l/6),0},{l,h/3},{5*(l/6),h/2},{l/2,h/2},{l,h},{2*(l/3),h},{l/4,h/2},{l/4,h},{0,h}};
//     for(int a=0;a<10;a++){
//         r[a].X+=x;
//         r[a].Y+=y;
//     }
//     lcd.DrawPolygon(r, 10);
// }
// void DrawA (int l,int h,int x, int y, unsigned char fill){
//     Point a[8]={{l/4,0},{3*(l/4),0},{l,h},{5*(l/6),h},{4*(l/6),h/2},{2*(l/6),h/2},{l/6,h},{0,h}};
//     for(int z=0;z<8;z++){
//         a[z].X+=x;
//         a[z].Y+=y;
//     }
//     lcd.DrawPolygon(a, 8);
// }
// void DrawCheck (int l,int h,int x, int y, int cochage){
//     if(x<3)x=3;
//     if(cochage == 1){
//         lcd.SetTextColor(VERT_F);
//         lcd.DrawLine(x,y-(h/2),x+l/2,y+h/2);
//         lcd.DrawLine(x-1,y-(h/2),x+l/2,y+h/2+1);
//         lcd.DrawLine(x-2,y-(h/2),x+l/2,y+h/2+2);
//         lcd.DrawLine(x+l/2,y+h/2,x+(4*(l/3)),y);
//         lcd.DrawLine(x+l/2,y+h/2+1,x+(4*(l/3))+1,y);
//         lcd.DrawLine(x+l/2,y+h/2+2,x+(4*(l/3))+2,y);
//         lcd.SetTextColor(NOIR);
//     } else {
//         Point a[4]={{0,0},{l,0},{l,h},{0,h}};
//         for(int z=0;z<8;z++){
//             a[z].X+=x;
//             a[z].Y+=y;
//         }
//         lcd.DrawPolygon(a, 4);
//     }
// }