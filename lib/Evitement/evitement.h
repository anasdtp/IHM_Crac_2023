#ifndef __EVITEMENT_H
#define __EVITEMENT_H
#include "instruction.h"

class Evitement {
// protected:


public:
   // Evitement();
    int lidar_danger(short x_obstacle, short y_obstacle, signed short angle_obstacle);
    void lidar_end_danger(Instruction* instruction, S_Dodge_queue* dodgeq, signed short local_target_x_robot, signed short local_target_y_robot, signed short local_target_theta_robot);
};

#endif