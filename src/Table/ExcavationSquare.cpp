#include "Coin/Table/ExcavationSquare.h"

namespace rd {
ExcavationSquare::ExcavationSquare(unsigned int id, Point location, std::unordered_set<eColor> possibleColors)
    : id_(id), location_(location), possibleColors_(possibleColors), state_(UNFLIPPED) {}

}  // namespace rd
