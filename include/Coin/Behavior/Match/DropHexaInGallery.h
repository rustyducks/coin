#ifndef DROPHEXAINGALLERY_H
#define DROPHEXAINGALLERY_H

#include "Coin/Behavior/Match/Action.h"
#include "Coin/Behavior/Timer.h"
#include "Coin/Table/Hexa.h"
#include "GeometryTools/Point.h"

namespace rd {

struct sDropHexaTraj {
    PointOriented pt;
    bool backwards;
    bool withFirstRotation;
};

class DropHexaInGallery : public Action {
   public:
    DropHexaInGallery(const sDropHexaTraj goTo, const sDropHexaTraj retract, const Arm::ArmID armId, GalleryPtr gallery);

    virtual ActionPtr run(Robot& robot);
    virtual void deinit(Robot& robot);
    virtual bool isDeinit(Robot& robot);

    virtual Action::eIntegrityCheck checkIntegrity();
    void setOnSuccess(ActionPtr onSuccess) { onSuccess_ = onSuccess; }

    virtual bool goTo(const PointOriented& robotPose, Robot& robot) override;
    virtual bool retract(const PointOriented& robotPose, Robot& robot) override;

   protected:
    enum eState { IDLE, HALF_UNSTACKING, APPROACHING, RELEASING };
    eState state_;
    Arm::ArmID armId_;
    GalleryPtr gallery_;
    sDropHexaTraj goTo_;
    sDropHexaTraj retract_;
    ActionPtr onSuccess_;
    Timer halfUnstackingTimeout_;
};
}  // namespace rd

#endif /* DROPHEXAINGALLERY_H */
