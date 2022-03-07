#ifndef UDPDUCKLINKCOMMUNICATION_H
#define UDPDUCKLINKCOMMUNICATION_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <stdexcept>

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Communication/CommunicationBase.h"
#include "Coin/Communication/JSONInterface.h"
#include "Ducklink/UDPDucklink.h"
#include "Ducklink/messages.pb.h"
#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"
namespace rd {

class UDPDucklinkInput : public CommunicationInputBase, public UDPDucklinkReceiver {
   public:
    UDPDucklinkInput(const std::string& addr, const int port);
    /*void sendSpeedJson(const Speed &speed, const std::string &name = "speed") override;
    void sendPointOrientedJson(const PointOriented &point, const std::string &name = "point_oriented") override;*/

    virtual std::vector<std::unique_ptr<Input>> getInputs() override;
};

class UDPDucklinkOutput : public CommunicationOutputBase,
                          public ArmCommandSenderInterface,
                          public HatCommandSenderInterface,
                          public UDPDucklinkSender,
                          public StackerProcedureSenderInterface {
   public:
    UDPDucklinkOutput(const std::string& addr, const int port);
    virtual void sendSpeed(const Speed& speed) override;
    virtual void sendPoseReport(const PointOriented& pose) override;
    virtual void sendArmCommand(const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled, const bool valveOpen);
    virtual void sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen);
    virtual void sendHome(const uint8_t armId) override;
    virtual void sendStack(const uint8_t armId, const double stackHeight) override;
    virtual void sendFlipAndStack(const uint8_t armId, const double stackHeight) override;
    virtual void sendTakeFromStack(const uint8_t armId, const double stackHeight) override;
};
}  // namespace rd

#endif /* UDPDUCKLINKCOMMUNICATION_H */
