#ifndef ROBOT_H
#define ROBOT_H
#include <memory>

#include "Navigation/PositionControlBase.h"

namespace rd {
class Robot {
   public:
    Robot(const PositionControlParameters& posControlParams);
    void setTargetPoint(const PointOriented& target);
    void setTargetSpeed(const Speed& speed);
    void controlPosition();

   protected:
    PointOriented position_;

    enum ePositionControlType { IDLE, SPEED, POSITION_CONTROL };
    ePositionControlType positionControlType_;
    std::unique_ptr<PositionControlBase> positionControl_;
    Speed targetSpeed_;
};
}  // namespace rd

#endif /* ROBOT_H */
