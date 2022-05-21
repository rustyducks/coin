#ifndef STATUETTEARM_H
#define STATUETTEARM_H

#include "Coin/Actuator/ActuatorBase.h"

namespace rd {
class StatuetteArm : public ActuatorBase<ProcedureInput> {
   public:
    enum eArmState { RETRACTED = 0, HALF_DEPLOYED = 1, DEPLOYED = 2 };
    enum eMagnetState { OFF = 0, ON = 1 };

    StatuetteArm(ProcedureCommandSenderInterface& procedureSender);
    virtual void updateState(const ProcedureInput& input) override;

    eArmState armState() { return armState_; }
    eMagnetState magnetState() { return magnetState_; }

    void retractArm();
    void halfDeployArm();
    void deployArm();

    void magnet(bool on);

   protected:
    struct sCommand {
        eArmState armState;
        eMagnetState magnetState;
    };

    eArmState armState_;
    eMagnetState magnetState_;
    sCommand lastCommand_;

    void sendCommand(sCommand command);

    ProcedureCommandSenderInterface& procedureSender_;
};
}  // namespace rd

#endif /* STATUETTEARM_H */
