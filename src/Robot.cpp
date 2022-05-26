#include "Coin/Robot.h"

#include "Coin/Sensors/GPIOHandler.h"
#include "Navigation/PurePursuitControl.h"

namespace rd {

Robot::Robot(UDPDucklink& motorDucklink, UDPDucklink& ioDucklink, UDPDucklink& lidarDucklink, PositionControlParameters& positionControlParams, bool holonomic,
             Table& table)
    : locomotion(positionControlParams, motorDucklink),
      arm1(Arm::ArmID::ARM_1, ioDucklink),
      arm2(Arm::ArmID::ARM_2, ioDucklink),
      hat(ioDucklink),
      stackManager(arm1, arm2, hat, ioDucklink),
      hmi(ioDucklink),
      finger(ioDucklink),
      replicaHolder(ioDucklink),
      statuetteArm(ioDucklink),
      color(UNKNOWN),
      table(table),
      holonomic_(holonomic),
      motorDucklink_(motorDucklink),
      ioDucklink_(ioDucklink),
      lidarDucklink_(lidarDucklink),
      matchStarted_(false) {
    setupPin(17, rd::eInputOutput::OUT);
    setupPin(27, rd::eInputOutput::OUT);
    setupPin(22, rd::eInputOutput::OUT);

    // rd::digitalWrite(17, rd::eLowHigh::LOW);
    digitalWrite(27, rd::eLowHigh::LOW);
    digitalWrite(22, rd::eLowHigh::LOW);

    digitalWrite(17, rd::eLowHigh::HIGH);
    usleep(5000);
    borderSensorPurple.initialize(1, 0x29);
    borderSensorPurple.changeAddr(0x31);

    digitalWrite(27, rd::eLowHigh::HIGH);
    usleep(5000);
    borderSensorYellow.initialize(1, 0x29);
    borderSensorYellow.changeAddr(0x30);

    digitalWrite(22, rd::eLowHigh::HIGH);
    excavationSensor.initialize(1, 0x29);
}

void Robot::sense() {
    for (const auto& input : motorDucklink_.getInputs()) {
        if (input->type() == rd::eInput::POSITION_REPORT) {
            auto& msg = static_cast<rd::PointOrientedInputWithTimestamp&>(*input);
            locomotion.updateRobotPose(msg);
            lidarDucklink_.sendPoseReport(locomotion.robotPose());
        } else if (input->type() == rd::eInput::SPEED_REPORT) {
            auto& msg = static_cast<rd::SpeedInput&>(*input);
            locomotion.updateRobotSpeed(msg);
        }
    }
    for (const auto& input : ioDucklink_.getInputs()) {
        if (input->type() == rd::eInput::ARM_STATUS) {
            auto& msg = static_cast<rd::ArmInput&>(*input);
            if (msg.armId() == 0) {
                arm1.updateState(msg);
            } else {
                arm2.updateState(msg);
            }
        } else if (input->type() == rd::eInput::HAT_STATUS) {
            auto& msg = static_cast<rd::HatInput&>(*input);
            hat.updateState(msg);
        } else if (input->type() == rd::eInput::PROCEDURE_STATUS) {
            auto& msg = static_cast<rd::ProcedureInput&>(*input);
            stackManager.updateState(msg);
        } else if (input->type() == rd::eInput::HMI_REPORT) {
            auto& msg = static_cast<rd::HMIInput&>(*input);
            hmi.updateState(msg);
        } else if (input->type() == rd::eInput::RESISTOR_REPORT) {
            auto& msg = static_cast<rd::ResistorInput&>(*input);
            finger.updateState(msg);
        }
    }
    for (const auto& input : lidarDucklink_.getInputs()) {
        if (input->type() == rd::eInput::LIDAR_ADVERSARIES) {
            auto& msg = static_cast<rd::LidarAdversaries&>(*input);
            locomotion.updateAdversaries(msg);
        }
    }
    if (holonomic_) {
        if (color == eColor::PURPLE) {
            borderSensorPurple.sense();
        } else {
            borderSensorYellow.sense();
        }
    }
}

void Robot::startMatch() {
    if (!matchStarted_) {
        matchStarted_ = true;
        almostEndMatchTime_ = clock::now() + std::chrono::seconds(ALMOST_END_DURATION);
        endMatchTime_ = clock::now() + std::chrono::seconds(MATCH_DURATION);
        startMatchTime_ = clock::now();
    }
}

}  // namespace rd