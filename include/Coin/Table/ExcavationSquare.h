#ifndef EXCAVATIONSQUARE_H
#define EXCAVATIONSQUARE_H

#include <GeometryTools/Point.h>

#include <memory>
#include <unordered_set>

namespace rd {
class ExcavationSquare {
   public:
    enum eColor { YELLOW, PURPLE, RED };
    enum eState { UNFLIPPED, FLIPPED };

    ExcavationSquare(unsigned int id, Point location, std::unordered_set<eColor> possibleColors);

    const Point& location() { return location_; }
    const std::unordered_set<eColor> possibleColors() { return possibleColors_; }
    bool isFlipped() { return state_ == FLIPPED; }
    void flip() { state_ = FLIPPED; }
    void setColor(eColor color) {
        possibleColors_.clear();
        possibleColors_.insert(color);
    }

    bool knownColor() { return possibleColors_.size() == 1; }
    unsigned int id() { return id_; }

   protected:
    unsigned int id_;
    Point location_;
    std::unordered_set<eColor> possibleColors_;
    eState state_;
};

typedef std::shared_ptr<ExcavationSquare> ExcavationSquarePtr;

}  // namespace rd

#endif /* EXCAVATIONSQUARE_H */
