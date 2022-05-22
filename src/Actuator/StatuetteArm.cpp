#include "Coin/Actuator/StatuetteArm.h"

namespace rd {
StatuetteArm::StatuetteArm(ProcedureCommandSenderInterface& procedureSender)
    : armState_(RETRACTED), magnetState_(OFF), lastCommand_({RETRACTED, OFF}), procedureSender_(procedureSender), hasStatuette_(false) {
    retractArm();
    magnet(false);
}

void StatuetteArm::updateState(const ProcedureInput&) {
    // No procedure status on crolonome :'(
}

void StatuetteArm::retractArm() {
    sCommand cmd({RETRACTED, lastCommand_.magnetState});
    sendCommand(cmd);
}
void StatuetteArm::halfDeployArm() {
    sCommand cmd({HALF_DEPLOYED, lastCommand_.magnetState});
    sendCommand(cmd);
}

void StatuetteArm::deployArm() {
    sCommand cmd({DEPLOYED, lastCommand_.magnetState});
    sendCommand(cmd);
}

void StatuetteArm::magnet(bool on) {
    sCommand cmd({lastCommand_.armState, on ? ON : OFF});
    sendCommand(cmd);
    if (!on) {
        hasStatuette_ = false;
    }
}

void StatuetteArm::sendCommand(sCommand cmd) {
    int param = cmd.magnetState << 8 | cmd.armState;
    procedureSender_.sendProcedureCommand(0, protoduck::Procedure_Proc::Procedure_Proc_TROMPE, param);
    lastCommand_ = cmd;
    magnetState_ = lastCommand_.magnetState;
    armState_ = lastCommand_.armState;
}

}  // namespace rd
