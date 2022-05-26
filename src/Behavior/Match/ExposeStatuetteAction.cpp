#include "Coin/Behavior/Match/ExposeStatuetteAction.h"

namespace rd {

ExposeStatuetteAction::ExposeStatuetteAction(const std::string& name, PointOriented entryPoint, PointOriented exposePoint, PointOriented exitPoint)
    : Action(name, std::make_shared<PointOriented>(entryPoint), nullptr), state_(IDLE), onSuccess_(nullptr), exposePoint_(exposePoint), exitPoint_(exitPoint) {}

ActionPtr ExposeStatuetteAction::run(Robot& robot) {
    switch (state_) {
        case IDLE: {
            robot.statuetteArm.deployArm();
            PointOriented corrected(robot.locomotion.robotPose().x(), exposePoint_.y(), exposePoint_.theta().value());
            robot.locomotion.goToPointHolonomic(corrected);
            state_ = APPROCHING;
        } break;
        case APPROCHING: {
            if (robot.locomotion.isGoalReached()) {
                robot.statuetteArm.magnet(false);
                PointOriented corrected(robot.locomotion.robotPose().x(), exitPoint_.y(), exitPoint_.theta().value());
                robot.locomotion.goToPointHolonomic(corrected);
                robot.table.isStatuetteInCabinet = true;
                robot.table.isCabinetActivated = true;
                state_ = RETREATING;
            }
        } break;
        case RETREATING:
            if (robot.locomotion.isGoalReached()) {
                robot.statuetteArm.retractArm();
                state_ = DONE;
                return onSuccess_;
            }
            break;
        case DONE:
            return onSuccess_;
            break;
        default:
            break;
    }
    return nullptr;
}

void ExposeStatuetteAction::deinit(Robot& robot) {
    if (robot.statuetteArm.hasStatuette()) {
        robot.statuetteArm.halfDeployArm();
        robot.statuetteArm.magnet(true);
    } else {
        robot.statuetteArm.retractArm();
        robot.statuetteArm.magnet(false);
    }
}

Action::eIntegrityCheck ExposeStatuetteAction::checkIntegrity() {
    if (onSuccess_ == nullptr) {
        logIntegrity(ERROR, "On success action is nullptr");
        return ERROR;
    }
    return onSuccess_->checkIntegrity();
}
}  // namespace rd
