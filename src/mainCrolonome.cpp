#include <unistd.h>

#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>
#include <random>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/HMI.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Behavior/Match/Strat1.h"
#include "Coin/Communication/Ducklink/UDPDucklinkCommunication.h"
#include "Coin/Communication/UDPJson.h"
#include "Coin/Locomotion/Locomotion.h"
#include "Coin/Table/Table.h"
#include "GeometryTools/Point.h"
#include "GeometryTools/Trajectory.h"

int main(int, char**) {
    std::cout << "Coin coin" << std::endl;

    rd::Table table;

    // rd::IvyHandler ivyHandler;
    // rd::SerialDucklink serialDucklink("/dev/ttyUSB0", 57600);
    rd::UDPDucklink motorDucklink("127.0.0.1", 3456);
    rd::UDPDucklink ioDucklink("127.0.0.1", 3457);
    rd::UDPJson udpClientJugglerPlot("192.168.42.190", 9870);
    rd::UDPDucklink lidarDucklink("127.0.0.1", 4321);
    // rd::UDPDucklink udpClientAnatidae("127.0.0.1", 8888);
    //  rd::UDPDucklinkInput udpClientAnatidaeServer("0.0.0.0", 9999);

    rd::PositionControlParameters robotParams = {
        100.,      // maxLinearAcceleration
        400.,      // maxLinearSpeed
        2.0,       // maxRotationalAcceleration
        2.5,       // maxRotationalSpeed
        5.,        // admittedLinearPositionError
        0.045,     // admittedAnglePositionError
        40.,       // minLinearSpeed
        M_PI / 8.  // minRotationalSpeed
    };

    rd::Robot dalek(motorDucklink, ioDucklink, lidarDucklink, robotParams);
    rd::ActionJuggler actionJuggler = rd::createStrat1(dalek, table);

    std::default_random_engine generator;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point lastSimu = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastBehavior = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    while (true) {
        dalek.sense();
        // Update sensor values
        /*for (const auto& input : udpClientAnatidaeServer.getInputs()) {
            std::cout << input->type() << std::endl;
        }*/
        /*        for (const auto& input : ivyHandler.getInputs()) {
                    if (input->type() == rd::eInput::GO_TO_POINT) {
                        // auto& msg = static_cast<rd::PointInput&>(*Input);
                        //  behavior.newTargetPoint(msg.getPoint());
                    } else if (input->type() == rd::eInput::GO_TO_POINT_ORIENTED) {
                        // auto& msg = static_cast<rd::PointOrientedInput&>(*Input);
                        //  behavior.newTargetOrientedPoint(msg.getPoint());
                    }
                }*/

        /*        for (const auto& input : serialDucklink.getInputs()) {
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

        double dtBehavior = std::chrono::duration_cast<std::chrono::microseconds>(now - lastBehavior).count() / 1000000.;
        if (dtBehavior >= 0.2) {
            if (dtBehavior >= 0.2 * 1.2) {
                std::cout << "Warning behavior loop exceeded by " << dtBehavior - 0.2 << "s" << std::endl;
            }
            actionJuggler.tick();
            lastBehavior = now;
        }

        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastControl).count() / 1000000.;
        rd::Speed speedCmd;
        if (dt > 0.05) {
            /*if (dt > 5.10) {
                std::cout << "Position Control loop missed by: " << dt - 5. << "s" << std::endl;
            }*/
            if (dtBehavior >= 0.1 * 1.2) {
                std::cout << "Warning behavior loop exceeded by " << dtBehavior - 0.1 << "s" << std::endl;
            }
            lastControl = now;
            // std::cout << "dt: " << dt << std::endl;
            // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            speedCmd = dalek.locomotion.run(dt);
            // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            // double exet = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.;
            // std::cout << "exec time: " << exet << std::endl;
            // udpClientJugglerPlot.sendSpeedJson(speedCmd, "speed_cmd");

            // std::cout << robotSpeed << std::endl;
            // locomotion.updateRobotSpeed(rd::SpeedInput(rd::eInput::SPEED_REPORT, speedCmd));

            /*std::cout << "Robot Pose: " << robotPose << std::endl;
            std::cout << "Speed: " << robotSpeed << std::endl;
            std::cout << "Speed Command: " << speedCmd << std::endl;*/
            udpClientJugglerPlot.sendSpeedJson(speedCmd, "speed_cmd");
            udpClientJugglerPlot.sendSpeedJson(dalek.locomotion.robotSpeed(), "robot_speed");
            udpClientJugglerPlot.sendPointOrientedJson(dalek.locomotion.robotPose(), "robot_pose");
        }
        /*double dtSimu = std::chrono::duration_cast<std::chrono::microseconds>(now - lastSimu).count() / 1000000.;
                if (dtSimu > 0.01) {
                    rd::Speed robotSpeed = dalek.locomotion.robotSpeed();
                    double vxNoise = robotSpeed.vx() < 20. ? 0. : std::normal_distribution<double>(0.0, 20.0)(generator);
                    double vthetaNoise = std::normal_distribution<double>(0.0, 0.001)(generator);
                    lastSimu = now;
                    robotSpeed = speedCmd * 0.6 + robotSpeed * 0.4 + rd::Speed(vxNoise, 0, vthetaNoise);
                    // robotSpeed = rd::Speed(robotSpeed.vx(), robotSpeed.vy(), std::max(-1.5, std::min(1.5, robotSpeed.vtheta())));
                    rd::PointOriented robotPose = dalek.locomotion.robotPose();
                    robotPose += rd::PointOriented((robotSpeed.vx() + 0) * robotPose.theta().cos() * dtSimu - robotSpeed.vy() * robotPose.theta().sin() *
           dtSimu, (robotSpeed.vx() + 0) * robotPose.theta().sin() * dtSimu + robotSpeed.vy() * robotPose.theta().cos() * dtSimu, (robotSpeed.vtheta() + 0) *
           dtSimu); dalek.locomotion.updateRobotSpeed(rd::SpeedInput(rd::eInput::SPEED_REPORT, robotSpeed));
                    dalek.locomotion.updateRobotPose(rd::PointOrientedInput(rd::eInput::POSITION_REPORT, robotPose));
                    // udpClientJugglerPlot.sendSpeedJson(robotSpeed, "robot_speed");
                    udpClientJugglerPlot.sendPointOrientedJson(robotPose);

                    // udpClientAnatidae.sendPoseReport(robotPose);
                }*/

        // Abstract these values

        // Decide
        // behavior.tick();

        // Act
        // mainRobot.controlPosition();
        usleep(1);
    }
}
