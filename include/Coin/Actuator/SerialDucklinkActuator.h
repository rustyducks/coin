#ifndef SERIALDUCKLINKACTUATOR_H
#define SERIALDUCKLINKACTUATOR_H

#include "Coin/Actuator/ActuatorBase.h"
#include "Coin/Communication/Ducklink/SerialDucklink.h"

namespace rd {
class SerialDucklinkActuator : public ActuatorBase {
   public:
    SerialDucklinkActuator(SerialDucklink& ducklink) : ducklink_(ducklink) {}

   protected:
    SerialDucklink& ducklink_;
};
}  // namespace rd

#endif /* SERIALDUCKLINKACTUATOR_H */
