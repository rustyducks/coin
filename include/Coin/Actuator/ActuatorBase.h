#ifndef ACTUATORBASE_H
#define ACTUATORBASE_H

#include "Coin/Communication/CommunicationBase.h"

namespace rd {
template <class T>
class ActuatorBase {
   public:
    virtual void updateState(const T& input) = 0;
};
}  // namespace rd

#endif /* ACTUATORBASE_H */
