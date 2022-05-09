#include "Coin/Actuator/StackManager.h"

namespace rd {
StackManager::StackManager(Arm& arm1, Arm& arm2, Hat& hat, ProcedureCommandSenderInterface& procedureSender)
    : arm1_(arm1), arm2_(arm2), hat_(hat), procedureSender_(procedureSender), lastProcedure_(eProcedure::None), status_(rd::ProcedureInput::SUCCESS) {}

void StackManager::updateState(const ProcedureInput& input) { status_ = input.status_; }  // TODO: Add stack representation and hexa put on stack or removed
void StackManager::sendHome(const int armid) {
    if (armid > 1) {
        return;
    }
    procedureSender_.sendProcedureCommand(armid, protoduck::Procedure_Proc::Procedure_Proc_HOME, 0);
    lastProcedure_ = eProcedure::Home;
    status_ = ProcedureInput::RUNNING;
}
void StackManager::sendPutOnStack(const int armid, const int height) {
    if (armid > 1) {
        return;
    }
    procedureSender_.sendProcedureCommand(armid, protoduck::Procedure_Proc::Procedure_Proc_PUT_ON_STACK, height);
    lastProcedure_ = eProcedure::PutOnStack;
    status_ = ProcedureInput::RUNNING;
}
void StackManager::sendTurnAndPutOnStack(const int armid, const int height) {
    if (armid > 1) {
        return;
    }
    procedureSender_.sendProcedureCommand(armid, protoduck::Procedure_Proc::Procedure_Proc_TURN_AND_PUT_ON_STACK, height);
    lastProcedure_ = eProcedure::TurnAndPutOnStack;
    status_ = ProcedureInput::RUNNING;
}

void StackManager::sendTakeFromStack(const int armid, const int height) {
    if (armid > 1) {
        return;
    }
    procedureSender_.sendProcedureCommand(armid, protoduck::Procedure_Proc::Procedure_Proc_TAKE_FROM_STACK, height);
    lastProcedure_ = eProcedure::TakeFromStack;
    status_ = ProcedureInput::RUNNING;
}
}  // namespace rd
