#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include "Coin/Communication/CommunicationBase.h"
#include "Navigation/PositionControlBase.h"
#include "Navigation/PurePursuitControl.h"
#include "chrono"
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

    const PointOriented& robotPose() const { return robotPose_; }
    const Speed& robotSpeed() const { return robotSpeed_; }
    const std::vector<std::pair<int, Point>> getAdversaries() const { return adversaries_; }

    bool isRobotBlocked() const { return robotBlocked_; }
    double robotBlockedDuration() const;

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

    Speed lastCommand_;

    bool robotBlocked_;
    std::chrono::steady_clock::time_point robotBlockedSince_;
};

}  // namespace rd

#endif /* LOCOMOTION_H */
