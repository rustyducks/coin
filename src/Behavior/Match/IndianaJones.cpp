#include "Coin/Behavior/Match/IndianaJones.h"

namespace rd {

IndianaJones::IndianaJones(const std::string& name, const PointOriented entryPoint, PointOriented takeStatuettePoint, PointOriented dropReplicaPoint,
                           const PointOriented exitPoint)
    : Action(name, std::make_shared<PointOriented>(entryPoint), std::make_shared<PointOriented>(exitPoint)),
      state_(IDLE),
      takeStatuettePoint_(takeStatuettePoint),
      dropReplicaPoint_(dropReplicaPoint),
      catchStatuetteWait_(0.75),
      dropReplicaWait_(0.5) {}

ActionPtr IndianaJones::run(Robot& robot) {
    switch (state_) {
        case IDLE:
            robot.locomotion.goToPointHolonomic(takeStatuettePoint_);
            state_ = APPROACHING_STATUETTE;
            break;
        case APPROACHING_STATUETTE:
            if (robot.locomotion.isGoalReached()) {
                robot.statuetteArm.magnet(true);
                robot.statuetteArm.deployArm();
                catchStatuetteWait_.start();
                state_ = CATCHING_STATUETTE;
            }
            break;
        case CATCHING_STATUETTE:
            if (catchStatuetteWait_.check()) {
                robot.locomotion.goToPointHolonomic(dropReplicaPoint_);
                robot.statuetteArm.setStatuetteCatched();
                robot.table.isStatuetteOnPedestal = false;
                state_ = APPROACHING_REPLICA_DROP;
            }
            break;
        case APPROACHING_REPLICA_DROP:
            if (robot.locomotion.isGoalReached()) {
                robot.replicaHolder.release();
                dropReplicaWait_.start();
                state_ = DROPPING;
            }
            break;
        case DROPPING:
            if (dropReplicaWait_.check()) {
                robot.table.isReplicaOnPedestal = true;
                state_ = DONE;
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
}
void IndianaJones::deinit(Robot& robot) {
    if (robot.statuetteArm.hasStatuette()) {
        robot.statuetteArm.deployArm();
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
