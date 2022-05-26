#ifndef GOTODIFFACTION_H
#define GOTODIFFACTION_H

#include "Coin/Behavior/Match/Action.h"
namespace rd {

class WaitForMatchTimeAction : public Action {
   public:
    WaitForMatchTimeAction(const std::string& name, double matchTime) : Action(name, nullptr, nullptr), matchTime_(matchTime) {}

    virtual ActionPtr run(Robot& robot) {
        if (robot.secondsSinceMatchStart() >= matchTime_) {
            std::cout << name_ << "Ouah, the match is already started for more than " << matchTime_ << "seconds! Let's do the rest of the behavior!"
                      << std::endl;
            return onTime_;
        }
        return nullptr;
    };
    virtual void deinit(Robot&) {}
    virtual bool isDeinit(Robot&) { return true; }

    void setOnTime(ActionPtr onTime) { onTime_ = onTime; }

    virtual Action::eIntegrityCheck checkIntegrity() {
        if (onTime_ == nullptr) {
            logIntegrity(ERROR, "No on time defined.");
            return ERROR;
        }
        return onTime_->checkIntegrity();
    };

   protected:
    ActionPtr onTime_;
    double matchTime_;
};
}  // namespace rd

#endif /* GOTODIFFACTION_H */
