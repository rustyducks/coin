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

class UDPDucklink : public CommunicationOutputBase,
                    public CommunicationInputBase,
                    public ArmCommandSenderInterface,
                    public HatCommandSenderInterface,
                    public ProcedureCommandSenderInterface,
                    public HMICommandSenderInterface,
                    public FingerCommandSenderInterface,
                    public UDPDucklinkClient {
   public:
    UDPDucklink(const std::string& addr, const int port);
    virtual void sendSpeed(const Speed& speed) override;
    virtual void sendPoseReport(const PointOriented& pose) override;
    virtual void sendPoseCommand(const PointOriented& pose) override;
    virtual void sendArmCommand(const unsigned int armId, const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled,
                                const bool valveOpen);
    virtual void sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen);
    virtual void sendProcedureCommand(const unsigned int armId, const protoduck::Procedure_Proc procedure, const int param) override;
    virtual void sendHMICommand(const uint32_t scoreDisplay, const uint32_t led) override;
    virtual void sendFingerCommand(const int command) override;

    virtual std::vector<std::unique_ptr<Input>> getInputs() override;
};
}  // namespace rd

#endif /* UDPDUCKLINKCOMMUNICATION_H */
