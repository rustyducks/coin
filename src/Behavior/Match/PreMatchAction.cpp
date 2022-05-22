#include "Coin/Behavior/Match/PreMatchAction.h"

namespace rd {
PreMatchAction::PreMatchAction(const PointOriented& yellowStart, const PointOriented& purpleStart)
    : Action("Prematch", nullptr, nullptr), state_(IDLE), isYellowSelected_(false), yellowStart_(yellowStart), purpleStart_(purpleStart), isInit_(false) {}

void PreMatchAction::deinit(Robot&) {}

bool PreMatchAction::isDeinit(Robot&) { return true; }

ActionPtr PreMatchAction::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            robot.hmi.setScoreDisplay(0);
            robot.stackManager.sendHome(0);
            state_ = INITIALIZING_ARM1;
            break;
        case INITIALIZING_ARM1:
            if (robot.stackManager.getStatus() == ProcedureInput::SUCCESS) {
                robot.stackManager.sendHome(1);
                state_ = INITIALIZING_ARM2;
            }
            break;
        case INITIALIZING_ARM2:
            if (robot.stackManager.getStatus() == ProcedureInput::SUCCESS) {
                robot.hmi.setLed(HMI::eLedColor::YELLOW);
                robot.locomotion.forceRobotPose(yellowStart_);
                isYellowSelected_ = true;
                state_ = YELLOW_SELECTED;
            }
            break;
        case YELLOW_SELECTED:
            if (robot.hmi.color()) {
                std::cout << robot.hmi.color() << std::endl;
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
        return eIntegrityCheck::ERROR;
    }
    return assembleIntegrities(onStartYellow_->checkIntegrity(), onStartPurple_->checkIntegrity());
}
}  // namespace rd
