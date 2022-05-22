#include "Coin/Behavior/Match/IndianaJones.h"

namespace rd {

IndianaJones::IndianaJones(const std::string& name, const PointOriented entryPoint, PointOriented takeStatuettePoint, PointOriented dropReplicaPoint,
                           const PointOriented exitPoint)
    : Action(name, std::make_shared<PointOriented>(entryPoint), std::make_shared<PointOriented>(exitPoint)),
      state_(IDLE),
      takeStatuettePoint_(takeStatuettePoint),
      dropReplicaPoint_(dropReplicaPoint) {}

ActionPtr IndianaJones::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            robot.statuetteArm.halfDeployArm();
            robot.statuetteArm.magnet(true);
            robot.locomotion.goToPointHolonomic(takeStatuettePoint_);
            state_ = APPROACHING_STATUETTE;
            break;
        case APPROACHING_STATUETTE:
            if (robot.locomotion.isGoalReached()) {
                robot.statuetteArm.setStatuetteCatched();
                robot.locomotion.goToPointHolonomic(dropReplicaPoint_);
                state_ = APPROACHING_REPLICA_DROP;
            }
            break;
        case APPROACHING_REPLICA_DROP:
            if (robot.locomotion.isGoalReached()) {
                robot.replicaHolder.release();
                state_ = DROPPING;
            }
            break;
        case DROPPING:
            // Set small wait time
            state_ = DONE;
            return onSuccess_;
            break;
        case DONE:
            return onSuccess_;
            break;

        default:
            break;
    }
    return nullptr;
}
void IndianaJones::deinit(Robot& robot) {
    if (robot.statuetteArm.hasStatuette()) {
        robot.statuetteArm.halfDeployArm();
        robot.statuetteArm.magnet(true);
    } else {
        robot.statuetteArm.retractArm();
        robot.statuetteArm.magnet(false);
    }
    robot.replicaHolder.hold();
}
Action::eIntegrityCheck IndianaJones::checkIntegrity() {
    if (onSuccess_ == nullptr) {
        logIntegrity(ERROR, "No on success action defined");
        return ERROR;
    }
    return onSuccess_->checkIntegrity();
}
}  // namespace rd
