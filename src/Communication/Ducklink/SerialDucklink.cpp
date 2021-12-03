#include "Coin/Communication/Ducklink/SerialDucklink.h"

namespace rd {
SerialDucklink::SerialDucklink(const std::string& serialPath, const unsigned int baudrate) {
    if (serial_.openDevice(serialPath.c_str(), baudrate) != 1) {
        throw std::runtime_error("Cannot open serial");
    }
}

std::vector<std::unique_ptr<Input>> SerialDucklink::getInputs() {
    std::vector<std::unique_ptr<Input>> toReturn;
    protoduck::Message msg;
    char rcvChar;
    uint8_t rcv;
    while (serial_.available()) {
        if (serial_.readChar(&rcvChar) == 1) {
            rcv = static_cast<uint8_t>(rcvChar);
            if (ducklinkReceiver_.pushNewOct(rcv, msg)) {
                // A message is complete
                if (msg.msg_type() == protoduck::Message_MsgType::Message_MsgType_STATUS) {
                    if (msg.has_pos()) {
                        PointOriented p(msg.pos().x(), msg.pos().y(), msg.pos().theta());
                        toReturn.push_back(std::make_unique<PointOrientedInput>(eInput::POSITION_REPORT, p));
                    } else if (msg.has_speed()) {
                        Speed s(msg.speed().vx(), msg.speed().vy(), msg.speed().vtheta());
                        toReturn.push_back(std::make_unique<SpeedInput>(eInput::SPEED_REPORT, s));
                    } else if (msg.has_arm()) {
                        toReturn.push_back(std::make_unique<ArmInput>(eInput::ARM_STATUS, msg.arm().traz(), msg.arm().rotz(), msg.arm().roty(),
                                                                      msg.arm().pump(), msg.arm().valve(), msg.arm().pressure()));
                    } else if (msg.has_hat()) {
                        toReturn.push_back(
                            std::make_unique<HatInput>(eInput::HAT_STATUS, msg.hat().height(), msg.hat().pump(), msg.hat().valve(), msg.hat().pressure()));
                    }
                } else if (msg.msg_type() == protoduck::Message_MsgType::Message_MsgType_COMMAND) {
                    if (msg.has_speed()) {
                        Speed s(msg.speed().vx(), msg.speed().vy(), msg.speed().vtheta());
                        toReturn.push_back(std::make_unique<SpeedInput>(eInput::SPEED_COMMAND, s));
                    }
                }
            }
        }
    }
    return toReturn;
}

void SerialDucklink::sendSpeed(const Speed& speed) {
    protoduck::Message msg;
    auto* speedCmd = msg.mutable_speed();
    speedCmd->set_vx(speed.vx());
    speedCmd->set_vy(speed.vy());
    speedCmd->set_vtheta(speed.vtheta());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}

void SerialDucklink::sendPoseReport(const PointOriented& pose) {
    protoduck::Message msg;
    auto* poseReport = msg.mutable_pos();
    poseReport->set_x(pose.x());
    poseReport->set_y(pose.y());
    poseReport->set_theta(pose.theta().value());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_STATUS);
    sendMessage(msg);
}

void SerialDucklink::sendMessage(protoduck::Message& message) {
    message.set_source(protoduck::Message_Agent::Message_Agent_DIFF);
    std::vector<uint8_t> toSend(message.ByteSize());
    uint8_t header[3] = {0xFF, 0xFF, 0};
    header[2] = message.ByteSize();
    uint8_t checksum = 0;
    message.SerializeToArray(toSend.data(), message.ByteSize());
    for (size_t i = 0; i < toSend.size(); i++) {
        checksum ^= toSend[i];
    }
    serial_.writeBytes(header, 3);
    serial_.writeBytes(toSend.data(), message.ByteSize());
    serial_.writeBytes(&checksum, 1);
}

void SerialDucklink::sendArmCommand(const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled, const bool valveOpen) {
    protoduck::Message msg;
    protoduck::Arm* arm = msg.mutable_arm();
    arm->set_arm_id(protoduck::ArmID::ARM1);
    arm->set_traz(zPrismatic);
    arm->set_rotz(zRotational);
    arm->set_roty(yRotational);
    arm->set_pump(pumpEnabled);
    arm->set_valve(valveOpen);
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}
void SerialDucklink::sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) {
    protoduck::Message msg;
    protoduck::Hat* hat = msg.mutable_hat();
    hat->set_height(height);
    hat->set_pump(pumpEnabled);
    hat->set_valve(valveOpen);
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}

}  // namespace rd
