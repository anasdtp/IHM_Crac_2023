#ifndef __CONFIG_H
#define __CONFIG_H

#include <mIni.h>

static mINI::INIStructure config;

bool readConfig();
bool writeConfig();

#endif
