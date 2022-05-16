#ifndef ROBOT_H
#define ROBOT_H
#include <memory>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/HMI.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Locomotion/Locomotion.h"

namespace rd {
class Robot {
   public:
    Locomotion locomotion;
    Arm arm1;
    Arm arm2;
    Hat hat;
    StackManager stackManager;
    HMI hmi;

   protected:
};
}  // namespace rd

#endif /* ROBOT_H */
