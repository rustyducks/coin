#include "Coin/Behavior/Match/ActionJuggler.h"

namespace rd {
ActionJuggler::ActionJuggler(Robot& robot) : BehaviorBase(robot), actionState_(eActionState::GO_TO) {}

void ActionJuggler::tick() {
    // TODO: add guards for going to start zone at the end of match and overtime stop
    switch (actionState_) {
        case GO_TO:
            if (robot_.locomotion.robotBlockedDuration() > action_->blockedDuration()) {
                std::cout << "Robot is blocked for more than action blocked duration (" << action_->blockedDuration() << ")." << std::endl;
                nextAction_ = action_->onGoToBlocked();
                if (nextAction_ != nullptr) {
                    actionState_ = GO_TO;
                    action_ = nextAction_;
                    std::cout << "Running goTo of : " << action_->name() << std::endl;
                }
            } else if (robot_.locomotion.isGoalReached() || action_->getGoTo(robot_.locomotion.robotPose()) == nullptr) {
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
                PathPtr retractTraj = action_->getRetract(robot_.locomotion.robotPose());
                std::string lastname = action_->name();
                action_ = nextAction_;
                if (retractTraj != nullptr) {
                    actionState_ = RETRACT;
                    robot_.locomotion.followTrajectory((Path({robot_.locomotion.robotPose()}) + *retractTraj).computeSpeeds(500., 500., 500., 500.));
                    std::cout << "Running Retract of : " << lastname << std::endl;
                    for (size_t i = 0; i < (Path({robot_.locomotion.robotPose()}) + *retractTraj).size(); i++) {
                        std::cout << (Path({robot_.locomotion.robotPose()}) + *retractTraj).at(i);
                    }
                } else if (action_->getGoTo(robot_.locomotion.robotPose()) != nullptr) {
                    actionState_ = GO_TO;
                    auto fullPath =
                        (Path({robot_.locomotion.robotPose()}) + *action_->getGoTo(robot_.locomotion.robotPose())).computeSpeeds(500., 500., 500., 500.);
                    robot_.locomotion.followTrajectory(fullPath);
                    std::cout << "Running goTo of : " << action_->name() << std::endl;
                    std::cout << "FullPath len" << fullPath.size() << std::endl;
                    for (size_t i = 0; i < fullPath.size(); i++) {
                        std::cout << fullPath.at(i) << std::endl;
                    }
                } else {
                    actionState_ = FSM;
                    std::cout << "Running FSM of : " << action_->name() << std::endl;
                }
            }
            break;
        case RETRACT:
            if (robot_.locomotion.isGoalReached()) {
                if (action_->getGoTo(robot_.locomotion.robotPose()) != nullptr) {
                    actionState_ = GO_TO;
                    robot_.locomotion.followTrajectory(
                        (Path({robot_.locomotion.robotPose()}) + *action_->getGoTo(robot_.locomotion.robotPose())).computeSpeeds(500., 500., 500., 500.));
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
        return false;
    }
    action_ = firstAction;
    return true;
}

}  // namespace rd
