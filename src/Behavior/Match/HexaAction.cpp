#include "Coin/Behavior/Match/HexaAction.h"

namespace rd {
TakeHexaAction::TakeHexaAction(rd::TrajectoryPtr goTo, rd::TrajectoryPtr retract, Arm::ArmID armid, HexaPtr hexa)
    : Action("Take_" + hexa_->name_, goTo, retract), state_(TakeHexaAction::IDLE), armId_(armid), hexa_(hexa) {}

void TakeHexaAction::deinit(Robot& robot) { robot.stackManager.sendHome(armId_); }

bool TakeHexaAction::isDeinit(Robot& robot) { return robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS; }

ActionPtr TakeHexaAction::run(Robot& robot) {
    switch (state_) {
        case TakeHexaAction::IDLE:
            if (hexa_->face_ == Hexa::eFace::ROCKS) {
                robot.stackManager.sendTurnAndPutOnStack(armId_, robot.stackManager.getStackHeight() - robot.stackManager.BASE_STACK_TO_ARM_HOME_HEIGHT);
            } else {
                robot.stackManager.sendPutOnStack(armId_, robot.stackManager.getStackHeight() - robot.stackManager.BASE_STACK_TO_ARM_HOME_HEIGHT);
            }
            state_ = TakeHexaAction::TAKING;
            break;
        case TakeHexaAction::TAKING:
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS) {
                robot.stackManager.pushHexaOnStack(hexa_);
                hexa_->face_ = Hexa::eFace::TREASURE;
                state_ = TakeHexaAction::DONE;
                return onSuccess_;
            }
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::UNABLE_VACUUM) {
                state_ = TakeHexaAction::eState::VACUUM_ERROR;
                if (onUnableToVacuum_ == nullptr) {
                    std::cout << "[" << name_ << "] Unable to vacuum, but no 'unable to vacuum' action defined, staying in this action" << std::endl;
                } else {
                    return onUnableToVacuum_;
                }
            }
            break;
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

}  // namespace rd
