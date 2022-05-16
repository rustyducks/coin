#include "Coin/Behavior/Match/ActionJuggler.h"

namespace rd {
ActionJuggler::ActionJuggler(Robot& robot) : BehaviorBase(robot) {}

void ActionJuggler::tick() {
    // TODO: add guards for going to start zone at the end of match and overtime stop
    switch (actionState_) {
        case GO_TO:
            if (robot_.locomotion.robotBlockedDuration() > action_->blockedDuration()) {
                nextAction_ = action_->onGoToBlocked();
                if (nextAction_ != nullptr) {
                    actionState_ = GO_TO;
                    action_ = nextAction_;
                }
            } else if (robot_.locomotion.isGoalReached()) {
                actionState_ = FSM;
            }
            break;
        case FSM:
            nextAction_ = action_->run(robot_);
            if (nextAction_ != nullptr) {
                action_->deinit(robot_);
                actionState_ = DEINIT;
            }
            break;
        case DEINIT:
            if (action_->isDeinit(robot_)) {
                TrajectoryPtr retractTraj = action_->getRetract();
                action_ = nextAction_;
                if (retractTraj != nullptr) {
                    actionState_ = RETRACT;
                    robot_.locomotion.followTrajectory(*retractTraj);
                } else if (action_->getGoTo() != nullptr) {
                    actionState_ = GO_TO;
                    robot_.locomotion.followTrajectory(*action_->getGoTo());
                } else {
                    actionState_ = FSM;
                }
            }
            break;
        case RETRACT:
            if (robot_.locomotion.isGoalReached()) {
                if (action_->getGoTo() != nullptr) {
                    actionState_ = GO_TO;
                    robot_.locomotion.followTrajectory(*action_->getGoTo());
                } else {
                    actionState_ = FSM;
                }
            }
            break;

        default:
            break;
    }
}
}  // namespace rd
