#ifndef INDIANAJONES_H
#define INDIANAJONES_H

#include "Coin/Behavior/Match/Action.h"

namespace rd {
class IndianaJones : public Action {
   public:
    IndianaJones(const std::string& name, const PointOriented entryPoint, PointOriented takeStatuettePoint, PointOriented dropReplicaPoint,
                 const PointOriented exitPoint);
    virtual ActionPtr run(Robot& robot);
    virtual void deinit(Robot& robot);
    virtual bool isDeinit(Robot&) { return true; }

    virtual Action::eIntegrityCheck checkIntegrity();

    void setOnSuccess(ActionPtr onSuccessAction) { onSuccess_ = onSuccessAction; }
    const ActionPtr onSuccess() const { return onSuccess_; }

   protected:
    enum eState { IDLE, APPROACHING_STATUETTE, APPROACHING_REPLICA_DROP, DROPPING, DONE };
    eState state_;
    PointOriented takeStatuettePoint_;
    PointOriented dropReplicaPoint_;
    ActionPtr onSuccess_;
};
}  // namespace rd

#endif /* INDIANAJONES_H */
