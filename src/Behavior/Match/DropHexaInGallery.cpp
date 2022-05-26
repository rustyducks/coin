#include "Coin/Behavior/Match/DropHexaInGallery.h"

namespace rd {
DropHexaInGallery::DropHexaInGallery(const sDropHexaTraj goTo, const sDropHexaTraj retract, const Arm::ArmID armId, GalleryPtr gallery)
    : Action("Drop_in_gallery_" + std::to_string((int)gallery->sideColor()) + std::to_string((int)gallery->galleryColor()), nullptr, nullptr),
      state_(IDLE),
      armId_(armId),
      gallery_(gallery),
      goTo_(goTo),
      retract_(retract),
      onSuccess_(nullptr),
      halfUnstackingTimeout_(6.) {}

ActionPtr DropHexaInGallery::run(Robot& robot) {
    Arm& arm = armId_ == Arm::ARM_1 ? robot.arm1 : robot.arm2;
    switch (state_) {
        case IDLE:
            robot.stackManager.sendHalfTakeFromStack(
                armId_, robot.stackManager.getStackHeight() + robot.stackManager.SUCCION_TO_STACK_OFFSET - robot.stackManager.BASE_STACK_TO_ARM_HOME_HEIGHT);
            halfUnstackingTimeout_.start();
            state_ = HALF_UNSTACKING;
            break;
        case HALF_UNSTACKING:
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS) {
                robot.locomotion.goToPointDiff(goTo_.pt, goTo_.backwards, goTo_.withFirstRotation);
                state_ = APPROACHING;
            }
            if (robot.stackManager.getStatus() == ProcedureInput::eStatus::FAILURE || (halfUnstackingTimeout_.isStarted() && halfUnstackingTimeout_.check())) {
                if (onFailure_ != nullptr) {
                    std::cout << "[DropHexaInGallery] Failure, pop from stack and returns onFailure..." << std::endl;
                    robot.stackManager.popHexaFromStack();
                    return onFailure_;
                } else {
                    std::cout << "[DropHexaInGallery] Failure, but no onFailure set..." << std::endl;
                    state_ = APPROACHING;
                }
            }
            break;

        case APPROACHING:
            if (robot.locomotion.isGoalReached()) {
                arm.startPump(false);
                arm.openValve(true);
                state_ = RELEASING;
                gallery_->addHexa(robot.stackManager.getTopHexa());
                robot.stackManager.popHexaFromStack();
            }
            break;
        case RELEASING:
            if (arm.pressure() > -50) {
                return onSuccess_;
            }
            break;

        default:
            break;
    }
    return nullptr;
}

void DropHexaInGallery::deinit(Robot& robot) {
    Arm& arm = armId_ == Arm::ARM_1 ? robot.arm1 : robot.arm2;
    arm.openValve(false);
    arm.startPump(false);
    // robot.stackManager.sendHome(armId_);
}
bool DropHexaInGallery::isDeinit(Robot& robot) { return robot.stackManager.getStatus() == ProcedureInput::eStatus::SUCCESS; }

Action::eIntegrityCheck DropHexaInGallery::checkIntegrity() {
    if (onSuccess_ == nullptr) {
        logIntegrity(ERROR, "No onsuccess action defined.");
        return Action::eIntegrityCheck::ERROR;
    }
    if (gallery_ == nullptr) {
        logIntegrity(ERROR, "Undefined gallery.");
    }
    std::cout << "DropGallery Ok." << std::endl;
    return onSuccess_->checkIntegrity();
}

bool DropHexaInGallery::goTo(const PointOriented&, Robot&) {
    // robot.locomotion.goToPointDiff(goTo_.pt, goTo_.backwards, goTo_.withFirstRotation);
    return false;
}
bool DropHexaInGallery::retract(const PointOriented&, Robot& robot) {
    robot.locomotion.goToPointDiff(retract_.pt, retract_.backwards, retract_.withFirstRotation);
    return true;
}
}  // namespace rd
