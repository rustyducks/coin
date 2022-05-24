#include "Coin/Behavior/Match/Action.h"

namespace rd {

Action::Action(const std::string& name, const PointOrientedPtr& goTo, const PointOrientedPtr& retract)
    : name_(name), goToPoint_(goTo), retractPoint_(retract), blockedDuration_(30.) {}

bool Action::goTo(const PointOriented&, Robot& robot) {
    if (goToPoint_ != nullptr) {
        if (!robot.holonomic()) {
            robot.locomotion.goToPointDiff(*goToPoint_, false, true);
        } else {
            robot.locomotion.goToPointHolonomic(*goToPoint_);
        }
        return true;
    }
    return false;
}
bool Action::retract(const PointOriented&, Robot& robot) {
    if (retractPoint_ != nullptr) {
        if (!robot.holonomic()) {
            robot.locomotion.goToPointDiff(*retractPoint_, false, true);
        } else {
            robot.locomotion.goToPointHolonomic(*retractPoint_);
        }
        return true;
    }
    return false;
}
}  // namespace rd
