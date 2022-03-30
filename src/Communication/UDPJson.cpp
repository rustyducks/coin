#include "Coin/Communication/UDPJson.h"

namespace rd {
UDPJson::UDPJson(const std::string &addr, const int port) : UDPDucklinkClient(addr, port) {}

void UDPJson::sendSpeedJson(const Speed &speed, const std::string &name) {
    std::ostringstream oss;
    oss << "{\"" << name << "\": {\"vx\":" << speed.vx() << ",\"vy\":" << speed.vy() << ",\"vtheta\":" << speed.vtheta() << "}}";
    std::string str = oss.str();
    send(str.c_str(), str.size());
}

void UDPJson::sendPointOrientedJson(const PointOriented &point, const std::string &name) {
    std::ostringstream oss;
    oss << "{\"" << name << "\": {\"x\":" << point.x() << ",\"y\":" << point.y() << ",\"theta\":" << point.theta().value() << "}}";
    std::string str = oss.str();
    send(str.c_str(), str.size());
}
}  // namespace rd
