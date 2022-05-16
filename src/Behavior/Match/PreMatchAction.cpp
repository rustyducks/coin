#include "Coin/Behavior/Match/PreMatchAction.h"

namespace rd {
PreMatchAction::PreMatchAction() : Action("Prematch", nullptr, nullptr), state_(IDLE) {}

void PreMatchAction::deinit(Robot&) {}

bool PreMatchAction::isDeinit(Robot&) { return true; }

ActionPtr PreMatchAction::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            robot.hmi.setScoreDisplay(0);
            robot.hmi.setLed(HMI::eLedColor::YELLOW);
            isYellowSelected_ = true;
            state_ = YELLOW_SELECTED;
            break;
        case YELLOW_SELECTED:
            if (robot.hmi.color()) {
                robot.hmi.setLed(HMI::eLedColor::MAGENTA);
                isYellowSelected_ = false;
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
