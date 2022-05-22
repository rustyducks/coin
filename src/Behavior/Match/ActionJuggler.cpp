#include "Coin/Behavior/Match/ActionJuggler.h"

namespace rd {
ActionJuggler::ActionJuggler(Robot& robot) : BehaviorBase(robot), actionState_(eActionState::GO_TO) {}

void ActionJuggler::tick() {
    // TODO: add guards for going to start zone at the end of match and overtime stop
    switch (actionState_) {
        case GO_TO:
            if (robot_.locomotion.robotBlockedDuration() > action_->blockedDuration()) {
                std::cout << "Robot is blocked for more than action blocked duration (" << action_->blockedDuration() << ")." << std::endl;
                nextAction_ = action_->onGoToBlocked(robot_);
                if (nextAction_ != nullptr) {
                    actionState_ = GO_TO;
                    action_ = nextAction_;
                    action_->goTo(robot_.locomotion.robotPose(), robot_);
                    std::cout << "Running goTo of : " << action_->name() << std::endl;
                }
            } else if (robot_.locomotion.isGoalReached() || !action_->goTo(robot_.locomotion.robotPose(), robot_)) {
                std::cout << "Running FSM : " << action_->name() << std::endl;
                actionState_ = FSM;
            }
            break;
        case FSM:
            nextAction_ = action_->run(robot_);
            if (nextAction_ != nullptr) {
                action_->deinit(robot_);
                actionState_ = DEINIT;
                std::cout << "Running Deinit of : " << action_->name() << std::endl;
            }
            break;
        case DEINIT:
            if (action_->isDeinit(robot_)) {
                if (action_->retract(robot_.locomotion.robotPose(), robot_)) {
                    actionState_ = RETRACT;
                    // robot_.locomotion.followTrajectory((Path({robot_.locomotion.robotPose()}) + *retractTraj).computeSpeeds(500., 500., 500., 500.));
                    std::cout << "Running Retract of : " << action_->name() << std::endl;
                } else {
                    action_ = nextAction_;
                    if (action_->goTo(robot_.locomotion.robotPose(), robot_)) {
                        actionState_ = GO_TO;
                        std::cout << "Running goTo of : " << action_->name() << std::endl;
                    } else {
                        actionState_ = FSM;
                        std::cout << "Running FSM of : " << action_->name() << std::endl;
                    }
                }
            }
            break;
        case RETRACT:
            if (robot_.locomotion.isGoalReached()) {
                if (action_->goTo(robot_.locomotion.robotPose(), robot_)) {
                    actionState_ = GO_TO;
                    std::cout << "Running Goto of : " << action_->name() << std::endl;
                } else {
                    actionState_ = FSM;
                    std::cout << "Running FSM of : " << action_->name() << std::endl;
                }
            }
            break;

        default:
            break;
    }
}

bool ActionJuggler::setFirstAction(ActionPtr firstAction) {
    if (firstAction->checkIntegrity() == Action::eIntegrityCheck::ERROR) {
        std::cout << "Check integrity failed..." << std::endl;
        return false;
    }
    action_ = firstAction;
    return true;
}

}  // namespace rd
