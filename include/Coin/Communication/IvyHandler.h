#ifndef IVYHANDLER_H
#define IVYHANDLER_H

#include "Coin/Communication/CommunicationBase.h"
#include "Navigation/Communication/Ivy.h"

namespace rd {
class IvyHandler : public CommunicationBase {
   public:
    IvyHandler();

    virtual std::vector<std::unique_ptr<Input>> getInputs() override;

    void onGoToOrient(const PointOriented& point);
    void onGoToPoint(const Point& point);

   protected:
    std::vector<PointOrientedInput> pointOrientedInputs_;
    std::vector<PointInput> pointInputs_;
    Ivy& ivy_;
};
}  // namespace rd

#endif /* IVYHANDLER_H */
