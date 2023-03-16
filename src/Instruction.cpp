// #include "global.h"

// enum E_InstructionType charToInstructionType(char type)
// {
//     switch(type)
//     {
//         case 'B': return MV_BEZIER;
//         case 'C': return MV_COURBURE;
//         case 'L': return MV_LINE;
//         case 'T': return MV_TURN;
//         case 'X': return MV_XYT;
//         case 'R': return MV_RECALAGE;
//         case 'A': return ACTION;
//         case 'P': return POSITION_DEBUT;
//         default:  return UNKNOWN;
//     }    
// }

// enum E_InstructionDirection charToInstructionDirection(char type)
// {
//     switch(type)
//     {
//         case 'B': return BACKWARD;
//         case 'F': return FORWARD;
//         case 'R': return RELATIVE;
//         case 'A': return ABSOLUTE;
//         case 'L': return LEFT;
//         default:  return NODIRECTION;
//     } 
// }

// enum E_InstructionPrecisionOuRecalage charToInstructionPrecisionOuRecalage(char type)
// {
//     switch(type)
//     {
//         case 'P': return PRECISION;
//         case 'X': return RECALAGE_X;
//         case 'Y': return RECALAGE_Y;
//         case 'T': return RECALAGE_T;
//         default:  return NOPRECISION;
//     } 
// }

// enum E_InstructionNextActionType charToInstructionNextActionType(char type)
// {
//     switch(type)
//     {
//         case 'J': return JUMP;
//         case 'W': return WAIT;
//         case 'E': return ENCHAINEMENT;
//         case 'M': return MECANIQUE;
//         case 'C': return CAPTEUR;
//         default:  return NONEXTACTION;
//     } 
// }

// enum E_InstructionNextActionJumpType charToInstructionNextActionJumpType(char type)
// {
//     switch(type)
//     {
//         case 'T': return JUMP_TIME;
//         case 'P': return JUMP_POSITION;
//         default:  return NONEXTACTIONJUMPTYPE;
//     } 
// }

// /****************************************************************************************/
// /* FUNCTION NAME: stringToInstruction                                                   */
// /* DESCRIPTION  : Conversion d'une ligne du fichier de strat en instruction             */
// /****************************************************************************************/
// struct S_Instruction stringToInstruction(char line[]) {
//     struct S_Instruction instruction;
    
//     char instructionOrder;
//     char instructionDirection;
//     char instructionPrecision;
//     char instructionNextActionType;
//     char instructionJumpAction;
//     int errorCode = 0;
//     /*
//     Info sur la fonction sscanf
//     %d -> Entier signé
//     %u -> Entié non signé
//     %c -> char
//     */
//   errorCode = sscanf(line, "%hd,%c,%c,%hu,%hu,%hd,%c,%c,%c,%hu,%hu,%hd,%hd,%hu,%hu,%hd",
//         &instruction.lineNumber,
//         &instructionOrder,
//         &instructionDirection,
//         &instruction.arg1,
//         &instruction.arg2,
//         &instruction.arg3,
//         &instructionPrecision,
//         &instructionNextActionType,
//         &instructionJumpAction,
//         &instruction.JumpTimeOrX,
//         &instruction.JumpY,
//         &instruction.nextLineOK,
//         &instruction.nextLineError,
//         &instruction.arg4,
//         &instruction.arg5,
//         &instruction.arg6
//     );
//     /*
//     if(errorCode != 13) {
//         errorInstructionLoop();//L'instruction est pas bonne !!  
//     }*/
    
//     instruction.order           = charToInstructionType(instructionOrder);
//     instruction.direction       = charToInstructionDirection(instructionDirection);
//     instruction.precision       = charToInstructionPrecisionOuRecalage(instructionPrecision);
//     instruction.nextActionType  = charToInstructionNextActionType(instructionNextActionType);
//     instruction.jumpAction      = charToInstructionNextActionJumpType(instructionJumpAction);
    
    
//     return instruction;
// }

// /****************************************************************************************/
// /* FUNCTION NAME: loadAllInstruction                                                    */
// /* DESCRIPTION  : Charger toutes les instructions du fichier de stratégie               */
// /*  Il faut utiliser strcpy(cheminFileStart,"/local/strat.txt");                        */
// /*   pour indiquer le fichier à utiliser                                                */
// /****************************************************************************************/
// void loadAllInstruction( signed char Strategie) {
    
//     struct S_Instruction instruction;
//     char LineBuffer[SIZE];
//     printf("Reading file : ");
//     printf(strat_sd[Strategie]);
//     printf("\n");
//     strcpy(PATH[Strategie],"/sd/");
//     strcat(PATH[Strategie],strat_sd[Strategie]);
//     strcat(PATH[Strategie],".txt");
//     FILE *testFile = fopen(PATH[Strategie], "rt"); //Ouverture du fichier en mode lecture seul au format string
    
//     nb_instructions = 0;
//     while (fgets(LineBuffer, SIZE, testFile) != NULL)  {
//         instruction = stringToInstruction(LineBuffer);
//         strat_instructions[nb_instructions] = instruction;
//         if(strat_instructions[nb_instructions].order == UNKNOWN) {
//             Button STRAT_1 (0, 30, 190, 110, PATH[Strategie]);
//             STRAT_1.Draw(0xFFF0F0F0, 0);
//             errorInstructionLoop();//L'instruction est pas bonne !!   
//         }
//         //printf(LineBuffer);
//         //debug_Instruction(instruction);
//         nb_instructions++;
//     }
//     printf("nb instruction = %d\n",nb_instructions);
//     fclose(testFile);
    
// }

// /****************************************************************************************/
// /* FUNCTION NAME: FileExists                                                            */
// /* DESCRIPTION  : Permet de vérifier si un fichier existe                               */
// /****************************************************************************************/
// int FileExists(const char *fname)
// {
//     FILE *file;
//     if (file == fopen(fname, "r"))
//     {
//         fclose(file);
//         return 1;
//     }
//     return 0;
// }
