#include "Coin/Behavior/Slave.h"

namespace rd {
Slave::Slave(Robot& robot) : BehaviorBase(robot) {}

void Slave::tick() {}

void Slave::newTargetOrientedPoint(const PointOriented& newPoint) {
    state_ = eSlaveState::POSITION_CONTROL;
    target_ = newPoint;
    // robot_.positionControl().setTargetPoint(target_);
}
}  // namespace rd