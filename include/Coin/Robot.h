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
#include "Coin/Table/Table.h"

namespace rd {
class Robot {
   public:
    using clock = std::chrono::system_clock;

    const int MATCH_DURATION = 100;      // sec
    const int ALMOST_END_DURATION = 95;  // sec
    Robot(UDPDucklink& motorDucklink, UDPDucklink& ioDucklink, UDPDucklink& lidarDucklink, PositionControlParameters& positionControlParams, bool holonomic,
          Table& table);

    enum eColor { UNKNOWN, YELLOW, PURPLE };

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
    eColor color;

    Table& table;

    bool holonomic() { return holonomic_; }
    bool matchStarted() { return matchStarted_; }
    const clock::time_point almostEndMatchTime() { return almostEndMatchTime_; }
    const clock::time_point endMatchTime() { return endMatchTime_; }

    void startMatch();

   protected:
    bool holonomic_;
    rd::UDPDucklink& motorDucklink_;
    rd::UDPDucklink& ioDucklink_;
    rd::UDPDucklink& lidarDucklink_;

    bool matchStarted_;
    clock::time_point almostEndMatchTime_;
    clock::time_point endMatchTime_;
};
}  // namespace rd

#endif /* ROBOT_H */
