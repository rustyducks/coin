#include "Coin/Actuator/Arm.h"

namespace rd {
Arm::Arm(const ArmID armId, ArmCommandSenderInterface& sender)
    : lastCommand_({{{eJoint::Z_PRISMATIC, 0.}, {eJoint::Z_ROTATIONAL, 0.}, {eJoint::Y_ROTATIONAL, 0.}}, false, false}),
      position_({{eJoint::Z_PRISMATIC, 0.}, {eJoint::Z_ROTATIONAL, 0.}, {eJoint::Y_ROTATIONAL, 0.}}),
      pumpEnabled_(false),
      valveOpen_(false),
      pressure_(0.),
      armId_(armId),
      sender_(sender) {}

void Arm::setJoint(const eJoint& joint, const double value) {
    lastCommand_.position.at(joint) = value;
    sender_.sendArmCommand(armId_, lastCommand_.position.at(eJoint::Z_PRISMATIC), lastCommand_.position.at(eJoint::Z_ROTATIONAL),
                           lastCommand_.position.at(eJoint::Y_ROTATIONAL), lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Arm::startPump(bool enable) {
    lastCommand_.pumpEnabled = enable;
    sender_.sendArmCommand(armId_, lastCommand_.position.at(eJoint::Z_PRISMATIC), lastCommand_.position.at(eJoint::Z_ROTATIONAL),
                           lastCommand_.position.at(eJoint::Y_ROTATIONAL), lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Arm::openValve(bool open) {
    lastCommand_.valveOpen = open;
    sender_.sendArmCommand(armId_, lastCommand_.position.at(eJoint::Z_PRISMATIC), lastCommand_.position.at(eJoint::Z_ROTATIONAL),
                           lastCommand_.position.at(eJoint::Y_ROTATIONAL), lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Arm::updateState(const ArmInput& input) {
    position_.at(eJoint::Z_PRISMATIC) = input.zPrismatic();
    position_.at(eJoint::Z_ROTATIONAL) = input.zRotational();
    position_.at(eJoint::Y_ROTATIONAL) = input.yRotational();
    pumpEnabled_ = input.pumpOn();
    valveOpen_ = input.valveOpen();
    pressure_ = input.pressure();
}

}  // namespace rd
