#ifndef IVYHANDLER_H
#define IVYHANDLER_H

#include "Coin/Communication/CommunicationBase.h"
#include "Navigation/Communication/Ivy.h"

namespace rd {
class IvyHandler : public CommunicationInputBase, public CommunicationOutputBase {
   public:
    IvyHandler();

    virtual std::vector<std::unique_ptr<Input>> getInputs() override;

    void onGoToOrient(const PointOriented& point);
    void onGoToPoint(const Point& point);
    virtual void sendSpeed(const Speed& speed) override;
    virtual void sendPoseReport(const PointOriented& pose) override;

   protected:
    std::vector<PointOrientedInput> pointOrientedInputs_;
    std::vector<PointInput> pointInputs_;
    Ivy& ivy_;
};
}  // namespace rd

#endif /* IVYHANDLER_H */
