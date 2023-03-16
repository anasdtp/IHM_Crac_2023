#ifndef CRAC_ASSERVISSEMENT
#define CRAC_ASSERVISSEMENT

void GoToPosition (unsigned short x,unsigned short y,signed short theta,signed char sens);

/****************************************************************************************/
/* FUNCTION NAME: Rotate                                                                */
/* DESCRIPTION  : Transmission CAN correspondant à une rotation                         */
/****************************************************************************************/

void Rotate (signed short angle);

/*********************************************************************************************/
/* FUNCTION NAME: GoStraight                                                                 */
/* DESCRIPTION  : Transmission CAN correspondant à une ligne droite, avec ou sans recalage   */
/*  recalage : 0 => pas de recalage                                                          */
/*             1 => recalage en X                                                            */
/*             2 => Recalage en Y                                                            */
/*  newValue : Uniquement en cas de recalage, indique la nouvelle valeur de l'odo            */
/*  isEnchainement : Indique si il faut executer l'instruction en enchainement               */
/*                   0 => non                                                                */
/*                   1 => oui                                                                */
/*                   2 => dernière instruction de l'enchainement                             */
/*********************************************************************************************/
void GoStraight (signed short distance,unsigned char recalage, unsigned short newValue, unsigned char isEnchainement);

/********************************************************************************************/
/* FUNCTION NAME: BendRadius                                                                */
/* DESCRIPTION  : Transmission CAN correspondant à un rayon de courbure                     */
/********************************************************************************************/
void BendRadius (unsigned short rayon,signed short angle,signed char sens, unsigned char enchainement);

void SetOdometrie (unsigned short canId, unsigned short x,unsigned short y,signed short theta);

/****************************************************************************************/
/* FUNCTION NAME: setAsservissementEtat                                                 */
/* DESCRIPTION  : Activer ou désactiver l'asservissement                                */
/****************************************************************************************/
void setAsservissementEtat(unsigned char enable);

/****************************************************************************************/
/* FUNCTION NAME: SendSpeed                                                             */
/* DESCRIPTION  : Envoie un asservissement paramètre retournant à une vitesse           */
/****************************************************************************************/
void SendSpeed (unsigned short vitesse);
void SendSpeedAccelDecel (unsigned short vitesse, unsigned short acceleration,unsigned short deceleration);
void SendAccel(unsigned short acceleration,unsigned short deceleration);
void courbeBezier(int nbCourbes, short P1[][2], short C1[][2], short C2[][2], char sens);
#endif