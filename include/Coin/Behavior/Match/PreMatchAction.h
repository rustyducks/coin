#ifndef PREMATCHACTION_H
#define PREMATCHACTION_H

#include "Coin/Behavior/Match/Action.h"

namespace rd {

class PreMatchAction : public Action {
   public:
    PreMatchAction();
    virtual ActionPtr run(Robot& robot) override;
    virtual void deinit(Robot& robot) override;
    virtual bool isDeinit(Robot& robot) override;

    void setOnStartYellow(ActionPtr onStartYellow) { onStartYellow_ = onStartYellow; }
    void setOnStartPurple(ActionPtr onStartPurple) { onStartPurple_ = onStartPurple; }

    eIntegrityCheck checkIntegrity() override;

   protected:
    enum eState { IDLE, PURPLE_SELECTED, YELLOW_SELECTED, DONE };
    eState state_;
    bool isYellowSelected_;
    ActionPtr onStartYellow_;
    ActionPtr onStartPurple_;
};
}  // namespace rd

#endif /* PREMATCHACTION_H */
