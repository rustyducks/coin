#include "Coin/Behavior/Match/PreMatchAction.h"

namespace rd {
PreMatchAction::PreMatchAction(const PointOriented& yellowStart, const PointOriented& purpleStart)
    : Action("Prematch", nullptr, nullptr), state_(IDLE), isYellowSelected_(false), yellowStart_(yellowStart), purpleStart_(purpleStart), isInit_(false) {}

void PreMatchAction::deinit(Robot& robot) {
    if (isYellowSelected_) {
        robot.color = Robot::YELLOW;
    } else {
        robot.color = Robot::PURPLE;
    }
}

bool PreMatchAction::isDeinit(Robot&) { return true; }

ActionPtr PreMatchAction::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            init(robot);
            state_ = INITIALIZING;
            break;
        case INITIALIZING:
            if (isInit_) {
                robot.hmi.setLed(HMI::eLedColor::YELLOW);
                robot.locomotion.forceRobotPose(yellowStart_);
                isYellowSelected_ = true;
                state_ = YELLOW_SELECTED;
            } else {
                init(robot);  // Update state... Meh..
            }
            break;
        case YELLOW_SELECTED:
            if (robot.hmi.color()) {
                robot.hmi.setLed(HMI::eLedColor::MAGENTA);
                isYellowSelected_ = false;
                robot.locomotion.forceRobotPose(purpleStart_);
                state_ = PURPLE_SELECTED;
            }
            if (!robot.hmi.tirette()) {
                state_ = DONE;
                if (isYellowSelected_) {
                    return onStartYellow_;
                } else {
                    return onStartPurple_;
                }
            }
            break;
        case PURPLE_SELECTED:
            if (!robot.hmi.color()) {
                robot.hmi.setLed(HMI::eLedColor::YELLOW);
                isYellowSelected_ = true;
                robot.locomotion.forceRobotPose(yellowStart_);
                state_ = YELLOW_SELECTED;
            }
            if (!robot.hmi.tirette()) {
                state_ = DONE;
                if (isYellowSelected_) {
                    return onStartYellow_;
                } else {
                    return onStartPurple_;
                }
            }
            break;
        case DONE:
            if (isYellowSelected_) {
                return onStartYellow_;
            } else {
                return onStartPurple_;
            }
            break;
        default:
            break;
    }
    return nullptr;
}

Action::eIntegrityCheck PreMatchAction::checkIntegrity() {
    if (onStartYellow_ == nullptr || onStartPurple_ == nullptr) {
        logIntegrity(ERROR, "No on start Yellow or on Start Purple defined...");
        return eIntegrityCheck::ERROR;
    }
    return assembleIntegrities(onStartYellow_->checkIntegrity(), onStartPurple_->checkIntegrity());
}
void PreMatchAction::init(Robot& robot) {
    if (!isInit_) {
        robot.hmi.setScoreDisplay(0);
        if (!robot.holonomic()) {
            if (robot.stackManager.initArms()) {
                isInit_ = true;
            }
        } else {
            robot.replicaHolder.hold();
            robot.statuetteArm.retractArm();
            robot.statuetteArm.magnet(false);
            robot.finger.retractFinger();
            isInit_ = true;
        }
    }
}

}  // namespace rd
