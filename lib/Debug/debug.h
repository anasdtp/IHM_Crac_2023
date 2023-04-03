#ifndef CRAC_DEBUG
#define CRAC_DEBUG
//debug
#include "global.h"
#include <Instruction.h>

void debug_Instruction(struct S_Instruction instruction);

char* InstructionTypeToString(enum E_InstructionType type);

char* InstructionDirectionToString(enum E_InstructionDirection type);

char* InstructionPrecisionOuRecalageToString(enum E_InstructionPrecisionOuRecalage type);

char* InstructionNextActionTypeToString(enum E_InstructionNextActionType type);

char* InstructionNextActionJumpTypeToString(enum E_InstructionNextActionJumpType type);




#endif