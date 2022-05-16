#ifndef ACTIONFSM_H
#define ACTIONFSM_H

#include <memory>

#include "Coin/Robot.h"

namespace rd {
class Action;

class ActionFSM {
   public:
    virtual std::shared_ptr<Action> run(Robot& robot) = 0;
};

}  // namespace rd

#endif /* ACTIONFSM_H */
