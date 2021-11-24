#ifndef HAT_H
#define HAT_H

#include "Coin/Actuator/SerialDucklinkActuator.h"

namespace rd {
class Hat : public SerialDucklinkActuator {
   public:
    Hat(SerialDucklink& ducklink);
    void setHeight(const double height);
    void startPump(const bool enable = true);
    void openValve(const bool open = true);
    double getHeight() { return height_; }

    void updateState(const HatInput& input);

   protected:
    struct sHatCommand {
        double height;
        bool pumpEnabled;
        bool valveOpen;
    };
    sHatCommand lastCommand_;
    double height_;
    bool pumpEnabled_;
    bool valveOpen_;
    double pressure_;
};
}  // namespace rd

#endif /* HAT_H */
