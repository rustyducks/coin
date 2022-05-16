#ifndef DISPENSER_H
#define DISPENSER_H
#include <GeometryTools/Point.h>

#include <memory>
#include <vector>

namespace rd {

class Hexa;

class Dispenser {
   public:
    enum eAccessibleColor {
        YELLOW,
        PURPLE,
        BOTH,
    };
    enum eOrientation {
        HORIZONTAL,
        VERTICAL,
    };
    Dispenser(const Point& position, eAccessibleColor accessibleBy, eOrientation orientation);

    void pushHexa(std::shared_ptr<Hexa> hexa);
    std::shared_ptr<Hexa> retrieveHexa();
    size_t hexaLeft() { return hexas_.size(); }
    const Point& position() { return position_; }

   protected:
    Point position_;
    eAccessibleColor accessibleBy_;
    eOrientation orientation_;
    std::vector<std::shared_ptr<Hexa>> hexas_;
};

}  // namespace rd

#endif /* DISPENSER_H */
