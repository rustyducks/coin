#ifndef HAT_H
#define HAT_H

#include "Coin/Actuator/SerialDucklinkActuator.h"

namespace rd {
class Hat : public SerialDucklinkActuator {
   public:
    Hat(SerialDucklink& ducklink);

   protected:
};
}  // namespace rd

#endif /* HAT_H */
