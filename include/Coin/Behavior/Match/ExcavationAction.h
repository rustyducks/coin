#ifndef EXCAVATIONACTION_H
#define EXCAVATIONACTION_H

#include "Coin/Behavior/Match/Action.h"
#include "Coin/Behavior/Timer.h"
#include "Coin/Table/ExcavationSquare.h"

namespace rd {

class ExcavationAction;

typedef std::shared_ptr<ExcavationAction> ExcavationActionPtr;

class ExcavationAction : public Action {
   public:
    ExcavationAction(ExcavationSquarePtr square);
    virtual ActionPtr run(Robot& robot) override;
    virtual void deinit(Robot& robot) override;
    virtual bool isDeinit(Robot& robot) override;

    void setNextSquares(const std::vector<ExcavationActionPtr>& nexts) { nextSquares_ = nexts; }
    void setOnSuccess(const ActionPtr onSuccess) { onSuccess_ = onSuccess; }
    virtual ActionPtr onGoToBlocked(Robot& robot) override;

    eIntegrityCheck checkIntegrity() override;
    const ExcavationSquarePtr square() { return square_; }

   protected:
    ActionPtr getNextSquare(Robot::eColor color) const;

    enum eState { IDLE, APPROACHING, RETURNING, DONE };
    eState state_;
    ExcavationSquarePtr square_;
    std::vector<ExcavationActionPtr> nextSquares_;
    ActionPtr onSuccess_;  // What to do when no more squares are to flip
    Timer colorDetectionTimeout_;
    Timer fingerDeployWait_;
};
}  // namespace rd

#endif /* EXCAVATIONACTION_H */
