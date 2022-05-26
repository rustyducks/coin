#ifndef HEXAACTION_H
#define HEXAACTION_H

#include "Coin/Behavior/Match/Action.h"

namespace rd {

struct sTakeHexaTraj {
    PointOriented pt;
    bool backwards;
    bool withFirstRotation;
};
class TakeHexaAction : public Action {
   public:
    TakeHexaAction(const sTakeHexaTraj goTo, const sTakeHexaTraj retract, const Arm::ArmID armId, HexaPtr hexa);
    virtual ActionPtr run(Robot& robot) override;
    virtual void deinit(Robot& robot) override;
    virtual bool isDeinit(Robot& robot) override;

    void setOnSuccess(ActionPtr onSuccess) { onSuccess_ = onSuccess; }
    void setOnVacuumError(ActionPtr onUnableToVacuum) { onUnableToVacuum_ = onUnableToVacuum; }

    virtual bool goTo(const PointOriented& robotPose, Robot& robot) override;
    virtual bool retract(const PointOriented& robotPose, Robot& robot) override;

    eIntegrityCheck checkIntegrity() override;

   protected:
    enum eState { IDLE, TAKING, VACUUM_ERROR, RETRACTING, RETRACTING_VACCUM_ERROR, DONE };
    eState state_;
    Arm::ArmID armId_;
    HexaPtr hexa_;
    ActionPtr onSuccess_;
    ActionPtr onUnableToVacuum_;
    sTakeHexaTraj goTo_;
    sTakeHexaTraj retract_;
};
}  // namespace rd

#endif /* HEXAACTION_H */
