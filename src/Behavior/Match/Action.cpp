#include "Coin/Behavior/Match/Action.h"

namespace rd {

Action::Action(const std::string& name, const PointOrientedPtr& goTo, const PointOrientedPtr& retract)
    : name_(name), goToPoint_(goTo), retractPoint_(retract), blockedDuration_(30.) {}

bool Action::goTo(const PointOriented& robotPose, Robot& robot) {
    if (goToPoint_ != nullptr) {
        if (!robot.holonomic()) {
            Trajectory traj = Path({robotPose, *goToPoint_}).computeSpeeds(500., 500., 500., 500.);
            robot.locomotion.followTrajectory(traj);
        } else {
            robot.locomotion.goToPointHolonomic(*goToPoint_);
        }
        return true;
    }
    return false;
}
bool Action::retract(const PointOriented& robotPose, Robot& robot) {
    if (retractPoint_ != nullptr) {
        if (!robot.holonomic()) {
            Trajectory traj = Path({robotPose, *retractPoint_}).computeSpeeds(500., 500., 500., 500.);
            robot.locomotion.followTrajectory(traj);
        } else {
            robot.locomotion.goToPointHolonomic(*retractPoint_);
        }
        return true;
    }
    return false;
}
}  // namespace rd
