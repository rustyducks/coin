#ifndef GOTODIFFACTION_H
#define GOTODIFFACTION_H

#include "Coin/Behavior/Match/Action.h"
#include "Coin/Behavior/Timer.h"
#include "Coin/Sensors/VL6180.h"
namespace rd {

class AlignDiffAction : public Action {
   public:
    AlignDiffAction(const std::string& name, PointOriented entryPoint, double distance, bool inverted, VL6180& sensor, double timeout)
        : Action(name, std::make_shared<PointOriented>(entryPoint), nullptr),
          state_(IDLE),
          alignDistance_(distance),
          inverted_(inverted),
          sensor_(sensor),
          timeout_(timeout) {}

    virtual ActionPtr run(Robot& robot) {
        switch (state_) {
            case IDLE:
                robot.locomotion.align(alignDistance_, &sensor_, inverted_);
                timeout_.start();
                state_ = ALIGNING;
                break;

            case ALIGNING:
                if (robot.locomotion.isGoalReached()) {
                    state_ = DONE;
                    return onSuccess_;
                }
                if (timeout_.isStarted() && timeout_.check()) {
                    state_ = DONE;
                    std::cout << "Can't align, timout, but we keep going boiz." << std::endl;
                    return onSuccess_;
                }

                break;
            case DONE:
                return onSuccess_;
                break;

            default:
                break;
        }
        return nullptr;
    };
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

   protected:
    enum eState { IDLE, ALIGNING, DONE };
    eState state_;
    ActionPtr onSuccess_;
    double alignDistance_;
    bool inverted_;

    VL6180& sensor_;

    Timer timeout_;
};
}  // namespace rd

#endif /* GOTODIFFACTION_H */
