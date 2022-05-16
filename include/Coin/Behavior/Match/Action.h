#ifndef ACTION_H
#define ACTION_H

#include <memory>

#include "Coin/Robot.h"
#include "GeometryTools/Trajectory.h"

namespace rd {

class Action;

typedef std::shared_ptr<Trajectory> TrajectoryPtr;
typedef std::shared_ptr<Action> ActionPtr;

class Action {
   protected:
    enum eIntegrityCheck { OK, WARNING, ERROR };
    std::string name_;
    TrajectoryPtr goToApproachTraj_;
    TrajectoryPtr retractTraj_;

    ActionPtr onGoToBlocked_;

    double blockedDuration_;

   public:
    // Action(Trajectory& goToApproach, const Point& entryPoint, ActionFSM& actionFSM, const Trajectory& retractTrajectory);
    Action(const std::string& name, TrajectoryPtr goTo, TrajectoryPtr retract);

    virtual ActionPtr onGoToBlocked() { return onGoToBlocked_; }
    virtual void setOnGoToBlocked(ActionPtr onGoToBlocked) { onGoToBlocked_ = onGoToBlocked; }

    const TrajectoryPtr getGoTo() { return goToApproachTraj_; }
    const TrajectoryPtr getRetract() { return retractTraj_; }

    double blockedDuration() { return blockedDuration_; }
    void setBlockedDuration(double blockedDuration) { blockedDuration_ = blockedDuration; }

    virtual ActionPtr run(Robot& robot) = 0;
    virtual void deinit(Robot& robot) = 0;
    virtual bool isDeinit(Robot& robot) = 0;

    virtual Action::eIntegrityCheck checkIntegrity() = 0;

    void logIntegrity(const eIntegrityCheck severity, const std::string& message) {
        std::string prepend = severity == OK ? "\033[32mINFO" : (severity == WARNING ? "\033[33mWARNING" : "\033[31mERROR");
        std::cout << prepend << ": [" << name_ << "] " << message << "\033[0m" << std::endl;
    }

    eIntegrityCheck assembleIntegrities(const eIntegrityCheck check1, const eIntegrityCheck check2) {
        if (check1 == eIntegrityCheck::ERROR || check2 == eIntegrityCheck::ERROR) {
            return eIntegrityCheck::ERROR;
        }
        if (check1 == eIntegrityCheck::WARNING || check2 == eIntegrityCheck::WARNING) {
            return eIntegrityCheck::WARNING;
        }
        return eIntegrityCheck::OK;
    }
};

}  // namespace rd

#endif /* ACTION_H */
