#ifndef __herkulex_H
#define __herkulex_H

#include "threadCAN.h"

class herkulex {
protected:
    ThreadCAN *m_can;

public:
    herkulex(ThreadCAN &threadCAN);
    void controlePince(uint8_t Etage, uint8_t etatHerkulex, uint8_t sens);

};

#endif
