#include "global.h"
#include "asservissement.h"
#include "ident_crac.h"

/*********************************************************************************************/
/* FUNCTION NAME: GoToPosition                                                               */
/* DESCRIPTION  : Transmission CAN correspondant à un asservissement en position (x,y,theta) */
/*********************************************************************************************/
void GoToPosition (unsigned short x,unsigned short y,signed short theta,signed char sens)
{
    //id_to_expect=ACK_CONSIGNE;

    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_XYT; // tx nouvelle position en (x,y,theta)
    msgTx.len=7;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    // x sur 2 octets
    msgTx.data[0]=(unsigned char)x;
    msgTx.data[1]=(unsigned char)(x>>8);
    // y sur 2 octets
    msgTx.data[2]=(unsigned char)y;
    msgTx.data[3]=(unsigned char)(y>>8);
    // theta signé sur 2 octets
    msgTx.data[4]=(unsigned char)theta;
    msgTx.data[5]=(unsigned char)(theta>>8);
    msgTx.data[6]=sens;

    threadCAN.send(msgTx);
}

/****************************************************************************************/
/* FUNCTION NAME: Rotate                                                                */
/* DESCRIPTION  : Transmission CAN correspondant à une rotation                         */
/****************************************************************************************/
void Rotate (signed short angle)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_ROTATION;  // Tx rotation autour du centre du robot
    msgTx.len=2;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    //  Angle signé sur 2 octets
    msgTx.data[0]=(unsigned char)angle;
    msgTx.data[1]=(unsigned char)(angle>>8);

    threadCAN.send(msgTx);
}


/*********************************************************************************************/
/* FUNCTION NAME: GoStraight                                                                 */
/* DESCRIPTION  : Transmission CAN correspondant à une ligne droite, avec ou sans recalage   */
/*  recalage : 0 => pas de recalage                                                          */
/*             1 => recalage en X                                                            */
/*             2 => Recalage en Y                                                            */
/*  newValue : Uniquement en cas de recalage, indique la nouvelle valeur de l'odo            */
/*  isEnchainement : Indique si il faut executer l'instruction en enchainement               */
/*                   0 => non                                                                */
/*                   1 => oui                                                                */
/*                   2 => dernière instruction de l'enchainement                             */
/*********************************************************************************************/
void GoStraight (signed short distance,unsigned char recalage, unsigned short newValue, unsigned char isEnchainement)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_RECALAGE;
    msgTx.len=6;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    // x sur 2 octets
    msgTx.data[0]=(unsigned char)distance;
    msgTx.data[1]=(unsigned char)(distance>>8);
    //Recalage sur 1 octet
    msgTx.data[2]=recalage;
    //Valeur du recalage sur 2 octets
    msgTx.data[3]=(unsigned char)newValue;
    msgTx.data[4]=(unsigned char)(newValue>>8);
    //Enchainement sur 1 octet
    msgTx.data[5]=isEnchainement;

    threadCAN.send(msgTx);
    //wait_ms(500);
}

/********************************************************************************************/
/* FUNCTION NAME: BendRadius                                                                */
/* DESCRIPTION  : Transmission CAN correspondant à un rayon de courbure                     */
/********************************************************************************************/
void BendRadius (unsigned short rayon,signed short angle,signed char sens, unsigned char enchainement)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_COURBURE; // tx asservissement rayon de courbure
    msgTx.len=6;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    // Rayon sur 2 octets
    msgTx.data[0]=(unsigned char)rayon;
    msgTx.data[1]=(unsigned char)(rayon>>8);
    // Angle signé sur 2 octets
    msgTx.data[2]=(unsigned char)angle;
    msgTx.data[3]=(unsigned char)(angle>>8);
    // Sens signé sur 1 octet
    msgTx.data[4]=sens;
    // Enchainement sur 1 octet
    msgTx.data[5]=enchainement;

    threadCAN.send(msgTx);
}

void SetOdometrie (unsigned short canId, unsigned short x,unsigned short y,signed short theta)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=canId;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    msgTx.len=6;

        // x sur 2 octets
    msgTx.data[0]=(unsigned char)x;
    msgTx.data[1]=(unsigned char)(x>>8);
    // y sur 2 octets
    msgTx.data[2]=(unsigned char)y;
    msgTx.data[3]=(unsigned char)(y>>8);
    // theta signé sur 2 octets
    msgTx.data[4]=(unsigned char)theta;
    msgTx.data[5]=(unsigned char)(theta>>8);

    threadCAN.send(msgTx);
}

/****************************************************************************************/
/* FUNCTION NAME: setAsservissementEtat                                                 */
/* DESCRIPTION  : Activer ou désactiver l'asservissement                                */
/****************************************************************************************/
void setAsservissementEtat(unsigned char enable)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_ENABLE;  // Tx rotation autour du centre du robot
    msgTx.len=1;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    //  Angle signé sur 2 octets
    msgTx.data[0]=(unsigned char)((enable==0)?0:1);

    threadCAN.send(msgTx);
}


/****************************************************************************************/
/* FUNCTION NAME: SendSpeed accel decel                                                             */
/* DESCRIPTION  : Envoie un asservissement paramètre retournant à une vitesse           */
/****************************************************************************************/
void SendSpeedAccelDecel (unsigned short vitesse, unsigned short acceleration,unsigned short deceleration)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_CONFIG;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    msgTx.len=8;
    msgTx.data[0]=(unsigned char)(vitesse&0x00FF);
    msgTx.data[1]=(unsigned char)((vitesse&0xFF00)>>8);
    
    msgTx.data[2]=(unsigned char)(acceleration&0x00FF);
    msgTx.data[3]=(unsigned char)((acceleration&0xFF00)>>8);
    
    msgTx.data[4]=(unsigned char)(deceleration&0x00FF);
    msgTx.data[5]=(unsigned char)((deceleration&0xFF00)>>8);
    
    msgTx.data[6]=(unsigned char)(acceleration&0x00FF);//cloto
    msgTx.data[7]=(unsigned char)((acceleration&0xFF00)>>8);//cloto

    threadCAN.send(msgTx);
        
}

/****************************************************************************************/
/* FUNCTION NAME: SendSpeed                                                             */
/* DESCRIPTION  : Envoie un asservissement paramètre retournant à une vitesse           */
/****************************************************************************************/
void SendSpeed (unsigned short vitesse)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_CONFIG_VIT;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    msgTx.len=2;
    msgTx.data[0]=(unsigned char)(vitesse&0x00FF);
    msgTx.data[1]=(unsigned char)((vitesse&0xFF00)>>8);

    threadCAN.send(msgTx);
        
}

/****************************************************************************************/
/* FUNCTION NAME: SendSpeed                                                             */
/* DESCRIPTION  : Envoie un asservissement paramètre retournant à une vitesse           */
/****************************************************************************************/
void SendAccel(unsigned short acceleration,unsigned short deceleration)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_CONFIG_ACCEL;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    msgTx.len=4;
    msgTx.data[0]=(unsigned char)(acceleration&0x00FF);
    msgTx.data[1]=(unsigned char)((acceleration&0xFF00)>>8);
    
    msgTx.data[2]=(unsigned char)(deceleration&0x00FF);
    msgTx.data[3]=(unsigned char)((deceleration&0xFF00)>>8);

    threadCAN.send(msgTx);
        
}
void courbeBezier(int nbCourbes, short P1[][2], short C1[][2], short C2[][2], char sens)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_BEZIER; // tx nouvelle position en (x,y,theta)
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    
    msgTx.len=2;
    
    msgTx.data[0]=(unsigned char)nbCourbes;
    msgTx.data[1]=sens;
    
    threadCAN.send(msgTx);
        
    wait_ms(150);
    
    
    for (int i = 0; i < nbCourbes; i++)
    {
        msgTx.len=7;
        // x sur 2 octets
        msgTx.data[0]=(unsigned char)P1[i][0];
        msgTx.data[1]=(unsigned char)(P1[i][0]>>8);
        // y sur 2 octets
        msgTx.data[2]=(unsigned char)P1[i][1];
        msgTx.data[3]=(unsigned char)(P1[i][1]>>8);
        
        msgTx.data[4]=(unsigned char)C1[i][0];
        msgTx.data[5]=(unsigned char)(C1[i][0]>>8);

        msgTx.data[6]=(unsigned char)i;
    
        threadCAN.send(msgTx);
        
        wait_us(150);
        
        
        msgTx.len=7;
        // y sur 2 octets
        msgTx.data[0]=(unsigned char)C1[i][1];
        msgTx.data[1]=(unsigned char)(C1[i][1]>>8);
        // x sur 2 octets
        msgTx.data[2]=(unsigned char)C2[i][0];
        msgTx.data[3]=(unsigned char)(C2[i][0]>>8);
        // y sur 2 octets
        msgTx.data[4]=(unsigned char)C2[i][1];
        msgTx.data[5]=(unsigned char)(C2[i][1]>>8);
        
        msgTx.data[6]=(unsigned char)(i + 100);//Bidouille pour envoyer les points en deux trames
    
        threadCAN.send(msgTx);
        
        wait_us(150);
    }
}



/****************************************************************************************/
/* FUNCTION NAME: SendSpeedDecel                                                        */
/* DESCRIPTION  : Envoie un asservissement paramètre retournant à une vitesse           */
/****************************************************************************************/
 /*
void SendSpeedDecel (unsigned short vitesse, unsigned short deceleration)
{
    CANMessage msgTx=CANMessage();
    msgTx.id=ASSERVISSEMENT_CONFIG_DECEL;
    msgTx.format=CANStandard;
    msgTx.type=CANData;
    msgTx.len=4;
    msgTx.data[0]=(unsigned char)(vitesse&0x00FF);
    msgTx.data[1]=(unsigned char)((vitesse&0xFF00)>>8);
    msgTx.data[2]=(unsigned char)(deceleration&0x00FF);
    msgTx.data[3]=(unsigned char)((deceleration&0xFF00)>>8);
 
    threadCAN.send(msgTx);
        
}*/
   
   