#ifndef UDPJSON_H
#define UDPJSON_H

#include "Coin/Communication/JSONInterface.h"
#include "Ducklink/UDPDucklink.h"  // Hack to easily create a UDP sender

namespace rd {
class UDPJson : public JsonInterface, public UDPDucklinkSender {
   public:
    UDPJson(const std::string &addr, const int port);
    virtual void sendSpeedJson(const Speed &speed, const std::string &name = "speed") override;
    virtual void sendPointOrientedJson(const PointOriented &point, const std::string &name = "point_oriented") override;
};
}  // namespace rd

#endif /* UDPJSON_H */
