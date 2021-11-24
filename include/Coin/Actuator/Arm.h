#ifndef ARM_H
#define ARM_H

#include <unordered_map>

#include "Coin/Actuator/SerialDucklinkActuator.h"

namespace rd {
class Arm : public SerialDucklinkActuator {
   public:
    Arm(SerialDucklink& ducklink);

    enum eJoint { Z_PRISMATIC, Z_ROTATIONAL, Y_ROTATIONAL };
    void setJoint(const eJoint& joint, const double value);
    double getJoint(const eJoint& joint) const { return position_.at(joint); }
    void startPump(bool enable = true);
    void openValve(bool open = true);
    double pressure() const { return pressure_; }

    void updateState(const ArmInput& input);

   protected:
    struct sArmCommand {
        std::unordered_map<eJoint, double> position;
        bool pumpEnabled;
        bool valveOpen;
    };
    sArmCommand lastCommand_;
    std::unordered_map<eJoint, double> position_;
    bool pumpEnabled_;
    bool valveOpen_;
    double pressure_;
};
}  // namespace rd

#endif /* ARM_H */
