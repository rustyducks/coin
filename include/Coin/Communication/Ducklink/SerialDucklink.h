#ifndef SERIALDUCKLINK_H
#define SERIALDUCKLINK_H

#include "Coin/Communication/CommunicationBase.h"
#include "generated/messages.pb.h"
#include "serialib.h"

namespace rd {
class SerialDucklink : public CommunicationBase {
   public:
    SerialDucklink(const std::string& serialPath, const unsigned int baudrate);

    virtual std::vector<std::unique_ptr<rd::Input>> getInputs() override;

    void sendSpeed(const Speed& speed);

   protected:
    void sendMessage(protoduck::Message& message);

    enum eReceiveState { START_1FF, START_2FF, LEN, PAYLOAD, CHECKSUM };
    eReceiveState receiveState_;
    uint8_t expectedLen_;
    uint8_t payloadRead_;
    uint8_t computedChecksum_;
    serialib serial_;
    uint8_t constructingPayload_[256];
};
}  // namespace rd

#endif /* SERIALDUCKLINK_H */
