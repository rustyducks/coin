#include <Coin/Table/Dispenser.h>

namespace rd {

Dispenser::Dispenser(const Point& position, eAccessibleColor accessibleBy, eOrientation orientation)
    : position_(position), accessibleBy_(accessibleBy), orientation_(orientation) {}

void Dispenser::pushHexa(std::shared_ptr<Hexa> hexa) { hexas_.push_back(hexa); }
}  // namespace rd
