#ifndef HEXA_H
#define HEXA_H

#include <GeometryTools/Point.h>

namespace rd {
class Hexa {
   public:
    enum eColor {
        UNKNOWN,
        GREEN,
        BLUE,
        RED,
    };
    enum eFace {
        ROCKS,
        TREASURE,
        STANDING,
    };

    Hexa(const Point& position, eFace face, eColor color, bool poseKnown);

   protected:
    Point position_;
    eFace face_;
    eColor color_;
    bool poseKnown_;  // Is the pose an estimation? (hexa in the excavation site)
};
}  // namespace rd

#endif /* HEXA_H */
