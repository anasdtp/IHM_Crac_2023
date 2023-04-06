#include <global.h>
#include <Instruction.h>
enum E_InstructionType charToInstructionType(char type)
{
    switch(type)
    {
        case 'B': return MV_BEZIER;
        case 'C': return MV_COURBURE;
        case 'L': return MV_LINE;
        case 'T': return MV_TURN;
        case 'X': return MV_XYT;
        case 'R': return MV_RECALAGE;
        case 'A': return ACTION;
        case 'H': return PINCE;
        case 'P': return POSITION_DEBUT;
        default:  return UNKNOWN;
    }    
}

enum E_InstructionDirection charToInstructionDirection(char type)
{
    switch(type)
    {
        case 'B': return BACKWARD;
        case 'F': return FORWARD;
        case 'R': return RELATIVE;
        case 'A': return ABSOLUTE;
        case 'L': return LEFT;
        default:  return NODIRECTION;
    } 
}

enum E_InstructionPrecisionOuRecalage charToInstructionPrecisionOuRecalage(char type)
{
    switch(type)
    {
        case 'P': return PRECISION;
        case 'X': return RECALAGE_X;
        case 'Y': return RECALAGE_Y;
        case 'T': return RECALAGE_T;
        default:  return NOPRECISION;
    } 
}

enum E_InstructionNextActionType charToInstructionNextActionType(char type)
{
    switch(type)
    {
        case 'J': return JUMP;
        case 'W': return WAIT;
        case 'E': return ENCHAINEMENT;
        case 'M': return MECANIQUE;
        case 'C': return CAPTEUR;
        default:  return NONEXTACTION;
    } 
}

enum E_InstructionNextActionJumpType charToInstructionNextActionJumpType(char type)
{
    switch(type)
    {
        case 'T': return JUMP_TIME;
        case 'P': return JUMP_POSITION;
        default:  return NONEXTACTIONJUMPTYPE;
    } 
}

/****************************************************************************************/
/* FUNCTION NAME: stringToInstruction                                                   */
/* DESCRIPTION  : Conversion d'une ligne du fichier de strat en instruction             */
/****************************************************************************************/
struct S_Instruction stringToInstruction(char line[]) {
    struct S_Instruction instruction;
    
    char instructionOrder;
    char instructionDirection;
    char instructionPrecision;
    char instructionNextActionType;
    char instructionJumpAction;
    int errorCode = 0;
    /*
    Info sur la fonction sscanf
    %d -> Entier signé
    %u -> Entié non signé
    %c -> char
    */
  errorCode = sscanf(line, "%hd,%c,%c,%hu,%hu,%hd,%c,%c,%c,%hu,%hu,%hd,%hd,%hu,%hu,%hd",
        &instruction.lineNumber,
        &instructionOrder,
        &instructionDirection,
        &instruction.arg1,
        &instruction.arg2,
        &instruction.arg3,
        &instructionPrecision,
        &instructionNextActionType,
        &instructionJumpAction,
        &instruction.JumpTimeOrX,
        &instruction.JumpY,
        &instruction.nextLineOK,
        &instruction.nextLineError,
        &instruction.arg4,
        &instruction.arg5,
        &instruction.arg6
    );
    /*
    if(errorCode != 13) {
        errorInstructionLoop();//L'instruction est pas bonne !!  
    }*/
    
    instruction.order           = charToInstructionType(instructionOrder);
    instruction.direction       = charToInstructionDirection(instructionDirection);
    instruction.precision       = charToInstructionPrecisionOuRecalage(instructionPrecision);
    instruction.nextActionType  = charToInstructionNextActionType(instructionNextActionType);
    instruction.jumpAction      = charToInstructionNextActionJumpType(instructionJumpAction);
    
    
    return instruction;
}

