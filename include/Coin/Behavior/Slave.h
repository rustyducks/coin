#ifndef SLAVE_H
#define SLAVE_H
#include "Coin/Behavior/BehaviorBase.h"

namespace rd {
class Slave : public BehaviorBase {
  Slave(Robot& robot);

  virtual void tick() override;

  void newTargetOrientedPoint(const PointOriented& newPoint);
  void newTargetPoint(const Point& newPoint);

 protected:
  enum eSlaveState { IDLE, POSITION_CONTROL, SPEED_CONTROL };
  eSlaveState state_;
  PointOriented target_;
  Speed speedCommand_;
};
}  // namespace rd
#endif /* SLAVE_H */
