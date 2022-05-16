#ifndef HMI_H
#define HMI_H

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Communication/CommunicationBase.h"

namespace rd {
class HMI : public ActuatorBase<HMIInput> {
   public:
    enum eLedColor {
        BLACK = 0b000,
        RED = 0b001,
        GREEN = 0b010,
        BLUE = 0b100,
        YELLOW = RED | GREEN,
        CYAN = GREEN | BLUE,
        MAGENTA = RED | BLUE,
        WHITE = RED | GREEN | BLUE,
    };

    HMI(HMICommandSenderInterface& sender);

    virtual void updateState(const HMIInput& input) override;

    void setScoreDisplay(const uint32_t score);
    void setLed(const eLedColor);

    bool button() const { return button_; }
    bool color() const { return color_; }
    bool tirette() const { return tirette_; }
    uint32_t scoreDisplay() const { return scoreDisplay_; }
    eLedColor ledColor() const { return led_; }

   protected:
    struct sHMICommand {
        uint32_t scoreDisplay;
        uint32_t led;
    };
    sHMICommand lastCommand_;
    bool button_, color_, tirette_;
    uint32_t scoreDisplay_;
    eLedColor led_;

    HMICommandSenderInterface& sender_;
};
}  // namespace rd

#endif /* HMI_H */
