#ifndef GOTODIFFACTION_H
#define GOTODIFFACTION_H

#include "Coin/Behavior/Match/Action.h"
namespace rd {

struct sGoToDiffTraj {
    PointOriented pt;
    bool backwards;
    bool withFirstRotation;
};
class GoToDiffAction : public Action {
   public:
    GoToDiffAction(const std::string& name, sGoToDiffTraj goTo) : Action(name, nullptr, nullptr), traj_(goTo) {}

    virtual ActionPtr run(Robot&) { return onSuccess_; };
    virtual void deinit(Robot&) {}
    virtual bool isDeinit(Robot&) { return true; }

    void setOnSuccess(ActionPtr onSuccess) { onSuccess_ = onSuccess; }

    virtual Action::eIntegrityCheck checkIntegrity() {
        if (onSuccess_ == nullptr) {
            logIntegrity(ERROR, "No on success defined.");
            return ERROR;
        }
        return onSuccess_->checkIntegrity();
    };

    virtual bool goTo(const PointOriented&, Robot& robot) override {
        robot.locomotion.goToPointDiff(traj_.pt, traj_.backwards, traj_.withFirstRotation);
        return true;
    }

   protected:
    ActionPtr onSuccess_;
    sGoToDiffTraj traj_;
};
}  // namespace rd

#endif /* GOTODIFFACTION_H */
