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
    void setOnFailure(ActionPtr onFailure) { onFailure_ = onFailure; }

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
    ActionPtr onFailure_;
    Timer halfUnstackingTimeout_;
};

class GallerySwitcher : public Action {
   public:
    GallerySwitcher(const std::string& name) : Action(name, nullptr, nullptr) {}

    virtual ActionPtr run(Robot& robot) {
        if (robot.stackManager.getStackSize() == 0) {
            std::cout << "[Galery Switcher] Go Success !" << std::endl;
            return onSuccess_;
        } else if (robot.stackManager.getTopHexa()->color_ == Hexa::eColor::BLUE) {
            std::cout << "[Galery Switcher] Go Blue !" << std::endl;
            return onBlue_;
        } else if (robot.stackManager.getTopHexa()->color_ == Hexa::eColor::GREEN) {
            std::cout << "[Galery Switcher] Go Green !" << std::endl;

            return onGreen_;
        } else if (robot.stackManager.getTopHexa()->color_ == Hexa::eColor::RED) {
            std::cout << "[Galery Switcher] Go Red !" << std::endl;

            return onRed_;
        } else {
            std::cout << "[Gallery Switcher] Something went wrong, the stack is not empty but the color of hexa is not know... Returning on success..."
                      << std::endl;
            return onSuccess_;
        }
    }
    virtual void deinit(Robot&) {}
    virtual bool isDeinit(Robot&) { return true; }

    virtual Action::eIntegrityCheck checkIntegrity() {
        if (isIntegrityChecked_) {
            return OK;
        }
        isIntegrityChecked_ = true;
        if (onBlue_ == nullptr) {
            logIntegrity(ERROR, "No on Blue defined");
            return ERROR;
        }
        if (onGreen_ == nullptr) {
            logIntegrity(ERROR, "No on Green defined");
            return ERROR;
        }
        if (onRed_ == nullptr) {
            logIntegrity(ERROR, "No on red defined");
            return ERROR;
        }
        if (onSuccess_ == nullptr) {
            logIntegrity(ERROR, "No on success defined");
            return ERROR;
        }
        return OK;
    }
    void setOnBlue(ActionPtr onBlue) { onBlue_ = onBlue; }
    void setOnGreen(ActionPtr onGreen) { onGreen_ = onGreen; }
    void setOnRed(ActionPtr onRed) { onRed_ = onRed; }
    void setOnSuccess(ActionPtr onSuccess) { onSuccess_ = onSuccess; }

   protected:
    ActionPtr onBlue_;
    ActionPtr onGreen_;
    ActionPtr onRed_;
    ActionPtr onSuccess_;
};
}  // namespace rd

#endif /* DROPHEXAINGALLERY_H */
