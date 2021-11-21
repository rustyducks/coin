#include "Coin/Robot.h"

#include "Navigation/PurePursuitControl.h"

namespace rd {
Robot::Robot(const PositionControlParameters& posControlParams) : positionControl_(std::make_unique<PurePursuitControl>(posControlParams, 3, 50.)) {}

void Robot::controlPosition() {
    if (positionControlType_ == ePositionControlType::IDLE) {
    }
}
}  // namespace rd