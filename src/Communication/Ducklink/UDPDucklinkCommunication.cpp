#include "Coin/Communication/Ducklink/UDPDucklinkCommunication.h"

#include <unistd.h>

#include <cstring>

namespace rd {

UDPDucklink::UDPDucklink(const std::string& addr, const int port) : UDPDucklinkClient(addr, port) {}

std::vector<std::unique_ptr<Input>> UDPDucklink::getInputs() {
    std::vector<protoduck::Message> msgs;
    std::vector<std::unique_ptr<Input>> toReturn;
    if (getMessages(msgs) > 0) {
        for (const auto& msg : msgs) {
            if (msg.msg_type() == protoduck::Message_MsgType::Message_MsgType_STATUS) {
                if (msg.has_pos()) {
                    PointOriented p(msg.pos().x(), msg.pos().y(), msg.pos().theta());
                    toReturn.push_back(std::make_unique<PointOrientedInput>(eInput::POSITION_REPORT, p));
                } else if (msg.has_speed()) {
                    Speed s(msg.speed().vx(), msg.speed().vy(), msg.speed().vtheta());
                    toReturn.push_back(std::make_unique<SpeedInput>(eInput::SPEED_REPORT, s));
                } else if (msg.has_arm()) {
                    toReturn.push_back(std::make_unique<ArmInput>(eInput::ARM_STATUS, msg.arm().traz(), msg.arm().rotz(), msg.arm().roty(), msg.arm().pump(),
                                                                  msg.arm().valve(), msg.arm().pressure()));
                } else if (msg.has_hat()) {
                    toReturn.push_back(
                        std::make_unique<HatInput>(eInput::HAT_STATUS, msg.hat().height(), msg.hat().pump(), msg.hat().valve(), msg.hat().pressure()));
                } else if (msg.has_player_pos()) {
                    Point p(msg.player_pos().pos().x(), msg.player_pos().pos().y());
                    toReturn.push_back(std::make_unique<LidarAdversary>(eInput::LIDAR_ADVERSARY, msg.player_pos().aruco_id(), p));
                }
            } else if (msg.msg_type() == protoduck::Message_MsgType::Message_MsgType_COMMAND) {
                if (msg.has_speed()) {
                    Speed s(msg.speed().vx(), msg.speed().vy(), msg.speed().vtheta());
                    toReturn.push_back(std::make_unique<SpeedInput>(eInput::SPEED_COMMAND, s));
                }
            }
        }
    }
    return toReturn;
}

void UDPDucklink::sendSpeed(const Speed& speed) {
    protoduck::Message msg;
    auto* speedCmd = msg.mutable_speed();
    speedCmd->set_vx(speed.vx());
    speedCmd->set_vy(speed.vy());
    speedCmd->set_vtheta(speed.vtheta());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}

void UDPDucklink::sendPoseReport(const PointOriented& pose) {
    protoduck::Message msg;
    auto* poseReport = msg.mutable_pos();
    poseReport->set_x(pose.x());
    poseReport->set_y(pose.y());
    poseReport->set_theta(pose.theta().value());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_STATUS);
    sendMessage(msg);
}

void UDPDucklink::sendArmCommand(const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled, const bool valveOpen) {
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
void UDPDucklink::sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) {
    protoduck::Message msg;
    protoduck::Hat* hat = msg.mutable_hat();
    hat->set_height(height);
    hat->set_pump(pumpEnabled);
    hat->set_valve(valveOpen);
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}
}  // namespace rd
