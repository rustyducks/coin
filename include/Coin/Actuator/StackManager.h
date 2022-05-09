#ifndef STACKMANAGER_H
#define STACKMANAGER_H

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Communication/CommunicationBase.h"

namespace rd {
class StackManager : public ActuatorBase<ProcedureInput> {
   public:
    StackManager(Arm& arm1, Arm& arm2, Hat& hat, ProcedureCommandSenderInterface& procedureSender);

    virtual void updateState(const ProcedureInput& input) override;
    void sendHome(const int armid);
    void sendPutOnStack(const int armid, const int height);
    void sendTurnAndPutOnStack(const int armid, const int height);
    void sendTakeFromStack(const int armid, const int height);

   protected:
    enum eProcedure {
        None,
        Home,
        PutOnStack,
        TurnAndPutOnStack,
        TakeFromStack,
    };
    Arm& arm1_;
    Arm& arm2_;
    Hat& hat_;
    ProcedureCommandSenderInterface& procedureSender_;
    eProcedure lastProcedure_;
    ProcedureInput::eStatus status_;
};
}  // namespace rd

#endif /* STACKMANAGER_H */
