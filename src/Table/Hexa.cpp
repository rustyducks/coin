#include "Coin/Table/Hexa.h"

namespace rd {
Hexa::Hexa(const Point& position, eFace face, eColor color, bool poseKnown) : position_(position), face_(face), color_(color), poseKnown_(poseKnown) {}
}  // namespace rd
