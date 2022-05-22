#ifndef FINGER_H
#define FINGER_H

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Communication/CommunicationBase.h"

namespace rd {
class Finger : public ActuatorBase<ResistorInput> {
   public:
    enum eTouching { NOTHING, YELLOW, PURPLE, RED, SHORT };

    Finger(FingerCommandSenderInterface& fingerSender);
    void updateState(const ResistorInput& resistor);

    void deployFinger();
    void retractFinger();

    eTouching isTouching() { return touching_; }

    // Resistor Values: 616 --> Purple
    // 230 --> Red
    // 510 --> Yellow

   protected:
    FingerCommandSenderInterface& fingerSender_;
    eTouching touching_;
};
}  // namespace rd

#endif /* FINGER_H */
