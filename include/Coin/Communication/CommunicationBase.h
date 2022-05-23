#ifndef COMMUNICATIONBASE_H
#define COMMUNICATIONBASE_H

#include <chrono>
#include <memory>
#include <vector>

#include "Ducklink/messages.pb.h"
#include "GeometryTools/Point.h"
#include "GeometryTools/Speed.h"

namespace rd {

enum eInput {
    GO_TO_POINT,
    GO_TO_POINT_ORIENTED,
    POSITION_REPORT,
    SPEED_REPORT,
    ARM_STATUS,
    HAT_STATUS,
    SPEED_COMMAND,
    LIDAR_ADVERSARIES,
    PROCEDURE_STATUS,
    HMI_REPORT,
    RESISTOR_REPORT
};

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

class PointOrientedInputWithTimestamp : public Input {
   public:
    PointOrientedInputWithTimestamp(eInput type, const PointOriented& point, const std::chrono::system_clock::time_point& timestamp)
        : Input(type), point_(point), timestamp_(timestamp) {}
    const PointOriented& getPoint() const { return point_; }
    const std::chrono::system_clock::time_point& timestamp() const { return timestamp_; }

   protected:
    PointOriented point_;
    std::chrono::system_clock::time_point timestamp_;
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
    ArmInput(eInput type, const unsigned int armId, const double zPrismatic, const double zRotational, const double yRotational, const bool pumpOn,
             const bool valveOpen, const double pressure)
        : Input(type),
          armId_(armId),
          zPrismatic_(zPrismatic),
          zRotational_(zRotational),
          yRotational_(yRotational),
          pressure_(pressure),
          pumpOn_(pumpOn),
          valveOpen_(valveOpen) {}
    unsigned int armId() const { return armId_; }
    double zPrismatic() const { return zPrismatic_; }
    double zRotational() const { return zRotational_; }
    double yRotational() const { return yRotational_; }
    double pressure() const { return pressure_; }
    bool pumpOn() const { return pumpOn_; }
    bool valveOpen() const { return valveOpen_; }

   protected:
    unsigned int armId_;
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

class LidarAdversaries : public Input {
   public:
    LidarAdversaries(eInput type, const std::vector<std::pair<int, Point>> adversaries) : Input(type), adversaries_(adversaries) {}
    std::vector<std::pair<int, Point>> adversaries_;
};

class ProcedureInput : public Input {
   public:
    enum eStatus {
        RUNNING,
        RUNNING_FREE,
        SUCCESS,
        FAILURE,
        POSITION_UNREACHABLE,
        UNABLE_VACUUM,
    };
    ProcedureInput(eInput type, const protoduck::Procedure_Status status) : Input(type) {
        switch (status) {
            case protoduck::Procedure_Status::Procedure_Status_RUNNING:
                status_ = eStatus::RUNNING;
                break;
            case protoduck::Procedure_Status::Procedure_Status_SUCCESS:
                status_ = eStatus::SUCCESS;
                break;
            case protoduck::Procedure_Status::Procedure_Status_FAILURE:
                status_ = eStatus::FAILURE;
                break;
            case protoduck::Procedure_Status::Procedure_Status_POSITION_UNREACHABLE:
                status_ = eStatus::POSITION_UNREACHABLE;
                break;
            case protoduck::Procedure_Status::Procedure_Status_UNABLE_VACUUM:
                status_ = eStatus::UNABLE_VACUUM;
                break;
            case protoduck::Procedure_Status::Procedure_Status_RUNNING_FREE:
                status_ = eStatus::RUNNING_FREE;
                break;
            case protoduck::Procedure_Status::Procedure_Status_Procedure_Status_INT_MAX_SENTINEL_DO_NOT_USE_:
                break;
            case protoduck::Procedure_Status::Procedure_Status_Procedure_Status_INT_MIN_SENTINEL_DO_NOT_USE_:
                break;
        }
    }
    eStatus status_;
};

class ResistorInput : public Input {
   public:
    ResistorInput(eInput type, const float resistorValue) : Input(type), resistorValue(resistorValue) {}
    float resistorValue;
};

class HMIInput : public Input {
   public:
    HMIInput(bool button, bool color, uint32_t scoreDisplay, uint32_t led, bool tirette)
        : Input(HMI_REPORT), button_(button), color_(color), tirette_(tirette), scoreDisplay_(scoreDisplay), led_(led) {}
    bool button_, color_, tirette_;
    uint32_t scoreDisplay_, led_;
};

class CommunicationInputBase {
   public:
    virtual std::vector<std::unique_ptr<Input>> getInputs() = 0;
};

class CommunicationOutputBase {
   public:
    virtual void sendSpeed(const Speed& speed) = 0;
    virtual void sendPoseReport(const PointOriented& pose) = 0;
    virtual void sendPoseCommand(const PointOriented& pose) = 0;
};

class ArmCommandSenderInterface {
   public:
    virtual void sendArmCommand(const unsigned int armId, const double zPrismatic, const double zRotational, const double yRotational, const bool pumpEnabled,
                                const bool valveOpen) = 0;
};
class HatCommandSenderInterface {
   public:
    virtual void sendHatCommand(const double height, const bool pumpEnabled, const bool valveOpen) = 0;
};

class ProcedureCommandSenderInterface {
   public:
    virtual void sendProcedureCommand(const unsigned int armId, const protoduck::Procedure_Proc procedure, const int param) = 0;
};

class HMICommandSenderInterface {
   public:
    virtual void sendHMICommand(const uint32_t scoreDisplay, const uint32_t led) = 0;
};

class FingerCommandSenderInterface {
   public:
    virtual void sendFingerCommand(const int command) = 0;
};

}  // namespace rd

#endif /* COMMUNICATIONBASE_H */
