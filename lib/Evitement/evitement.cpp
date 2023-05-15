#include "global.h"
#include "evitement.h"
#include <strategie.h>
#include "math.h"

int Evitement::lidar_danger(short x_obstacle, short y_obstacle, signed short angle_obstacle, int distance)
{

    signed short debut_angle_detection = theta_robot - 225, fin_angle_detection = theta_robot + 225;
    int distance_lim = 500;

    if(x_obstacle<=0 || y_obstacle<=0){return NO_POINT;}
    // si angle_obstacle E [debut_angle_detection ; fin_angle_detection] angle_obstacle en dizieme de degree
    // if (angle_obstacle >= debut_angle_detection && angle_obstacle <= fin_angle_detection)
    // {
        // int delta_x = x_robot - x_obstacle, delta_y = y_robot - y_obstacle;
        // int distance = sqrt((delta_x * delta_x) + (delta_y * delta_y));
        if (distance > 0 && distance < distance_lim)
        {
            if ((actionPrecedente == MV_COURBURE) || (actionPrecedente == MV_LINE) || (actionPrecedente == MV_XYT))
            {
                return DANGER_MV;//S'arreter
            }else{
                return DANGER_ST;
            }
        }
        else
        {
            int coef_directeur_robot = (target_y_robot - y_robot) / (target_x_robot - x_robot);
            int coef_directeur_obstacle = (y_obstacle - y_robot) / (x_obstacle - x_robot);

            // Calcul de la différence entre les coefficients directeurs
            float diff_coef_directeur = fabs(coef_directeur_robot - coef_directeur_obstacle);

            // Définir une marge d'erreur acceptable
            float tolerance = 0.1; // Valeur à ajuster en fonction de la précision souhaitée

            // Vérifier si la différence des coefficients directeurs est inférieure à la tolérance
            if (diff_coef_directeur < tolerance)
            {
                return OBSTACLE_SUR_TRAJECTOIRE;
            }
            else
            {
                return NO_DANGER;
            }
            // if (coef_directeur_robot == coef_directeur_obstacle)
            // {
            //     return OBSTACLE_SUR_TRAJECTOIRE;
            // }
            // else
            // {
            //     return NO_DANGER;
            // }
        }
    // }
    return NO_POINT;
}

void Evitement::lidar_end_danger(Instruction* instruction, S_Dodge_queue* dodgeq, signed short local_target_x_robot, signed short local_target_y_robot, signed short local_target_theta_robot){
    
        switch(instruction->order){
            case MV_RECALAGE:
                gameEtat=ETAT_GAME_PROCESInstruction;  
                             
            break;
            
            case MV_LINE:
                gameEtat=ETAT_GAME_PROCESInstruction;
                instruction->order = MV_XYT;
                instruction->arg1 = local_target_x_robot;// X
                instruction->arg2 = local_target_y_robot;// Y
                instruction->arg3 = local_target_theta_robot;// T
                    
            break;
            case MV_TURN:
                gameEtat=ETAT_GAME_PROCESInstruction;
                instruction->order = MV_XYT;
                instruction->arg1 = local_target_x_robot;// X
                instruction->arg2 = local_target_y_robot;// Y
                instruction->arg3 = local_target_theta_robot;// T
                    
            break;
            case MV_XYT:
                gameEtat=ETAT_GAME_PROCESInstruction;
                    
            break;
            case MV_COURBURE:
                short alpha;
                gameEtat=ETAT_GAME_PROCESInstruction;
                instruction->order=MV_XYT;
                if(instruction->direction==LEFT) 
                    alpha=(dodgeq->inst[0].arg3-theta_robot);
                else 
                    alpha=(theta_robot-dodgeq->inst[0].arg3);
                if(alpha>3600)
                    alpha=alpha-3600;
                if(alpha<-3600)
                    alpha=alpha+3600;   
                if(alpha<0)
                    alpha=-alpha;
                if(alpha<450)
                {
                    dodgeq->nb=0;
                    instruction->arg1=dodgeq->inst[0].arg1;//x
                    instruction->arg2=dodgeq->inst[0].arg2;//y
                    instruction->arg3=dodgeq->inst[0].arg3;//t
                } 
                else if(alpha<900)
                { 
                    dodgeq->nb=1;
                    instruction->arg1=dodgeq->inst[1].arg1;//x
                    instruction->arg2=dodgeq->inst[1].arg2;//y
                    instruction->arg3=dodgeq->inst[1].arg3;//t
                } else if(alpha<1350){ 
                    dodgeq->nb=2;
                    instruction->arg1=dodgeq->inst[2].arg1;//x
                    instruction->arg2=dodgeq->inst[2].arg2;//y
                    instruction->arg3=dodgeq->inst[2].arg3;//t
                } else if(alpha<1800){ 
                    dodgeq->nb=3;
                    instruction->arg1=dodgeq->inst[3].arg1;//x
                    instruction->arg2=dodgeq->inst[3].arg2;//y
                    instruction->arg3=dodgeq->inst[3].arg3;//t
                } else if(alpha<2250){ 
                    dodgeq->nb=4;
                    instruction->arg1=dodgeq->inst[4].arg1;//x
                    instruction->arg2=dodgeq->inst[4].arg2;//y
                    instruction->arg3=dodgeq->inst[4].arg3;//t
                } else { 
                    dodgeq->nb=5;
                    instruction->arg1=dodgeq->inst[5].arg1;//x
                    instruction->arg2=dodgeq->inst[5].arg2;//y
                    instruction->arg3=dodgeq->inst[5].arg3;//t
                } 
                    
            break;
        }

}


