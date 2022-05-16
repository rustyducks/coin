#include "Coin/Behavior/Match/Action.h"

namespace rd {

Action::Action(const std::string& name, TrajectoryPtr goTo, TrajectoryPtr retract)
    : name_(name), goToApproachTraj_(goTo), retractTraj_(retract), blockedDuration_(30.) {}

}  // namespace rd
