#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include "Coin/Communication/CommunicationBase.h"
#include "Navigation/PositionControlBase.h"
#include "Navigation/PurePursuitControl.h"
#include "memory"

namespace rd {

class Locomotion {
   public:
    Locomotion(PositionControlParameters positionControlParameters_, CommunicationOutputBase& communicationBase);

    Speed run(const double dt);

    void followTrajectory(const Trajectory& traj);

    void updateRobotPose(PointOrientedInput pose) { robotPose_ = pose.getPoint(); }
    void updateRobotSpeed(SpeedInput speed) { robotSpeed_ = speed.getSpeed(); }
    void updateAdversaries(LidarAdversaries adv) { adversaries_ = adv.adversaries_; }
    void forceRobotPose(const PointOriented& pose) {
        robotPose_ = pose;
        communicationBase_.sendPoseCommand(pose);
    }

   protected:
    PointOriented robotPose_;
    Speed robotSpeed_;

    CommunicationOutputBase& communicationBase_;

    std::vector<std::pair<int, Point>> adversaries_;

    PositionControlParameters parameters_;
    enum ePositionControlType { IDLE, SPEED, POSITION_CONTROL };
    ePositionControlType positionControlType_;
    PurePursuitControl positionControl_;
    Speed targetSpeed_;
};

}  // namespace rd

#endif /* LOCOMOTION_H */
