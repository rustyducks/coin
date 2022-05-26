#ifndef ACTION_H
#define ACTION_H

#include <memory>

#include "Coin/Robot.h"
#include "GeometryTools/Trajectory.h"

namespace rd {

class Action;

typedef std::shared_ptr<Path> PathPtr;
typedef std::shared_ptr<Action> ActionPtr;
typedef std::shared_ptr<PointOriented> PointOrientedPtr;

class Action {
   protected:
    std::string name_;
    PointOrientedPtr goToPoint_;
    PointOrientedPtr retractPoint_;

    ActionPtr onGoToBlocked_;

    double blockedDuration_;

    bool isIntegrityChecked_;

   public:
    enum eIntegrityCheck { OK, WARNING, ERROR };
    // Action(Trajectory& goToApproach, const Point& entryPoint, ActionFSM& actionFSM, const Trajectory& retractTrajectory);
    Action(const std::string& name, const PointOrientedPtr& goTo, const PointOrientedPtr& retract);

    virtual ActionPtr onGoToBlocked(Robot&) { return onGoToBlocked_; }
    virtual void setOnGoToBlocked(ActionPtr onGoToBlocked) { onGoToBlocked_ = onGoToBlocked; }

    virtual bool goTo(const PointOriented& robotPose, Robot& robot);
    virtual bool retract(const PointOriented& robotPose, Robot& robot);

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

    const std::string& name() { return name_; }
};

}  // namespace rd

#endif /* ACTION_H */
