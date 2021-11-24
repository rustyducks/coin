#ifndef COMMUNICATIONBASE_H
#define COMMUNICATIONBASE_H

#include <memory>
#include <vector>

#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"

namespace rd {

enum eInput { GO_TO_POINT, GO_TO_POINT_ORIENTED, POSITION, SPEED, ARM_STATUS, HAT_STATUS };

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

class ArmInput : public Input {
   public:
    ArmInput(eInput type, const double zPrismatic, const double zRotational, const double yRotational, const bool pumpOn, const bool valveOpen,
             const double pressure)
        : Input(type),
          zPrismatic_(zPrismatic),
          zRotational_(zRotational),
          yRotational_(yRotational),
          pressure_(pressure),
          pumpOn_(pumpOn),
          valveOpen_(valveOpen) {}
    double zPrismatic() const { return zPrismatic_; }
    double zRotational() const { return zRotational_; }
    double yRotational() const { return yRotational_; }
    double pressure() const { return pressure_; }
    bool pumpOn() const { return pumpOn_; }
    bool valveOpen() const { return valveOpen_; }

   protected:
    double zPrismatic_, zRotational_, yRotational_, pressure_;
    bool pumpOn_, valveOpen_;
};

class HatInput : public Input {
   public:
    HatInput(eInput type, const double height, const bool pumpOn, const bool valveOpen, const double pressure)
        : Input(type), height_(height), pressure_(pressure), pumpOn_(pumpOn), valveOpen_(valveOpen) {}
    double height() const { return height_; }
    double pressure() const { return pressure_; }
    bool pumpOn() const { return pumpOn_; }
    bool valveOpen() const { return valveOpen_; }

   protected:
    double height_, pressure_;
    bool pumpOn_, valveOpen_;
};

class CommunicationBase {
   public:
    virtual std::vector<std::unique_ptr<Input>> getInputs() = 0;
};
}  // namespace rd

#endif /* COMMUNICATIONBASE_H */
