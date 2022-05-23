#ifndef GOTOACTION_H
#define GOTOACTION_H

#include "Coin/Behavior/Match/Action.h"
namespace rd {
class GoToAction : public Action {
   public:
    GoToAction(const std::string& name, PointOriented goTo) : Action(name, std::make_shared<PointOriented>(goTo), nullptr) {}

    virtual ActionPtr run(Robot&) { return onSuccess_; };
    virtual void deinit(Robot&) {}
    virtual bool isDeinit(Robot&) { return true; }

    void setOnSuccess(ActionPtr onSuccess) { onSuccess_ = onSuccess; }

    virtual Action::eIntegrityCheck checkIntegrity() {
        if (onSuccess_ == nullptr) {
            logIntegrity(ERROR, "No on success defined.");
            return ERROR;
        }
        if (goToPoint_ == nullptr) {
            logIntegrity(ERROR, "No point to go to defined.");
            return ERROR;
        }
        return onSuccess_->checkIntegrity();
    };

   protected:
    ActionPtr onSuccess_;
};
}  // namespace rd

#endif /* GOTOACTION_H */
