#ifndef PREMATCHACTION_H
#define PREMATCHACTION_H

#include "Coin/Behavior/Match/Action.h"

namespace rd {

class PreMatchAction : public Action {
   public:
    PreMatchAction(const PointOriented& yellowStartPoint, const PointOriented& purpleStartPoint);
    virtual ActionPtr run(Robot& robot) override;
    virtual void deinit(Robot& robot) override;
    virtual bool isDeinit(Robot& robot) override;

    void setOnStartYellow(ActionPtr onStartYellow) { onStartYellow_ = onStartYellow; }
    void setOnStartPurple(ActionPtr onStartPurple) { onStartPurple_ = onStartPurple; }
    void init(Robot& robot);

    eIntegrityCheck checkIntegrity() override;

   protected:
    enum eState { IDLE, INITIALIZING, PURPLE_SELECTED, YELLOW_SELECTED, DONE };
    eState state_;
    bool isYellowSelected_;
    ActionPtr onStartYellow_;
    ActionPtr onStartPurple_;
    PointOriented yellowStart_;
    PointOriented purpleStart_;
    bool isInit_;
};
class EndMatchAction : public Action {
   public:
    EndMatchAction() : Action("End Match", nullptr, nullptr) {}
    virtual ActionPtr run(Robot& robot) override {
        robot.locomotion.stop();
        return nullptr;
    }
    virtual void deinit(Robot&) override {}
    virtual bool isDeinit(Robot&) override { return false; }

    eIntegrityCheck checkIntegrity() override { return eIntegrityCheck::OK; }
};
}  // namespace rd

#endif /* PREMATCHACTION_H */
