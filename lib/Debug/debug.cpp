#include "debug.h"

void debug_Instruction(struct S_Instruction instruction)
{
    printf("\n********* Debug instruction *********\n");
    
    printf("* Line      => %d\n", instruction.lineNumber);
    printf("* Type      => %s\n", InstructionTypeToString(instruction.order));
    printf("* Direction => %s\n", InstructionDirectionToString(instruction.direction));
    
    printf("* Arg1      => %d\n", instruction.arg1);
    printf("* Arg2      => %d\n", instruction.arg2);
    printf("* Arg3      => %d\n", instruction.arg3);
    
    printf("* Recalage  => %s\n", InstructionPrecisionOuRecalageToString(instruction.precision));
    printf("* NextAction=> %s\n", InstructionNextActionTypeToString(instruction.nextActionType));
    printf("* JumpAction=> %s\n", InstructionNextActionJumpTypeToString(instruction.jumpAction));
    
    printf("* JumpTimeOrX   => %d\n", instruction.JumpTimeOrX);
    printf("* JumpY         => %d\n", instruction.JumpY);
    printf("* nextLineOK    => %d\n", instruction.nextLineOK);
    printf("* nextLineError => %d\n", instruction.nextLineError);
    
    printf("*************************************\n");
}

const char* InstructionTypeToString(enum E_InstructionType type)
{
    switch(type)
    {
        case MV_COURBURE:       return "Courbure";
        case MV_LINE:           return "Ligne";
        case MV_TURN:           return "Rotation";
        case MV_XYT:            return "Position XYT";
        case MV_RECALAGE:       return "Recalage";
        case ACTION:            return "Action";
        case PINCE:             return "Pince";
        default:                return "Inconnue";
    }    
}
const char* InstructionDirectionToString(enum E_InstructionDirection type)
{
    switch(type)
    {
        case BACKWARD:      return "en arriere";
        case FORWARD:       return "en avant";
        case RELATIVE:      return "relatif";
        case ABSOLUTE:      return "absolu";
        default:            return "absent";
    }    
}
const char* InstructionPrecisionOuRecalageToString(enum E_InstructionPrecisionOuRecalage type)
{
    switch(type)
    {
        case PRECISION:     return "correction position en fin de mouvement";
        case RECALAGE_X:    return "recalage en X";
        case RECALAGE_Y:    return "recalage en Y";
        default:            return "absent";
    }    
}
const char* InstructionNextActionTypeToString(enum E_InstructionNextActionType type)
{
    switch(type)
    {
        case JUMP:          return "jump";
        case WAIT:          return "attente fin instruction";
        case ENCHAINEMENT:  return "enchainement";
        default:            return "absent";
    }    
}
const char* InstructionNextActionJumpTypeToString(enum E_InstructionNextActionJumpType type)
{
    switch(type)
    {
        case JUMP_TIME:         return "attente temps";
        case JUMP_POSITION:     return "attente position";
        default:                return "absent";
    }    
}
