#include "Coin/Actuator/Arm.h"

namespace rd {
Arm::Arm(SerialDucklink& ducklink)
    : SerialDucklinkActuator(ducklink),
      position_({{eJoint::Z_PRISMATIC, 0.}, {eJoint::Z_ROTATIONAL, 0.}, {eJoint::Y_ROTATIONAL, 0.}}),
      pumpEnabled_(false),
      valveOpen_(false) {}

void Arm::setJoint(const eJoint& joint, const double value) {
    double zPri, zRot, yRot;
    zPri = position_.at(eJoint::Z_PRISMATIC);
    zRot = position_.at(eJoint::Z_ROTATIONAL);
    yRot = position_.at(eJoint::Y_ROTATIONAL);
    switch (joint) {
        case eJoint::Z_PRISMATIC:
            zPri = value;
            break;
        case eJoint::Z_ROTATIONAL:
            zRot = value;
            break;
        case eJoint::Y_ROTATIONAL:
            yRot = value;
            break;
        default:
            break;
    }
    ducklink_.sendArmCommand(zPri, zRot, yRot, pumpEnabled_, valveOpen_);
}

}  // namespace rd
