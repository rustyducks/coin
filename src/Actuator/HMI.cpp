#include "Coin/Actuator/HMI.h"

namespace rd {
HMI::HMI(HMICommandSenderInterface& sender) : lastCommand_({0, 0}), button_(false), color_(false), tirette_(true), sender_(sender) {
    sender_.sendHMICommand(lastCommand_.scoreDisplay, lastCommand_.led);
}

void HMI::updateState(const HMIInput& input) {
    button_ = input.button_;
    color_ = input.color_;
    tirette_ = input.tirette_;
    scoreDisplay_ = input.scoreDisplay_;
    color_ = static_cast<eLedColor>(input.color_);
}

void HMI::setScoreDisplay(uint32_t score) {
    lastCommand_.scoreDisplay = score;
    sender_.sendHMICommand(lastCommand_.scoreDisplay, lastCommand_.led);
}

void HMI::setLed(HMI::eLedColor color) {
    lastCommand_.led = color;
    sender_.sendHMICommand(lastCommand_.scoreDisplay, lastCommand_.led);
}

}  // namespace rd
