#ifndef BEHAVIORBASE_H
#define BEHAVIORBASE_H

#include "Coin/Robot.h"

namespace rd {
class BehaviorBase {
 public:
  BehaviorBase(Robot& robot) : robot_(robot) {}
  virtual void tick() = 0;

 protected:
  Robot& robot_;
};
}  // namespace rd

#endif /* BEHAVIORBASE_H */
