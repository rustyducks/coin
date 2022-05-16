#ifndef HEXA_H
#define HEXA_H

#include <GeometryTools/Point.h>

#include <memory>

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

    Hexa(const std::string& name, const Point& position, eFace face, eColor color, bool poseKnown);
    std::string name_;
    Point position_;
    eFace face_;
    eColor color_;
    bool poseKnown_;  // Is the pose an estimation? (hexa in the excavation site)
};
typedef std::shared_ptr<Hexa> HexaPtr;

}  // namespace rd

#endif /* HEXA_H */
