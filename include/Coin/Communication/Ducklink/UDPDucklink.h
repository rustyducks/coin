#ifndef UDPDUCKLINK_H
#define UDPDUCKLINK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <stdexcept>

#include "Coin/Communication/CommunicationBase.h"
#include "Coin/Communication/JSONInterface.h"
#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"
#include "generated/messages.pb.h"

namespace rd {

class UDPDucklink : CommunicationBase, JsonInterface {
   public:
    UDPDucklink(const std::string &addr, int port);
    ~UDPDucklink();

    int getSocket() const;
    int getPort() const;
    std::string getAddr() const;

    void sendSpeedJson(const Speed &speed, const std::string &name = "speed") override;
    void sendPointOrientedJson(const PointOriented &point, const std::string &name = "point_oriented") override;

    virtual std::vector<std::unique_ptr<Input>> getInputs() override;
    virtual void sendSpeed(const Speed &speed) override;
    virtual void sendPoseReport(const PointOriented &pose) override;
    virtual void sendArmCommand(const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled,
                                const bool valveOpen) override;
    virtual void sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) override;

   protected:
    template <typename T>
    int send(const T *msg, size_t size);
    int sendDucklinkMessage(protoduck::Message &message);
    int fSocket_;
    int fPort_;
    std::string fAddr_;
    struct addrinfo *fAddrinfo_;
};
}  // namespace rd

#endif /* UDPDUCKLINK_H */
