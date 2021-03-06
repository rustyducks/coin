#include "Coin/Behavior/Match/ExcavationAction.h"
namespace rd {
ExcavationAction::ExcavationAction(ExcavationSquarePtr square, double offset)
    : Action("Excavation of " + std::to_string(square->id()), std::make_shared<PointOriented>(square->location() + Point({offset, 150.}), -150. * M_PI / 180.),
             std::make_shared<PointOriented>(square->location() + Point({offset, 150.}), -150. * M_PI / 180.)),
      state_(IDLE),
      square_(square),
      colorDetectionTimeout_(3.),
      fingerDeployWait_(0.5),
      offset_(offset) {
    blockedDuration_ = 5.;
}
ActionPtr ExcavationAction::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            robot.locomotion.goToPointHolonomic(PointOriented(square_->location() + Point(offset_, 60.), -150. * M_PI / 180.));
            state_ = APPROACHING;
            break;
        case APPROACHING:
            if (robot.locomotion.isGoalReached()) {
                if (!colorDetectionTimeout_.isStarted()) {
                    colorDetectionTimeout_.start();
                }
                robot.locomotion.forceRobotPose({robot.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                if (!square_->knownColor()) {
                    if (robot.finger.isTouching() == robot.finger.PURPLE) {
                        square_->setColor(ExcavationSquare::PURPLE);
                        robot.table.reasonExcavationSquareColors();
                    } else if (robot.finger.isTouching() == robot.finger.YELLOW) {
                        square_->setColor(ExcavationSquare::YELLOW);
                        robot.table.reasonExcavationSquareColors();
                    } else if (robot.finger.isTouching() == robot.finger.RED) {
                        square_->setColor(ExcavationSquare::RED);
                        robot.table.reasonExcavationSquareColors();
                    }
                }
                if (square_->knownColor()) {
                    ExcavationSquare::eColor interstingColor = robot.color == Robot::YELLOW ? ExcavationSquare::YELLOW : ExcavationSquare::PURPLE;
                    if (square_->possibleColors().count(interstingColor) > 0) {
                        robot.finger.deployFinger();
                        fingerDeployWait_.start();
                        square_->flip();
                    }
                    state_ = RETURNING;
                } else if (colorDetectionTimeout_.check()) {
                    // Timeout...
                    std::cout << "[Excavation Action] Cannot determine the color of square: " << square_->id() << "." << std::endl;
                    state_ = RETURNING;
                }
            }
            break;
        case RETURNING: {
            if (!fingerDeployWait_.isStarted() || (fingerDeployWait_.isStarted() && fingerDeployWait_.check())) {
                const auto& nextSquareAct = getNextSquare(robot.color);
                if (nextSquareAct != nullptr) {
                    return nextSquareAct;
                } else {
                    return onSuccess_;
                }
                state_ = DONE;
            }
        } break;
        case DONE: {
            const auto& nextSquareAct = getNextSquare(robot.color);
            if (nextSquareAct != nullptr) {
                return nextSquareAct;
            } else {
                return onSuccess_;
            }
        } break;
    }
    return nullptr;
}
void ExcavationAction::deinit(Robot& robot) { robot.finger.retractFinger(); }
bool ExcavationAction::isDeinit(Robot&) { return true; }
ActionPtr ExcavationAction::onGoToBlocked(Robot& robot) {
    const auto& nextSquareAct = getNextSquare(robot.color);
    if (nextSquareAct != nullptr) {
        return nextSquareAct;
    } else {
        return onSuccess_;
    }
}

ActionPtr ExcavationAction::getNextSquare(Robot::eColor color) const {
    ExcavationSquare::eColor interstingColor = color == Robot::YELLOW ? ExcavationSquare::YELLOW : ExcavationSquare::PURPLE;
    for (const auto& squareAct : nextSquares_) {
        if (squareAct->square()->possibleColors().count(interstingColor) > 0 && !squareAct->square()->isFlipped()) {
            std::cout << "Going to " << squareAct->name() << std::endl;
            return squareAct;
        }
    }
    return nullptr;
}

Action::eIntegrityCheck ExcavationAction::checkIntegrity() {
    if (onSuccess_ == nullptr) {
        logIntegrity(ERROR, "On success action is nullptr");
        return eIntegrityCheck::ERROR;
    }
    if (square_ == nullptr) {
        logIntegrity(ERROR, "Square is nullptr");
    }
    eIntegrityCheck integrities = eIntegrityCheck::OK;
    for (const auto& sqact : nextSquares_) {
        assembleIntegrities(sqact->checkIntegrity(), integrities);
    }
    return integrities;
}
}  // namespace rd
