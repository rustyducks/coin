#ifndef WORKSHED_H
#define WORKSHED_H

#include <GeometryTools/Point.h>

#include <memory>
#include <vector>

#include "Coin/Table/Colors.h"

namespace rd {
class Hexa;

class WorkShed {
   public:
    WorkShed(const Point& position, const eColor& color);

    void setLeftSlot(std::shared_ptr<Hexa> hexa) { leftSlot_ = hexa; }
    void setRightSlot(std::shared_ptr<Hexa> hexa) { rightSlot_ = hexa; }

   protected:
    Point position_;
    eColor color_;
    std::shared_ptr<Hexa> leftSlot_;
    std::shared_ptr<Hexa> rightSlot_;
    bool hasStatuette_;
    bool hasReplica_;
    std::vector<std::shared_ptr<Hexa>> collectedHexas_;
};

}  // namespace rd

#endif /* WORKSHED_H */
