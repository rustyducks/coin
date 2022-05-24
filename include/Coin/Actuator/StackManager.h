#ifndef STACKMANAGER_H
#define STACKMANAGER_H

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Behavior/Timer.h"
#include "Coin/Communication/CommunicationBase.h"
#include "Coin/Table/Hexa.h"

namespace rd {
class StackManager : public ActuatorBase<ProcedureInput> {
   public:
    StackManager(Arm& arm1, Arm& arm2, Hat& hat, ProcedureCommandSenderInterface& procedureSender);

    virtual void updateState(const ProcedureInput& input) override;
    void sendHome(const int armid);
    void sendPutOnStack(const int armid, const int height);
    void sendTurnAndPutOnStack(const int armid, const int height);
    void sendTakeFromStack(const int armid, const int height);

    bool initArms();

    void pushHexaOnStack(const HexaPtr hexa) { inStack_.push_back(hexa); }
    void popHexaFromStack() {
        if (inStack_.size() == 0) {
            return;
        }
        inStack_.pop_back();
    }
    size_t getStackSize() { return inStack_.size(); }
    double getStackHeight() { return inStack_.size() * HEXA_HEIGHT; }

    ProcedureInput::eStatus getStatus() { return status_; }

    const double BASE_STACK_TO_ARM_HOME_HEIGHT = 150.;

   protected:
    const double HEXA_HEIGHT = 15.;
    enum eProcedure {
        None,
        Home,
        PutOnStack,
        TurnAndPutOnStack,
        TakeFromStack,
    };
    enum eArmInitState { IDLE, ARM1_INITIALIZING, ARM2_INITIALIZING, INITIALIZED };
    eArmInitState armInitState_;
    Arm& arm1_;
    Arm& arm2_;
    Hat& hat_;
    ProcedureCommandSenderInterface& procedureSender_;
    eProcedure lastProcedure_;
    ProcedureInput::eStatus status_;

    std::vector<HexaPtr> inStack_;

    Timer minProcedureDuration_;
};
}  // namespace rd

#endif /* STACKMANAGER_H */
