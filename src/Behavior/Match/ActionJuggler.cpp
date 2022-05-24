#include "Coin/Behavior/Match/ActionJuggler.h"

#include "Coin/Table/Colors.h"

namespace rd {
ActionJuggler::ActionJuggler(Robot& robot) : BehaviorBase(robot), actionState_(eActionState::GO_TO), almostEndStarted_(false) {}

void ActionJuggler::tick() {
    eColor color = robot_.color == robot_.YELLOW ? eColor::YELLOW : eColor::PURPLE;
    int score = robot_.table.countPoints(robot_.holonomic(), color, robot_.locomotion.robotPose());
    robot_.hmi.setScoreDisplay(score);
    if (robot_.matchStarted() && clock::now() >= robot_.endMatchTime()) {
        overtime_->run(robot_);
        return;
    }
    if (robot_.matchStarted()) {
        if (!almostEndStarted_) {
            clock::time_point toAlmostEndPoint;
            if (robot_.color == robot_.YELLOW) {
                toAlmostEndPoint = clock::now() + std::chrono::seconds((int)std::ceil(robot_.locomotion.robotPose().distanceTo(almostEndMatchYellow_) / 500.));
            } else {
                toAlmostEndPoint = clock::now() + std::chrono::seconds((int)std::ceil(robot_.locomotion.robotPose().distanceTo(almostEndMatchPurple_) / 500.));
            }
            if (toAlmostEndPoint >= robot_.almostEndMatchTime()) {
                action_->deinit(robot_);
                if (robot_.color == robot_.YELLOW) {
                    if (robot_.holonomic()) {
                        robot_.locomotion.goToPointHolonomic(almostEndMatchYellow_);
                    } else {
                        robot_.locomotion.goToPointDiff(almostEndMatchYellow_, false, true);
                    }
                } else {
                    if (robot_.holonomic()) {
                        robot_.locomotion.goToPointHolonomic(almostEndMatchPurple_);
                    } else {
                        robot_.locomotion.goToPointDiff(almostEndMatchPurple_, false, true);
                    }
                }
                std::cout << "[ActionJuggler] If we don't want to be like the white rabbit, we should got now! (almost end match)" << std::endl;
                almostEndStarted_ = true;
                return;
            }
        } else {
            if (robot_.locomotion.isGoalReached()) {
                overtime_->run(robot_);
            }
            return;
        }
    }
    std::cout << actionState_ << std::endl;
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
            } else if (robot_.locomotion.isGoalReached()) {
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
                    action_ = nextAction_;
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
    if (almostEndMatchYellow_ == PointOriented() || almostEndMatchPurple_ == PointOriented()) {
        std::cout << "No almost end point set in action juggler... integrity checks failed" << std::endl;
    }
    action_ = firstAction;
    if (action_->goTo(robot_.locomotion.robotPose(), robot_)) {
        actionState_ = GO_TO;
    } else {
        actionState_ = FSM;
    }

    return true;
}

}  // namespace rd
