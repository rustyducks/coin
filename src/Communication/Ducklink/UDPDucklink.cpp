#include "Coin/Communication/Ducklink/UDPDucklink.h"

#include <unistd.h>

#include <cstring>

namespace rd {
UDPDucklink::UDPDucklink(const std::string& addr, int port) : fPort_(port), fAddr_(addr) {
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", fPort_);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &fAddrinfo_));
    if (r != 0 || fAddrinfo_ == NULL) {
        throw std::runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    fSocket_ = socket(fAddrinfo_->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if (fSocket_ == -1) {
        freeaddrinfo(fAddrinfo_);
        throw std::runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

UDPDucklink::~UDPDucklink() {
    freeaddrinfo(fAddrinfo_);
    close(fSocket_);
}

int UDPDucklink::getSocket() const { return fSocket_; }

int UDPDucklink::getPort() const { return fPort_; }

std::string UDPDucklink::getAddr() const { return fAddr_; }

void UDPDucklink::sendSpeedJson(const Speed& speed, const std::string& name) {
    std::ostringstream oss;
    oss << "{\"" << name << "\": {\"vx\":" << speed.vx() << ",\"vy\":" << speed.vy() << ",\"vtheta\":" << speed.vtheta() << "}}";
    std::string str = oss.str();
    send(str.c_str(), str.size());
}

void UDPDucklink::sendPointOrientedJson(const PointOriented& point, const std::string& name) {
    std::ostringstream oss;
    oss << "{\"" << name << "\": {\"x\":" << point.x() << ",\"y\":" << point.y() << ",\"theta\":" << point.theta().value() << "}}";
    std::string str = oss.str();
    send(str.c_str(), str.size());
}

std::vector<std::unique_ptr<Input>> UDPDucklink::getInputs() { return {}; }

void UDPDucklink::sendSpeed(const Speed& speed) {
    protoduck::Message msg;
    auto* speedCmd = msg.mutable_speed();
    speedCmd->set_vx(speed.vx());
    speedCmd->set_vy(speed.vy());
    speedCmd->set_vtheta(speed.vtheta());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendDucklinkMessage(msg);
}

void UDPDucklink::sendPoseReport(const PointOriented& pose) {
    protoduck::Message msg;
    auto* poseReport = msg.mutable_pos();
    poseReport->set_x(pose.x());
    poseReport->set_y(pose.y());
    poseReport->set_theta(pose.theta().value());
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_STATUS);
    sendDucklinkMessage(msg);
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
    sendDucklinkMessage(msg);
}
void UDPDucklink::sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) {
    protoduck::Message msg;
    protoduck::Hat* hat = msg.mutable_hat();
    hat->set_height(height);
    hat->set_pump(pumpEnabled);
    hat->set_valve(valveOpen);
    msg.set_msg_type(protoduck::Message_MsgType::Message_MsgType_COMMAND);
    sendDucklinkMessage(msg);
}

template <typename T>
int UDPDucklink::send(const T* msg, size_t size) {
    return sendto(fSocket_, msg, size, 0, fAddrinfo_->ai_addr, fAddrinfo_->ai_addrlen);
}
int UDPDucklink::sendDucklinkMessage(protoduck::Message& message) {
    message.set_source(protoduck::Message_Agent::Message_Agent_DIFF);
    std::vector<uint8_t> toSend(message.ByteSize() + 3 + 1);
    toSend.at(0) = 0xFF;
    toSend.at(1) = 0xFF;
    toSend.at(2) = message.ByteSize();
    uint8_t checksum = 0;
    message.SerializeToArray(toSend.data() + 3, message.ByteSize());
    for (size_t i = 3; i < toSend.size() - 1; i++) {
        checksum ^= toSend[i];
    }
    toSend.back() = checksum;
    return send(toSend.data(), toSend.size());
}
}  // namespace rd
