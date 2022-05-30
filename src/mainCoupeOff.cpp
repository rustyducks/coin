#include <unistd.h>

#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>
#include <random>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/HMI.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Behavior/Match/StratDalek1.h"
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
    rd::UDPJson udpClientJugglerPlot("127.0.0.1", 9870);
    rd::UDPDucklink lidarDucklink("127.0.0.1", 4321);
    // rd::UDPDucklink udpClientAnatidae("127.0.0.1", 8888);
    //  rd::UDPDucklinkInput udpClientAnatidaeServer("0.0.0.0", 9999);

    rd::PositionControlParameters robotParams = {
        10.,       // maxLinearAcceleration
        100.,      // maxLinearSpeed
        2.0,       // maxRotationalAcceleration
        2.5,       // maxRotationalSpeed
        5.,        // admittedLinearPositionError
        0.045,     // admittedAnglePositionError
        40.,       // minLinearSpeed
        M_PI / 8.  // minRotationalSpeed
    };

    rd::Locomotion locomotion(robotParams, motorDucklink);
    locomotion.forceRobotPose({181.50854, 42.507328, 0.0});
    rd::Path duck = rd::Path::fromSVG("./dessin.svg", 5000);
    locomotion.followTrajectory(duck.computeSpeeds(100000, 1000000, 100000, 10000));

    std::default_random_engine generator;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastSimu = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    rd::Speed speedCmd;

    std::cout << "GO ? " << std::endl;

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    while (true) {
        /*for (const auto& input : motorDucklink.getInputs()) {
            if (input->type() == rd::eInput::POSITION_REPORT) {
                auto& msg = static_cast<rd::PointOrientedInputWithTimestamp&>(*input);
                locomotion.updateRobotPose(msg);
            } else if (input->type() == rd::eInput::SPEED_REPORT) {
                auto& msg = static_cast<rd::SpeedInput&>(*input);
                locomotion.updateRobotSpeed(msg);
            }
        }*/
        // dalek.sense();
        //  Update sensor values
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

        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastControl).count() / 1000000.;
        if (dt > 0.1) {
            /*if (dt > 5.10) {
                std::cout << "Position Control loop missed by: " << dt - 5. << "s" << std::endl;
            }*/
            if (dt >= 0.1 * 1.2) {
                std::cout << "Warning control loop exceeded by " << dt - 0.1 << "s" << std::endl;
            }
            lastControl = now;
            // std::cout << "dt: " << dt << std::endl;
            // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            speedCmd = locomotion.run(dt);
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
            udpClientJugglerPlot.sendSpeedJson(locomotion.robotSpeed(), "robot_speed");
            udpClientJugglerPlot.sendPointOrientedJson(locomotion.robotPose(), "robot_pose");
        }
        double dtSimu = std::chrono::duration_cast<std::chrono::microseconds>(now - lastSimu).count() / 1000000.;
        if (dtSimu > 0.05) {
            rd::Speed robotSpeed = locomotion.robotSpeed();
            double vxNoise = robotSpeed.vx() < 20. ? 0. : std::normal_distribution<double>(0.0, 20.0)(generator);
            double vthetaNoise = 0;
            lastSimu = now;
            robotSpeed = speedCmd * 0.9 + robotSpeed * 0.1 + rd::Speed(vxNoise, 0, vthetaNoise);
            // robotSpeed = rd::Speed(robotSpeed.vx(), robotSpeed.vy(), std::max(-1.5, std::min(1.5, robotSpeed.vtheta())));
            rd::PointOriented robotPose = locomotion.robotPose();
            std::cout << robotSpeed << std::endl;
            robotPose += rd::PointOriented((robotSpeed.vx() + 0) * robotPose.theta().cos() * dtSimu - robotSpeed.vy() * robotPose.theta().sin() * dtSimu,
                                           (robotSpeed.vx() + 0) * robotPose.theta().sin() * dtSimu + robotSpeed.vy() * robotPose.theta().cos() * dtSimu,
                                           (robotSpeed.vtheta() + 0) * dtSimu);
            std::cout << robotPose << std::endl;
            locomotion.updateRobotSpeed(rd::SpeedInput(rd::eInput::SPEED_REPORT, robotSpeed));
            locomotion.updateRobotPose(rd::PointOrientedInputWithTimestamp(rd::eInput::POSITION_REPORT, robotPose, std::chrono::system_clock::now()));
            // udpClientJugglerPlot.sendSpeedJson(robotSpeed, "robot_speed");
            udpClientJugglerPlot.sendPointOrientedJson(robotPose);

            // udpClientAnatidae.sendPoseReport(robotPose);
        }

        // Abstract these values

        // Decide
        // behavior.tick();

        // Act
        // mainRobot.controlPosition();
        usleep(1);
    }
}
