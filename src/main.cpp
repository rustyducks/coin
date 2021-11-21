#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>

#include "Coin/Actuator/Arm.h"
#include "Coin/Communication/Ducklink/SerialDucklink.h"
#include "Coin/Communication/IvyHandler.h"
#include "Navigation/PurePursuitControl.h"

int main(int, char**) {
    std::cout << "Coucou" << std::endl;

    rd::IvyHandler ivyHandler;
    rd::SerialDucklink serialDucklink("/dev/ttyUSB0", 57600);
    rd::Arm arm(serialDucklink);
    rd::PositionControlParameters robotParams = {
        100.,  // maxLinearAcceleration
        300.,  // maxLinearSpeed
        0.5,   // maxRotationalAcceleration
        1.5,   // maxRotationalSpeed
        5.,    // admittedLinearPositionError
        0.05   // admittedAnglePositionError
    };

    rd::PurePursuitControl pp(robotParams, 2, 100.);
    // rd::Trajectory traj = rd::Trajectory::lissajouPath({0., 0., 0.}, 200)
    //                           .computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 50., robotParams.maxLinearAcceleration);
    rd::Trajectory traj = rd::Path({{0.0, 0.0, 0.0}, {800.0, 0.0, 0.0}, {800.0, 800.0, 0.0}, {0.0, 800.0, 0.0}, {0.0, 0.0, 0.0}})
                              .computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 0, robotParams.maxLinearAcceleration);
    for (size_t i = 0; i < traj.size(); i++) {
        std::cout << traj.at(i).speed() << std::endl;
    }
    pp.setTrajectory(traj);
    rd::PointOriented robotPose;
    rd::Speed robotSpeed;
    rd::Speed speedCmd;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    while (true) {
        // Update sensor values
        for (const auto& Input : ivyHandler.getInputs()) {
            if (Input->type() == rd::eInput::GO_TO_POINT) {
                // auto msg = static_cast<rd::PointInput&>(*Input);
                //  behavior.newTargetPoint(msg.getPoint());
            } else if (Input->type() == rd::eInput::GO_TO_POINT_ORIENTED) {
                // auto msg = static_cast<rd::PointOrientedInput&>(*Input);
                //  behavior.newTargetOrientedPoint(msg.getPoint());
            }
        }
        for (const auto& Input : serialDucklink.getInputs()) {
            if (Input->type() == rd::eInput::POSITION) {
                auto msg = static_cast<rd::PointOrientedInput&>(*Input);
                robotPose = msg.getPoint();
            } else if (Input->type() == rd::eInput::SPEED) {
                // auto msg = static_cast<rd::SpeedInput&>(*Input);
                // robotSpeed = msg.getSpeed();
            }
        }

        now = std::chrono::steady_clock::now();
        double joints[3] = {-92., 0., 308.};
        size_t i = 0;
        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastControl).count() / 1000000.;
        if (dt > 3.) {
            if (dt > 5.10) {
                std::cout << "Position Control loop missed by: " << dt - 5. << "s" << std::endl;
            }
            lastControl = now;
            /*std::cout << dt << std::endl;
            robotSpeed = pp.computeSpeed(robotPose, robotSpeed, dt);
            serialDucklink.sendSpeed(robotSpeed);
            std::cout << "Robot Pose: " << robotPose << std::endl;
            std::cout << "Speed: " << robotSpeed << std::endl;
            std::cout << "Speed Command: " << speedCmd << std::endl;*/
            arm.setJoint(rd::Arm::eJoint::Z_ROTATIONAL, joints[i]);
            i = (i + 1) % 3;
        }

        // Abstract these values

        // Decide
        // behavior.tick();

        // Act
        // mainRobot.controlPosition();
        usleep(5);
    }
}