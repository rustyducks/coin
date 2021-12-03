#ifndef SERIALDUCKLINK_H
#define SERIALDUCKLINK_H

#include "Coin/Communication/CommunicationBase.h"
#include "Coin/Communication/Ducklink/DucklinkReceiver.h"
#include "generated/messages.pb.h"
#include "serialib.h"

namespace rd {
class SerialDucklink : public CommunicationBase {
   public:
    SerialDucklink(const std::string& serialPath, const unsigned int baudrate);

    virtual std::vector<std::unique_ptr<rd::Input>> getInputs() override;

    void sendSpeed(const Speed& speed) override;
    void sendPoseReport(const PointOriented& pose) override;
    void sendArmCommand(const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled, const bool valveOpen) override;
    void sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) override;

   protected:
    void sendMessage(protoduck::Message& message);
    serialib serial_;
    DucklinkReceiver ducklinkReceiver_;
};
}  // namespace rd

#endif /* SERIALDUCKLINK_H */
