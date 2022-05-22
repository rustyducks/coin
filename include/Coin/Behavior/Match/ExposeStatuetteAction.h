#ifndef EXPOSESTATUETTEACTION_H
#define EXPOSESTATUETTEACTION_H

#include "Coin/Behavior/Match/Action.h"

namespace rd {
class ExposeStatuetteAction : public Action {
   public:
    ExposeStatuetteAction(const std::string& name, PointOriented entryPoint, PointOriented exposePoint, PointOriented exitPoint);

    virtual ActionPtr run(Robot& robot);
    virtual void deinit(Robot& robot);
    virtual bool isDeinit(Robot&) { return true; }

    virtual Action::eIntegrityCheck checkIntegrity();

    void setOnSuccess(ActionPtr onSuccessAction) { onSuccess_ = onSuccessAction; }
    const ActionPtr onSuccess() const { return onSuccess_; }

   protected:
    enum eState { IDLE, APPROCHING, RETREATING, DONE };
    eState state_;
    ActionPtr onSuccess_;
    PointOriented exposePoint_;
    PointOriented exitPoint_;
};
}  // namespace rd

#endif /* EXPOSESTATUETTEACTION_H */
