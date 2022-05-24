#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include "Coin/Communication/CommunicationBase.h"
#include "Navigation/GoToPointHolonomic.h"
#include "Navigation/PolarControl.h"
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

    void goToPointDiff(const PointOriented& pt, bool backwards, bool withFirstRotation);
    void goToPointHolonomic(const PointOriented& pt);

    void updateRobotPose(PointOrientedInputWithTimestamp poseStamped) {
        robotPose_ = poseStamped.getPoint();
        lastUpdatedPoseTime_ = poseStamped.timestamp();
    }

    void predictRobotPose() {
        auto now = std::chrono::system_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdatedPoseTime_).count() / 1000000.;
        Speed delta = lastCommand_ * dt;  // Assumes speed has not changed since dt
        robotPose_ = robotPose_ + PointOriented(delta.vx(), delta.vy(), delta.vtheta());
    }

    void updateRobotSpeed(SpeedInput speed) { robotSpeed_ = speed.getSpeed(); }
    void updateAdversaries(LidarAdversaries adv) { adversaries_ = adv.adversaries_; }
    void forceRobotPose(const PointOriented& pose) {
        lastUpdatedPoseTime_ = std::chrono::system_clock::now();
        robotPose_ = pose;
        communicationBase_.sendPoseCommand(pose);
    }

    const PointOriented& robotPose() const { return robotPose_; }
    const Speed& robotSpeed() const { return robotSpeed_; }
    const std::vector<std::pair<int, Point>> getAdversaries() const { return adversaries_; }

    bool isRobotBlocked() const { return robotBlocked_; }
    double robotBlockedDuration() const;

    bool isGoalReached() const { return positionControlType_ == IDLE; }

    void stop() { positionControlType_ = IDLE; }

   protected:
    PointOriented robotPose_;
    std::chrono::system_clock::time_point lastUpdatedPoseTime_;
    Speed robotSpeed_;

    CommunicationOutputBase& communicationBase_;

    std::vector<std::pair<int, Point>> adversaries_;

    PositionControlParameters parameters_;
    enum ePositionControlType { IDLE, SPEED, POSITION_CONTROL, GO_TO_HOLONOMIC, GO_TO_DIFF };
    ePositionControlType positionControlType_;
    PolarControl polarControl_;
    GoToPointHolonomic goToPointHolonomic_;
    PurePursuitControl positionControl_;

    Speed targetSpeed_;

    Speed lastCommand_;

    bool robotBlocked_;
    std::chrono::steady_clock::time_point robotBlockedSince_;
};

}  // namespace rd

#endif /* LOCOMOTION_H */
