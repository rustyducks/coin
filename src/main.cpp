#include <unistd.h>

#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>
#include <random>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Communication/Ducklink/UDPDucklinkCommunication.h"
#include "Coin/Communication/UDPJson.h"
#include "Coin/Table/Table.h"
#include "GeometryTools/Point.h"
#include "Navigation/PurePursuitControl.h"

int main(int, char**) {
    std::cout << "Coucou" << std::endl;

    rd::Table table;

    // rd::IvyHandler ivyHandler;
    //  rd::SerialDucklink serialDucklink("/dev/ttyUSB0", 57600);
    //  rd::SerialDucklink motorDucklink("/dev/ttyUSB0", 57600);
    rd::UDPJson udpClientJugglerPlot("127.0.0.1", 9870);
    rd::UDPDucklinkOutput udpClientAnatidae("127.0.0.1", 8888);
    rd::UDPDucklinkInput udpClientAnatidaeServer("0.0.0.0", 9999);

    rd::PointOriented robotPose({1500., 1000., 0.});
    // rd::Arm arm(serialDucklink);
    const rd::PositionControlParameters robotParams = {
        100.,      // maxLinearAcceleration
        300.,      // maxLinearSpeed
        1.0,       // maxRotationalAcceleration
        2.0,       // maxRotationalSpeed
        5.,        // admittedLinearPositionError
        0.045,     // admittedAnglePositionError
        40.,       // minLinearSpeed
        M_PI / 8.  // minRotationalSpeed
    };
    std::default_random_engine generator;

    rd::PurePursuitControl pp(robotParams, 2, 100.);
    // rd::Path path = rd::Trajectory::lissajouPath(robotPose, 200, 750, 500);
    // m 1500,1000 c -402.1505,-531.48472 -843.09906,340.3162 -213.2185,377.6856 784.9207,46.5676 -190.6885,833.5373 722.989,846.7716
    // rd::Path path1 = rd::Trajectory::cubicBezier(robotPose, {1098., 468.5}, {657., 1340.}, {1287., 1377., 0.}, 500);
    // rd::Path path2 = rd::Trajectory::cubicBezier(path1.last(), {2072., 1424.}, {1096., 2211.}, {2010., 2224., 0.}, 500);
    //
    // rd::Path path = path1 + path2;
    rd::Path path = rd::Path::fromSVG("../dessin.svg", 100);
    std::cout << path.at(path.size() - 1) << std::endl;
    rd::Trajectory traj = path.computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 50., robotParams.maxLinearAcceleration);
    // rd::Trajectory traj = rd::Path({{0.0, 0.0, 0.0}, {800.0, 0.0, 0.0}, {800.0, 800.0, 0.0}, {0.0, 800.0, 0.0}, {0.0, 0.0, 0.0}})
    //                           .computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 0, robotParams.maxLinearAcceleration);
    pp.setTrajectory(traj);
    rd::Speed robotSpeed;
    rd::Speed speedCmd;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastSimu = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    while (true) {
        // Update sensor values
        for (const auto& input : udpClientAnatidaeServer.getInputs()) {
            std::cout << input->type() << std::endl;
        }
        /*        for (const auto& input : ivyHandler.getInputs()) {
                    if (input->type() == rd::eInput::GO_TO_POINT) {
                        // auto& msg = static_cast<rd::PointInput&>(*Input);
                        //  behavior.newTargetPoint(msg.getPoint());
                    } else if (input->type() == rd::eInput::GO_TO_POINT_ORIENTED) {
                        // auto& msg = static_cast<rd::PointOrientedInput&>(*Input);
                        //  behavior.newTargetOrientedPoint(msg.getPoint());
                    }
                }*/
        /*        for (const auto& input : serialMotor.getInputs()) {
                    if (input->type() == rd::eInput::POSITION) {
                        auto& msg = static_cast<rd::PointOrientedInput&>(*input);
                        robotPose = msg.getPoint();
                    } else if (input->type() == rd::eInput::SPEED) {
                        // auto& msg = static_cast<rd::SpeedInput&>(*Input);
                        // robotSpeed = msg.getSpeed();
                    } else if (input->type() == rd::eInput::ARM_STATUS) {
                        auto& msg = static_cast<rd::ArmInput&>(*input);
                        arm.updateState(msg);
                    } else if (input->type() == rd::eInput::HAT_STATUS) {
                    }
                }
                for (const auto& input : serialDucklink.getInputs()) {
                    if (input->type() == rd::eInput::POSITION) {
                        auto& msg = static_cast<rd::PointOrientedInput&>(*input);
                        robotPose = msg.getPoint();
                    } else if (input->type() == rd::eInput::SPEED) {
                        // auto& msg = static_cast<rd::SpeedInput&>(*Input);
                        // robotSpeed = msg.getSpeed();
                    } else if (input->type() == rd::eInput::ARM_STATUS) {
                        auto& msg = static_cast<rd::ArmInput&>(*input);
                        arm.updateState(msg);
                    } else if (input->type() == rd::eInput::HAT_STATUS) {
                    }
                } */

        now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastControl).count() / 1000000.;
        if (dt > 0.1) {
            /*if (dt > 5.10) {
                std::cout << "Position Control loop missed by: " << dt - 5. << "s" << std::endl;
            }*/
            lastControl = now;
            // std::cout << "dt: " << dt << std::endl;
            // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            speedCmd = pp.computeSpeed(robotPose, robotSpeed, dt);
            // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            // double exet = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.;
            // std::cout << "exec time: " << exet << std::endl;
            udpClientJugglerPlot.sendSpeedJson(speedCmd, "speed_cmd");

            // std::cout << robotSpeed << std::endl;

            /*serialDucklink.sendSpeed(robotSpeed);
            std::cout << "Robot Pose: " << robotPose << std::endl;
            std::cout << "Speed: " << robotSpeed << std::endl;
            std::cout << "Speed Command: " << speedCmd << std::endl;*/
            // arm.setJoint(rd::Arm::eJoint::Z_ROTATIONAL, joints[i]);
            // i = (i + 1) % 3;
        }
        double dtSimu = std::chrono::duration_cast<std::chrono::microseconds>(now - lastSimu).count() / 1000000.;
        if (dtSimu > 0.02) {
            double vxNoise = robotSpeed.vx() < 40. ? 0. : std::normal_distribution<double>(0.0, 1.0)(generator);
            double vthetaNoise = std::normal_distribution<double>(0.0, 0.001)(generator);
            lastSimu = now;
            robotSpeed = speedCmd * 0.9 + robotSpeed * 0.1 + rd::Speed(vxNoise, 0, vthetaNoise);
            // robotSpeed = rd::Speed(robotSpeed.vx(), robotSpeed.vy(), std::max(-1.5, std::min(1.5, robotSpeed.vtheta())));
            robotPose += rd::PointOriented((robotSpeed.vx() + 0) * robotPose.theta().cos() * dtSimu - robotSpeed.vy() * robotPose.theta().sin() * dtSimu,
                                           (robotSpeed.vx() + 0) * robotPose.theta().sin() * dtSimu + robotSpeed.vy() * robotPose.theta().cos() * dtSimu,
                                           (robotSpeed.vtheta() + 0) * dtSimu);
            udpClientJugglerPlot.sendSpeedJson(robotSpeed, "robot_speed");
            udpClientJugglerPlot.sendPointOrientedJson(robotPose);
            udpClientAnatidae.sendPoseReport(robotPose);
        }

        // Abstract these values

        // Decide
        // behavior.tick();

        // Act
        // mainRobot.controlPosition();
        usleep(5);
    }
}