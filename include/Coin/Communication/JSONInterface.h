#ifndef JSONINTERFACE_H
#define JSONINTERFACE_H

#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"

namespace rd {
class JsonInterface {
   public:
    virtual void sendSpeedJson(const Speed &speed, const std::string &name = "speed") = 0;
    virtual void sendPointOrientedJson(const PointOriented &point, const std::string &name = "point_oriented") = 0;
};
}  // namespace rd

#endif /* JSONINTERFACE_H */
