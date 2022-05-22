#include "Coin/Actuator/Finger.h"

namespace rd {
Finger::Finger(FingerCommandSenderInterface& fingersender) : fingerSender_(fingersender), touching_(NOTHING) {}

void Finger::deployFinger() { fingerSender_.sendFingerCommand(1); }

void Finger::retractFinger() { fingerSender_.sendFingerCommand(0); }

void Finger::updateState(const ResistorInput& resistor) {
    if (resistor.resistorValue < 100) {
        touching_ = eTouching::NOTHING;
    } else if (resistor.resistorValue < 400) {
        touching_ = eTouching::RED;
    } else if (resistor.resistorValue < 575) {
        touching_ = eTouching::YELLOW;
    } else if (resistor.resistorValue < 675) {
        touching_ = eTouching::PURPLE;
    } else {
        touching_ = eTouching::SHORT;
    }
}

}  // namespace rd
