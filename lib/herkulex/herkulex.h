#ifndef __herkulex_H
#define __herkulex_H

#include "threadCAN.h"

class herkulex {
protected:
    ThreadCAN *m_can;

public:
    typedef enum
    {
        VERT,
        ROUGE,
        BLEU
    }CouleurHerkulex;

    herkulex(ThreadCAN &threadCAN);
    void controlePince(uint8_t Etage, uint8_t etatHerkulex, uint8_t sens);
    void changerIdHerkulexPince(uint8_t id);

    void stepMotorHauteur(int mm);
    void stepMotorMode(uint8_t mode);

    void controleHerkulexPosition(uint8_t IDHerkulex, short position, CouleurHerkulex setLed);
    void controleHerkulexPosition_Mul_ensemble(uint8_t IDHerkulex, short position1, uint8_t ID2Herkulex, short position2);
    void controleHerkulexTurnMode(uint8_t IDHerkulex, uint16_t vitesse);
    void controleHerkulexCouple(uint8_t IDHerkulex, bool couple);
    void clearHerkulex(uint8_t IDHerkulex);

    

};

#endif
