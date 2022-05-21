#include "Coin/Actuator/ReplicaHolder.h"

namespace rd {
ReplicaHolder::ReplicaHolder(ProcedureCommandSenderInterface& procedureSender)
    : procedureSender_(procedureSender), state_(eState::HOLDING), hasReplica_(true), status_(ProcedureInput::eStatus::SUCCESS) {
    hold();
}

void ReplicaHolder::release() {
    procedureSender_.sendProcedureCommand(0, protoduck::Procedure_Proc::Procedure_Proc_DROPPER, 1);
    state_ = RELEASED;
    hasReplica_ = false;
}

void ReplicaHolder::hold() {
    procedureSender_.sendProcedureCommand(0, protoduck::Procedure_Proc::Procedure_Proc_DROPPER, 0);
    state_ = HOLDING;
}

void ReplicaHolder::updateState(const ProcedureInput&) {
    // No procedure status on Crolonome :'(
}

}  // namespace rd
