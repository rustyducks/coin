#ifndef ACTIONJUGGLER_H
#define ACTIONJUGGLER_H

#include "Action.h"
#include "Coin/Behavior/BehaviorBase.h"

namespace rd {
class ActionJuggler : public BehaviorBase {
   public:
    ActionJuggler(Robot& robot);
    virtual void tick() override;

    bool setFirstAction(ActionPtr action);

   protected:
    enum eActionState { GO_TO, FSM, DEINIT, RETRACT };
    eActionState actionState_;

    ActionPtr action_;
    ActionPtr nextAction_;  // To store next action while waiting for the current one to deinit.
};
}  // namespace rd

#endif /* ACTIONJUGGLER_H */
