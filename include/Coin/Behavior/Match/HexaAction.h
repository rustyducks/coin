#ifndef HEXAACTION_H
#define HEXAACTION_H

#include "Coin/Behavior/Match/Action.h"
#include "Coin/Behavior/Match/HexaAction.h"

namespace rd {
class TakeHexaAction : public Action {
   public:
    TakeHexaAction(TrajectoryPtr goTo, TrajectoryPtr retract, const Arm::ArmID armId, HexaPtr hexa);
    virtual ActionPtr run(Robot& robot) override;
    virtual void deinit(Robot& robot) override;
    virtual bool isDeinit(Robot& robot) override;

    void setOnSuccess(ActionPtr onSuccess);
    void setOnVacuumError(ActionPtr onUnableToVacuum);

    eIntegrityCheck checkIntegrity() override;

   protected:
    enum eState { IDLE, TAKING, VACUUM_ERROR, DONE };
    eState state_;
    Arm::ArmID armId_;
    HexaPtr hexa_;
    ActionPtr onSuccess_;
    ActionPtr onUnableToVacuum_;
};
}  // namespace rd

#endif /* HEXAACTION_H */
