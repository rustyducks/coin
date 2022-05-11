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
                    toReturn.push_back(std::make_unique<ArmInput>(eInput::ARM_STATUS, msg.arm().arm_id(), msg.arm().traz(), msg.arm().rotz(), msg.arm().roty(),
                                                                  msg.arm().pump(), msg.arm().valve(), msg.arm().pressure()));
                } else if (msg.has_hat()) {
                    toReturn.push_back(
                        std::make_unique<HatInput>(eInput::HAT_STATUS, msg.hat().height(), msg.hat().pump(), msg.hat().valve(), msg.hat().pressure()));
                } else if (msg.has_player_poses()) {
                    std::vector<std::pair<int, Point>> adv;
                    for (auto& a : msg.player_poses().player_poses()) {
                        auto pos = a.pos();
                        Point p(pos.x(), pos.y());
                        adv.push_back(std::make_pair(a.aruco_id(), p));
                    }
                    toReturn.push_back(std::make_unique<LidarAdversaries>(eInput::LIDAR_ADVERSARIES, adv));
                } else if (msg.has_procedure()) {
                    toReturn.push_back(std::make_unique<ProcedureInput>(eInput::PROCEDURE_STATUS, msg.procedure().status()));
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

void UDPDucklink::sendPoseCommand(const PointOriented& pose) {
    protoduck::Message msg;
    auto* poseCommand = msg.mutable_pos();
    poseCommand->set_x(pose.x());
    poseCommand->set_y(pose.y());
    poseCommand->set_theta(pose.theta().value());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}

void UDPDucklink::sendArmCommand(const unsigned int armId, const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled,
                                 const bool valveOpen) {
    protoduck::Message msg;
    protoduck::Arm* arm = msg.mutable_arm();
    if (armId > 1) {
        return;
    }
    arm->set_arm_id(protoduck::ArmID(armId));
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

void UDPDucklink::sendProcedureCommand(const unsigned int armId, const protoduck::Procedure_Proc procedure, const int param) {
    protoduck::Message msg;
    protoduck::Procedure* proc = msg.mutable_procedure();
    if (armId > 1) {
        return;
    }
    proc->set_arm_id(protoduck::ArmID(armId));
    proc->set_proc(procedure);
    proc->set_param(param);
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendMessage(msg);
}
}  // namespace rd
