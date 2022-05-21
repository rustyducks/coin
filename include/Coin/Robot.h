#ifndef ROBOT_H
#define ROBOT_H
#include <memory>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Finger.h"
#include "Coin/Actuator/HMI.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/ReplicaHolder.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Actuator/StatuetteArm.h"
#include "Coin/Communication/Ducklink/UDPDucklinkCommunication.h"
#include "Coin/Locomotion/Locomotion.h"

namespace rd {
class Robot {
   public:
    Robot(UDPDucklink& motorDucklink, UDPDucklink& ioDucklink, UDPDucklink& lidarDucklink, PositionControlParameters& positionControlParams);

    void sense();

    Locomotion locomotion;
    Arm arm1;
    Arm arm2;
    Hat hat;
    StackManager stackManager;
    HMI hmi;
    Finger finger;
    ReplicaHolder replicaHolder;
    StatuetteArm statuetteArm;

   protected:
    rd::UDPDucklink& motorDucklink_;
    rd::UDPDucklink& ioDucklink_;
    rd::UDPDucklink& lidarDucklink_;
};
}  // namespace rd

#endif /* ROBOT_H */
