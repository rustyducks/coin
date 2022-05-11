#include "Coin/Locomotion/Locomotion.h"

#include "Navigation/PurePursuitControl.h"

namespace rd {
Locomotion::Locomotion(PositionControlParameters positionControlParameters, CommunicationOutputBase& communicationBase)
    : communicationBase_(communicationBase),
      parameters_(positionControlParameters),
      positionControlType_(IDLE),
      positionControl_(positionControlParameters, 2, 150.),
      targetSpeed_(0., 0., 0.) {}

void Locomotion::followTrajectory(const Trajectory& traj) {
    positionControl_.setTrajectory(traj);
    positionControlType_ = POSITION_CONTROL;
}

Speed Locomotion::run(const double dt) {
    Speed outputSpeed = Speed(0., 0., 0.);
    switch (positionControlType_) {
        case IDLE:
            outputSpeed = Speed(0., 0., 0.);
            break;

        case POSITION_CONTROL: {
            const double maxY = 200;
            const double outerX = 500;
            const double innerX = 250;
            double minX = 99999999.;
            for (const auto& adv : adversaries_) {
                // Check for the closest adversary in a tube (-maxY, maxY) in front of the robot
                Point robot2adv = adv.second.transformIn(robotPose_);
                if (std::abs(robot2adv.y()) <= maxY) {
                    if (robot2adv.x() < minX) {  // Forward
                        minX = robot2adv.x();
                    }
                }
            }
            // Scale the speed according to outerX and innerX (>outerX fullSpeed, <innerX full stop, percent factor between the two)
            double maxSpeedObstacles = parameters_.maxLinearSpeed * std::max(0., std::min(1., (minX - innerX) / (outerX - innerX)));
            outputSpeed = positionControl_.computeSpeed(robotPose_, robotSpeed_, dt, maxSpeedObstacles);
        } break;

        default:
            outputSpeed = Speed(0., 0., 0.);
            break;
    }
    communicationBase_.sendSpeed(outputSpeed);
    return outputSpeed;
}
}  // namespace rd
