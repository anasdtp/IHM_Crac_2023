#ifndef CRAC_INSTRUCTION
#define CRAC_INSTRUCTION


enum E_InstructionType
{
    MV_BEZIER,
    MV_COURBURE,    // C -> Courbure
    MV_LINE,        // L -> Ligne droite
    MV_TURN,        // T -> Rotation sur place
    MV_XYT,         // X -> Aller à
    MV_RECALAGE,    // R -> Recalage bordure
    ACTION,         // A -> Action
    UNKNOWN,        // Erreur, instruction inconnue
    POSITION_DEBUT  // P
};
enum E_InstructionDirection
{
    NODIRECTION,     // N -> Parametre absent
    BACKWARD,
    FORWARD,
    RELATIVE,
    ABSOLUTE,
    LEFT,
    RIGHT
};
enum E_InstructionPrecisionOuRecalage
{
    NOPRECISION,// N -> Parametre absent
    PRECISION,  // P -> Precision, verifier la position à la fin du mouvement et refaire un XYT si erreur > 1cm
    RECALAGE_X, // X -> Recalage en X, indique un recalage sur l'axe X
    RECALAGE_Y,  // Y -> Recalage en Y, indique un recalage sur l'axe Y
    RECALAGE_T
};
enum E_InstructionNextActionType
{
    NONEXTACTION,    // N -> Parametre absent
    JUMP, 
    WAIT,
    ENCHAINEMENT,
    MECANIQUE,
    CAPTEUR
};
enum E_InstructionNextActionJumpType
{
    NONEXTACTIONJUMPTYPE,    // N -> Parametre absent
    JUMP_TIME,
    JUMP_POSITION
};
struct S_Instruction
{
    short lineNumber;//Numéro de la ligne
    enum E_InstructionType order; //Type de l'instruction
    enum E_InstructionDirection direction; //BackWard ou Forward || Relative ou Absolu
    
    unsigned short  arg1;
    unsigned short  arg2;
    signed   short  arg3;
    
    enum E_InstructionPrecisionOuRecalage    precision;
    enum E_InstructionNextActionType         nextActionType;
    enum E_InstructionNextActionJumpType     jumpAction;
    unsigned short JumpTimeOrX;
    unsigned short JumpY;
    unsigned short nextLineOK;
    unsigned short nextLineError;
    
    unsigned short  arg4;
    unsigned short  arg5;
    signed   short  arg6;
};

struct S_Dodge_queue
{
    unsigned short nb;//Nombre d action en file dattente [0,9]
    struct S_Instruction inst[10];
};

/**
* Convertir un char en type d'instruction
**/
enum E_InstructionType charToInstructionType(char type);

/**
* 
**/
enum E_InstructionDirection charToInstructionDirection(char type);

/**
* Convertir un char 
**/
enum E_InstructionPrecisionOuRecalage charToInstructionPrecisionOuRecalage(char type);

/**
* 
**/
enum E_InstructionNextActionType charToInstructionNextActionType(char type);

/**
* 
**/
enum E_InstructionNextActionJumpType charToInstructionNextActionJumpType(char type);

/****************************************************************************************/
/* FUNCTION NAME: stringToInstruction                                                   */
/* DESCRIPTION  : Conversion d'une ligne du fichier de strat en instruction             */
/****************************************************************************************/
struct S_Instruction stringToInstruction(char line[]);


#endif