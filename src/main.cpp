#include <unistd.h>

#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>
#include <random>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Communication/Ducklink/UDPDucklinkCommunication.h"
#include "Coin/Communication/UDPJson.h"
#include "Coin/Locomotion/Locomotion.h"
#include "GeometryTools/Point.h"
#include "GeometryTools/Trajectory.h"

int main(int, char**) {
    std::cout << "Coin coin" << std::endl;

    // rd::IvyHandler ivyHandler;
    // rd::SerialDucklink serialDucklink("/dev/ttyUSB0", 57600);
    rd::UDPDucklink motorDucklink("127.0.0.1", 3456);
    rd::UDPDucklink ioDucklink("127.0.0.1", 3457);
    rd::UDPJson udpClientJugglerPlot("127.0.0.1", 9870);
    rd::UDPDucklink lidarDucklink("127.0.0.1", 4321);
    // rd::UDPDucklink udpClientAnatidae("127.0.0.1", 8888);
    //  rd::UDPDucklinkInput udpClientAnatidaeServer("0.0.0.0", 9999);

    rd::Arm arm1(rd::Arm::ArmID::ARM_1, ioDucklink);
    rd::Arm arm2(rd::Arm::ArmID::ARM_2, ioDucklink);
    rd::Hat hat(ioDucklink);
    rd::StackManager stackman(arm1, arm2, hat, ioDucklink);
    const rd::PositionControlParameters robotParams = {
        200.,      // maxLinearAcceleration
        400.,      // maxLinearSpeed
        2.0,       // maxRotationalAcceleration
        2.5,       // maxRotationalSpeed
        5.,        // admittedLinearPositionError
        0.045,     // admittedAnglePositionError
        40.,       // minLinearSpeed
        M_PI / 8.  // minRotationalSpeed
    };
    rd::Locomotion locomotion(robotParams, motorDucklink);
    locomotion.forceRobotPose({1500., 1000., 0.0});

    std::default_random_engine generator;

    // rd::Path path = rd::Trajectory::lissajouPath(robotPose, 200, 750, 500);
    //  m 1500,1000 c -402.1505,-531.48472 -843.09906,340.3162 -213.2185,377.6856 784.9207,46.5676 -190.6885,833.5373 722.989,846.7716
    rd::Path path1 = rd::Trajectory::cubicBezier(locomotion.robotPose(), {1098., 468.5}, {657., 1340.}, {1287., 1377., 0.}, 100);
    rd::Path path2 = rd::Trajectory::cubicBezier(path1.last(), {2072., 1424.}, {1096., 2211.}, {2010., 2224., 0.}, 100);
    //
    rd::Path path = path1 + path2;
    //  rd::Path path = rd::Path::fromSVG("./dessin.svg", 100);
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path.at(i) << std::endl;
    }
    std::cout << path.at(path.size() - 1) << std::endl;
    rd::Trajectory traj = path.computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 50., robotParams.maxLinearAcceleration);
    // rd::Trajectory traj = rd::Path({{0.0, 0.0, 0.0}, {800.0, 0.0, 0.0}, {800.0, 800.0, 0.0}, {0.0, 800.0, 0.0}, {0.0, 0.0, 0.0}})
    //                           .computeSpeeds(robotParams.maxLinearSpeed, robotParams.maxRotationalSpeed, 0, robotParams.maxLinearAcceleration);
    locomotion.followTrajectory(traj);
    rd::Speed speedCmd;
    rd::Point adversaryPose;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastSimu = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    arm2.startPump(false);
    while (true) {
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
        for (const auto& input : motorDucklink.getInputs()) {
            if (input->type() == rd::eInput::POSITION_REPORT) {
                auto& msg = static_cast<rd::PointOrientedInput&>(*input);
                locomotion.updateRobotPose(msg);
                std::cout << "robotPose: " << locomotion.robotPose() << std::endl;
            } else if (input->type() == rd::eInput::SPEED_REPORT) {
                auto& msg = static_cast<rd::SpeedInput&>(*input);
                locomotion.updateRobotSpeed(msg);
                std::cout << "speedreport: " << locomotion.robotSpeed() << std::endl;
            }
        }
        for (const auto& input : ioDucklink.getInputs()) {
            if (input->type() == rd::eInput::ARM_STATUS) {
                auto& msg = static_cast<rd::ArmInput&>(*input);
                if (msg.armId() == 0) {
                    arm1.updateState(msg);
                } else {
                    arm2.updateState(msg);
                }
            } else if (input->type() == rd::eInput::HAT_STATUS) {
                auto& msg = static_cast<rd::HatInput&>(*input);
                hat.updateState(msg);
            } else if (input->type() == rd::eInput::PROCEDURE_STATUS) {
            }
        }
        for (const auto& input : lidarDucklink.getInputs()) {
            std::cout << "Received" << std::endl;
            if (input->type() == rd::eInput::LIDAR_ADVERSARIES) {
                auto& msg = static_cast<rd::LidarAdversaries&>(*input);
                locomotion.updateAdversaries(msg);
                std::cout << "Adversaries: " << std::endl;
                for (const auto& a : msg.adversaries_) {
                    std::cout << a.first << ": " << a.second << std::endl;
                }
            }
        }
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
        if (dtSimu > 0.01) {
            rd::Speed robotSpeed = locomotion.robotSpeed();
            double vxNoise = robotSpeed.vx() < 20. ? 0. : std::normal_distribution<double>(0.0, 20.0)(generator);
            double vthetaNoise = std::normal_distribution<double>(0.0, 0.001)(generator);
            lastSimu = now;
            robotSpeed = speedCmd * 0.6 + robotSpeed * 0.4 + rd::Speed(vxNoise, 0, vthetaNoise);
            // robotSpeed = rd::Speed(robotSpeed.vx(), robotSpeed.vy(), std::max(-1.5, std::min(1.5, robotSpeed.vtheta())));
            rd::PointOriented robotPose = locomotion.robotPose();
            robotPose += rd::PointOriented((robotSpeed.vx() + 0) * robotPose.theta().cos() * dtSimu - robotSpeed.vy() * robotPose.theta().sin() * dtSimu,
                                           (robotSpeed.vx() + 0) * robotPose.theta().sin() * dtSimu + robotSpeed.vy() * robotPose.theta().cos() * dtSimu,
                                           (robotSpeed.vtheta() + 0) * dtSimu);
            locomotion.updateRobotSpeed(rd::SpeedInput(rd::eInput::SPEED_REPORT, robotSpeed));
            locomotion.updateRobotPose(rd::PointOrientedInput(rd::eInput::POSITION_REPORT, robotPose));
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
