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

