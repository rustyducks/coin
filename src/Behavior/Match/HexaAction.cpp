#include "Coin/Behavior/Match/HexaAction.h"

namespace rd {
TakeHexaAction::TakeHexaAction(const sTakeHexaTraj goTo, const sTakeHexaTraj retract, Arm::ArmID armid, HexaPtr hexa)
    : Action("Take_" + hexa->name_, nullptr, nullptr), state_(TakeHexaAction::IDLE), armId_(armid), hexa_(hexa), goTo_(goTo), retract_(retract) {}

void TakeHexaAction::deinit(Robot&) {}  // robot.stackManager.sendHome(armId_); }

bool TakeHexaAction::isDeinit(Robot& robot) {
    if (robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS) {
        robot.stackManager.pushHexaOnStack(hexa_);
        hexa_->face_ = Hexa::eFace::TREASURE;
        return true;
    }
    if (robot.stackManager.getStatus() == ProcedureInput::eStatus::FAILURE || robot.stackManager.getStatus() == ProcedureInput::eStatus::POSITION_UNREACHABLE ||
        robot.stackManager.getStatus() == ProcedureInput::eStatus::UNABLE_VACUUM) {
        Arm& arm = armId_ == Arm::ARM_1 ? robot.arm1 : robot.arm2;
        arm.startPump(false);
        robot.hat.startPump(false);
        return true;
    }
    return false;
}

ActionPtr TakeHexaAction::run(Robot& robot) {
    switch (state_) {
        case TakeHexaAction::IDLE:
            if (hexa_->face_ == Hexa::eFace::ROCKS) {
                robot.stackManager.sendTurnAndPutOnStack(armId_, robot.stackManager.getStackHeight() + robot.stackManager.SUCCION_TO_STACK_OFFSET -
                                                                     robot.stackManager.BASE_STACK_TO_ARM_HOME_HEIGHT);
            } else {
                robot.stackManager.sendPutOnStack(armId_, robot.stackManager.getStackHeight() + robot.stackManager.SUCCION_TO_STACK_OFFSET -
                                                              robot.stackManager.BASE_STACK_TO_ARM_HOME_HEIGHT);
            }
            state_ = TakeHexaAction::TAKING;
            break;
        case TakeHexaAction::TAKING:
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::RUNNING_FREE || robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS) {
                robot.locomotion.goToPointDiff(retract_.pt, retract_.backwards, retract_.withFirstRotation);
                state_ = TakeHexaAction::RETRACTING;
            }
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::UNABLE_VACUUM ||
                robot.stackManager.getStatus() == ProcedureInput::eStatus::FAILURE) {
                robot.locomotion.goToPointDiff(retract_.pt, retract_.backwards, retract_.withFirstRotation);

                state_ = TakeHexaAction::eState::RETRACTING_VACCUM_ERROR;
                robot.locomotion.goToPointDiff(retract_.pt, retract_.backwards, retract_.withFirstRotation);
            }
            break;
        case TakeHexaAction::RETRACTING:
            if (robot.locomotion.isGoalReached()) {
                state_ = DONE;
                return onSuccess_;
            }
            break;
        case TakeHexaAction::RETRACTING_VACCUM_ERROR:
            if (robot.locomotion.isGoalReached()) {
                state_ = VACUUM_ERROR;
                if (onUnableToVacuum_ == nullptr) {
                    std::cout << "[" << name_ << "] Unable to vacuum, but no 'unable to vacuum' action defined, staying in this action" << std::endl;
                } else {
                    std::cout << "[" << name_ << "] Unable to vaccum, returning on unable to vacuum" << std::endl;
                    return onUnableToVacuum_;
                }
            }

        default:
            break;
    }
    return nullptr;
}

Action::eIntegrityCheck TakeHexaAction::checkIntegrity() {
    if (onSuccess_ == nullptr) {
        logIntegrity(ERROR, "No onsuccess action defined.");
        return Action::eIntegrityCheck::ERROR;
    }
    if (hexa_ == nullptr) {
        logIntegrity(ERROR, "Defined hexa is nullptr.");
        return Action::eIntegrityCheck::ERROR;
    }
    eIntegrityCheck onSuccessCheck = onSuccess_->checkIntegrity();
    eIntegrityCheck onUnableVacuumCheck;
    if (onUnableToVacuum_ == nullptr) {
        logIntegrity(WARNING, "No onUnableVacuum action defined.");
        onUnableVacuumCheck = Action::eIntegrityCheck::WARNING;
    } else {
        onUnableVacuumCheck = onUnableToVacuum_->checkIntegrity();
    }
    return assembleIntegrities(onSuccessCheck, onUnableVacuumCheck);
}
bool TakeHexaAction::goTo(const PointOriented&, Robot& robot) {
    robot.locomotion.goToPointDiff(goTo_.pt, goTo_.backwards, goTo_.withFirstRotation);
    return true;
}
bool TakeHexaAction::retract(const PointOriented&, Robot&) { return false; }

}  // namespace rd
