#ifndef GLOBAL_H
#define GLOBAL_H

#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include "threadCAN.h"

#include "F469_GUI/F469_GUI.hpp"

extern LCD_DISCO_F469NI lcd;
extern TS_DISCO_F469NI ts;
extern ThreadCAN threadCAN;

#endif
