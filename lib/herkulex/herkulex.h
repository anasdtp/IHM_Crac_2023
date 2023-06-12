#ifndef __herkulex_H
#define __herkulex_H

#include "threadCAN.h"

typedef enum{
    NO_COLOR,
    GREEN,
    RED,
    BLUE
} CouleurHerkulex;

typedef enum{
    NONE,
    DROIT,
    GAUCHE
}ASPIRATEUR;


class Herkulex {//Cela devrait plus se nommer Actionneurs, mais flemme de changer mtn
protected:
    ThreadCAN *m_can;

public:
 
    Herkulex(ThreadCAN &threadCAN);
    void controlePince(uint8_t Etage, uint8_t etatHerkulex, uint8_t sens);
    void changerIdHerkulexPince(uint8_t id);

    void stepMotorHauteur(int mm);
    void stepMotorMode(uint8_t mode);

    void controleHerkulexPosition(uint8_t IDHerkulex, short position, CouleurHerkulex setLed);
    void controleHerkulexPositionMulEnsemble(uint8_t IDHerkulex, short position1, uint8_t ID2Herkulex, short position2);
    void controleHerkulexTurnMode(uint8_t IDHerkulex, uint16_t vitesse);
    void controleHerkulexCouple(uint8_t IDHerkulex, bool couple = true);
    void clearHerkulex(uint8_t IDHerkulex);
    void poseCerise(bool presenceGatoInAccount = true);
    void controlePinceArriere(uint8_t etatPince, bool poseCerise);
    void controleAspirateur(ASPIRATEUR choix, bool activation);
    void controleLanceur(bool activation);
    

};


#endif
