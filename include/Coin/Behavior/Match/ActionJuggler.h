#ifndef ACTIONJUGGLER_H
#define ACTIONJUGGLER_H

#include <chrono>

#include "Action.h"
#include "Coin/Behavior/BehaviorBase.h"

namespace rd {
class ActionJuggler : public BehaviorBase {
   public:
    ActionJuggler(Robot& robot);
    virtual void tick() override;

    void setAlmostEndMatchPoints(PointOriented yellow, PointOriented purple) {
        almostEndMatchYellow_ = yellow;
        almostEndMatchPurple_ = purple;
    }
    void setOvertimeAction(ActionPtr action) { overtime_ = action; }
    bool setFirstAction(ActionPtr action);

   protected:
    enum eActionState { GO_TO, FSM, DEINIT, RETRACT };
    eActionState actionState_;

    ActionPtr action_;
    ActionPtr nextAction_;  // To store next action while waiting for the current one to deinit.

    PointOriented almostEndMatchYellow_;
    PointOriented almostEndMatchPurple_;
    ActionPtr overtime_;
    using clock = std::chrono::system_clock;
    bool almostEndStarted_;
};
}  // namespace rd

#endif /* ACTIONJUGGLER_H */
