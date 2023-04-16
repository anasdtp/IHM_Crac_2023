#ifndef CRAC_DEBUG
#define CRAC_DEBUG
//debug
#include "global.h"
#include <Instruction.h>

void debug_Instruction(struct S_Instruction instruction);

const char* InstructionTypeToString(enum E_InstructionType type);

const char* InstructionDirectionToString(enum E_InstructionDirection type);

const char* InstructionPrecisionOuRecalageToString(enum E_InstructionPrecisionOuRecalage type);

const char* InstructionNextActionTypeToString(enum E_InstructionNextActionType type);

const char* InstructionNextActionJumpTypeToString(enum E_InstructionNextActionJumpType type);




#endif