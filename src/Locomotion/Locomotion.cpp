#include "Coin/Locomotion/Locomotion.h"

#include "Navigation/PurePursuitControl.h"

namespace rd {
Locomotion::Locomotion(PositionControlParameters positionControlParameters, CommunicationOutputBase& communicationBase)
    : robotPose_(0., 0., 0.),
      robotSpeed_(0., 0., 0.),
      communicationBase_(communicationBase),
      adversaries_(),
      parameters_(positionControlParameters),
      positionControlType_(IDLE),
      positionControl_(positionControlParameters, 1.5, 150.),
      goToPointHolonomic_(positionControlParameters, 1.7),
      targetSpeed_(0., 0., 0.),
      lastCommand_(0., 0., 0.),
      robotBlocked_(false),
      robotBlockedSince_() {}

void Locomotion::followTrajectory(const Trajectory& traj) {
    positionControl_.setTrajectory(traj);
    positionControlType_ = POSITION_CONTROL;
}

void Locomotion::goToPointHolonomic(const PointOriented& pt) {
    goToPointHolonomic_.setTargetPoint(pt);
    positionControlType_ = GO_TO_HOLONOMIC;
}

Speed Locomotion::run(const double dt) {
    Speed outputSpeed = Speed(0., 0., 0.);
    switch (positionControlType_) {
        case IDLE:
            outputSpeed = Speed(0., 0., 0.);
            break;

        case POSITION_CONTROL: {
            outputSpeed = positionControl_.computeSpeed(robotPose_, lastCommand_, dt, 0.);
            if (positionControl_.isGoalReached()) {
                outputSpeed = Speed(0., 0., 0.);
                positionControlType_ = IDLE;
            }
        } break;

        case GO_TO_HOLONOMIC: {
            Speed computedSpeed = goToPointHolonomic_.computeSpeed(robotPose_, lastCommand_, dt, 1200.);
            if (goToPointHolonomic_.isGoalReached()) {
                outputSpeed = Speed(0., 0., 0.);
                positionControlType_ = IDLE;
            }
            double trajAngle = std::atan2(computedSpeed.vy(), computedSpeed.vx());
            double c = std::cos(trajAngle);
            double s = std::sin(trajAngle);
            const double tubeHalfWidth = 200;
            const double tubeInnerLength = 250.;
            const double tubeOuterLength = 400.;
            double correctedLinearSpeed = computedSpeed.linearSpeed();
            double minX = 99999999.;
            for (const auto& adv : adversaries_) {
                // Check for the closest adversary in a tube (-maxY, maxY) in front of the robot
                Point robot2adv = adv.second.transformIn(robotPose_);
                Point advInTrajFrame = Point(robot2adv.x() * c + robot2adv.y() * s, -robot2adv.x() * s + robot2adv.y() * c);
                if (std::abs(advInTrajFrame.y()) <= tubeHalfWidth) {
                    if (advInTrajFrame.x() > 0 && advInTrajFrame.x() < minX) {  // Forward
                        minX = advInTrajFrame.x();
                    }
                }
            }
            correctedLinearSpeed = correctedLinearSpeed * std::max(0., std::min(1., (minX - tubeInnerLength) / (tubeOuterLength - tubeInnerLength)));
            if (correctedLinearSpeed < computedSpeed.linearSpeed()) {
                goToPointHolonomic_.slowedDown();
            }
            if (correctedLinearSpeed <= parameters_.minLinearSpeed && std::abs(computedSpeed.vtheta()) < parameters_.minRotationalSpeed) {
                if (!robotBlocked_) {
                    robotBlocked_ = true;
                    robotBlockedSince_ = std::chrono::steady_clock::now();
                    std::cout << "[Locomotion] Robot is blocked" << std::endl;
                }
                outputSpeed = Speed(0., 0., 0.);
            } else {
                if (robotBlocked_) {
                    robotBlocked_ = false;
                    std::cout << "[Locomotion] Robot is free again" << std::endl;
                }
            }
            outputSpeed = Speed(correctedLinearSpeed * c, correctedLinearSpeed * s, computedSpeed.vtheta());
        } break;

        default:
            outputSpeed = Speed(0., 0., 0.);
            break;
    }
    lastCommand_ = outputSpeed;
    communicationBase_.sendSpeed(outputSpeed);
    return outputSpeed;
}

double Locomotion::robotBlockedDuration() const {
    if (!robotBlocked_) {
        return 0.;
    }
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - robotBlockedSince_).count() / 1000000.;
}

}  // namespace rd
