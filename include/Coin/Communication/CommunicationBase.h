#ifndef COMMUNICATIONBASE_H
#define COMMUNICATIONBASE_H

#include <memory>
#include <vector>

#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"

namespace rd {

enum eInput { GO_TO_POINT, GO_TO_POINT_ORIENTED, POSITION, SPEED };

class Input {
   public:
    Input(eInput type) : type_(type) {}
    eInput type() const { return type_; }

   protected:
    eInput type_;
};

class PointInput : public Input {
   public:
    PointInput(eInput type, const Point& point) : Input(type), point_(point) {}
    const Point& getPoint() const { return point_; }

   protected:
    Point point_;
};

class PointOrientedInput : public Input {
   public:
    PointOrientedInput(eInput type, const PointOriented& point) : Input(type), point_(point) {}
    const PointOriented& getPoint() const { return point_; }

   protected:
    PointOriented point_;
};

class SpeedInput : public Input {
   public:
    SpeedInput(eInput type, const Speed& speed) : Input(type), speed_(speed) {}
    const Speed& getSpeed() const { return speed_; }

   protected:
    Speed speed_;
};

class CommunicationBase {
   public:
    virtual std::vector<std::unique_ptr<Input>> getInputs() = 0;
};
}  // namespace rd

#endif /* COMMUNICATIONBASE_H */
