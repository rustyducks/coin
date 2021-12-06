#include "Coin/Actuator/Hat.h"

namespace rd {
Hat::Hat(HatCommandSenderInterface& sender)
    : lastCommand_({0., false, false}), height_(0.), pumpEnabled_(false), valveOpen_(false), pressure_(0.), sender_(sender) {}

void Hat::setHeight(const double height) {
    lastCommand_.height = height;
    sender_.sendHatCommand(lastCommand_.height, lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Hat::startPump(const bool enable) {
    lastCommand_.pumpEnabled = enable;
    sender_.sendHatCommand(lastCommand_.height, lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Hat::openValve(const bool open) {
    lastCommand_.valveOpen = open;
    sender_.sendHatCommand(lastCommand_.height, lastCommand_.pumpEnabled, lastCommand_.valveOpen);
}

void Hat::updateState(const HatInput& input) {
    height_ = input.height();
    pressure_ = input.pressure();
    valveOpen_ = input.valveOpen();
    pumpEnabled_ = input.pumpOn();
}
}  // namespace rd