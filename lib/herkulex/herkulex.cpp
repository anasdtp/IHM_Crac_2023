#include "identCrac.h"
#include "herkulex.h"

herkulex::herkulex(ThreadCAN &threadCAN)
{
    m_can = &threadCAN;
}

void herkulex::controlePince(uint8_t Etage, uint8_t etatHerkulex, uint8_t sens)
{
    CANMessage txMsg;
    txMsg.id = IDCAN_PINCE;
    txMsg.len = 3;
    txMsg.data[0] = Etage;
    txMsg.data[1] = etatHerkulex;
    txMsg.data[2] = sens;
    m_can->send(txMsg);
}

void herkulex::changerIdHerkulexPince(uint8_t id){
    m_can->send(IDCAN_PINCE_CHANGE_ID_HERKULEX, id);
}

void herkulex::stepMotorHauteur(int mm){
    unsigned int position = mm*3600/80.0;
    CANMessage txMsg;
    txMsg.id = IDCAN_PINCE_CHANGE_ID_HERKULEX;
    txMsg.len = 4;
    txMsg.data[0] = (position&0xFF);
    txMsg.data[1] = ((position>>8)&0xFF);
    txMsg.data[2] = ((position>>16)&0xFF);
    txMsg.data[3] = ((position>>24)&0XFF);
    m_can->send(txMsg);
}


void herkulex::controleHerkulexPosition(uint8_t IDHerkulex, short position, CouleurHerkulex setLed){
    char commande = 0, playtime = 0x3C, couleur = 0;
    if(setLed == VERT){couleur = 0x04;}else if(setLed == ROUGE){couleur = 0x08;}else if(setLed == BLEU){ couleur = 0x10;}
    CANMessage txMsg;
    txMsg.id = IDCAN_HERKULEX;
    txMsg.len = 6;
    txMsg.data[0] = IDHerkulex;
    txMsg.data[1] = commande;
    txMsg.data[2] = ((position)&0xFF);
    txMsg.data[3] = ((position>>8)&0XFF);
    txMsg.data[4] = playtime;
    txMsg.data[5] = couleur;
    m_can->send(txMsg);
}

void herkulex::controleHerkulexPosition_Mul_ensemble(uint8_t IDHerkulex, short position1, uint8_t ID2Herkulex, short position2){
    char commande = 1, playtime = 0x3C;
    CANMessage txMsg;
    txMsg.id = IDCAN_HERKULEX;
    txMsg.len = 8;
    txMsg.data[0] = IDHerkulex;
    txMsg.data[1] = commande;
    txMsg.data[2] = ((position1)&0xFF);
    txMsg.data[3] = ((position1>>8)&0XFF);
    txMsg.data[4] = ID2Herkulex;
    txMsg.data[5] = ((position2)&0xFF);
    txMsg.data[6] = ((position2>>8)&0XFF);
    txMsg.data[7] = playtime;
    m_can->send(txMsg);
}

 void herkulex::controleHerkulexTurnMode(uint8_t IDHerkulex, uint16_t vitesse){
    char setLed = 0x04;
    CANMessage txMsg;
    txMsg.id = ID_HERKULEX_VITESSE;
    txMsg.len = 4;
    txMsg.data[0] = IDHerkulex;
    txMsg.data[1] = ((vitesse)&0xFF);
    txMsg.data[2] = ((vitesse>>8)&0XFF);
    txMsg.data[3] = setLed;
    m_can->send(txMsg);
 }

 void herkulex::controleHerkulexCouple(uint8_t IDHerkulex, bool couple){
    CANMessage txMsg;
    txMsg.id = IDCAN_HERKULEX_Torque;
    txMsg.len = 2;
    txMsg.data[0] = IDHerkulex;
    if(couple){txMsg.data[1] = 0x60;}

    m_can->send(txMsg);
 }

 void herkulex::clearHerkulex(uint8_t IDHerkulex){
    m_can->send(IDCAN_HERKULEX_Clear, IDHerkulex);
 }