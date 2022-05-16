#include "Coin/Table/Hexa.h"

namespace rd {
Hexa::Hexa(const std::string& name, const Point& position, eFace face, eColor color, bool poseKnown)
    : name_(name), position_(position), face_(face), color_(color), poseKnown_(poseKnown) {}
}  // namespace rd
