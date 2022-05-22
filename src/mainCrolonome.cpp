#include <unistd.h>

#include <iostream>

//#include "Coin/Behavior/Slave.h"
#include <chrono>
#include <random>

#include "Coin/Actuator/Arm.h"
#include "Coin/Actuator/Finger.h"
#include "Coin/Actuator/HMI.h"
#include "Coin/Actuator/Hat.h"
#include "Coin/Actuator/StackManager.h"
#include "Coin/Behavior/Match/StratCrolonome1.h"
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
        200.,      // maxLinearAcceleration
        1000.,     // maxLinearSpeed
        0.5,       // maxRotationalAcceleration
        1.5,       // maxRotationalSpeed
        5.,        // admittedLinearPositionError
        0.045,     // admittedAnglePositionError
        40.,       // minLinearSpeed
        M_PI / 8.  // minRotationalSpeed
    };

    rd::Robot crolonome(motorDucklink, ioDucklink, lidarDucklink, robotParams, true, table);
    rd::ActionJuggler behavior = rd::createStratCrolonome1(crolonome, table);

    std::default_random_engine generator;
    std::chrono::steady_clock::time_point lastControl = std::chrono::steady_clock::now();
    // std::chrono::steady_clock::time_point lastSimu = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastBehavior = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    // rd::Robot mainRobot;
    //  rd::Slave behavior(mainRobot);

    while (true) {
        crolonome.sense();

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
            /*for (const auto a : crolonome.locomotion.getAdversaries()) {
                std::cout << a.second << std::endl;
            }*/
            // actionJuggler.tick();
            lastBehavior = now;
        }

        double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastControl).count() / 1000000.;
        rd::Speed speedCmd;
        if (dt > 0.1) {
            /*if (dt > 5.10) {
                std::cout << "Position Control loop missed by: " << dt - 5. << "s" << std::endl;
            }*/
            if (dtBehavior >= 0.1 * 1.2) {
                std::cout << "Warning behavior loop exceeded by " << dtBehavior - 0.1 << "s" << std::endl;
            }
            lastControl = now;
            // std::cout << "dt: " << dt << std::endl;
            // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            speedCmd = crolonome.locomotion.run(dt);
            if (crolonome.locomotion.isGoalReached()) {
                if (state == 0) {
                    crolonome.locomotion.goToPointHolonomic({1777.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 1) {
                    crolonome.locomotion.goToPointHolonomic({1777.5, 50., -150. * M_PI / 180.});
                    state++;
                } else if (state == 2) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({1593., 230, -150. * M_PI / 180.});
                    state++;
                } else if (state == 3) {
                    crolonome.locomotion.goToPointHolonomic({1593., 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                } else if (state == 4) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({1407.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 5) {
                    crolonome.locomotion.goToPointHolonomic({1407.5, 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                }

                else if (state == 6) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({1222.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 7) {
                    crolonome.locomotion.goToPointHolonomic({1222.5, 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                } else if (state == 8) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({1037.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 9) {
                    crolonome.locomotion.goToPointHolonomic({1037.5, 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                } else if (state == 10) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({852.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 11) {
                    crolonome.locomotion.goToPointHolonomic({852.5, 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                } else if (state == 12) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({667.5, 230., -150. * M_PI / 180.});
                    state++;
                } else if (state == 13) {
                    crolonome.locomotion.goToPointHolonomic({667.5, 50., -150. * M_PI / 180.});
                    ioDucklink.sendFingerCommand(0);
                    state++;
                } else if (state == 14) {
                    crolonome.locomotion.forceRobotPose({crolonome.locomotion.robotPose().x(), 90., -150. * M_PI / 180.});
                    if (crolonome.finger.isTouching() == rd::Finger::YELLOW) {
                        crolonome.finger.deployFinger();
                    }
                    crolonome.locomotion.goToPointHolonomic({320., 420, -70. * M_PI / 180.});
                    //                    280, 360

                    state++;
                } else if (state == 15) {
                    crolonome.statuetteArm.halfDeployArm();
                    crolonome.statuetteArm.magnet(true);
                    crolonome.locomotion.goToPointHolonomic({280., 380., -70. * M_PI / 180.});
                    state++;
                } else if (state == 16) {
                    crolonome.locomotion.goToPointHolonomic({336.5, 333.5, -70. * M_PI / 180.});
                    state++;
                } else if (state == 17) {
                    crolonome.replicaHolder.release();
                    crolonome.locomotion.goToPointHolonomic({376.5, 373.5, -70. * M_PI / 180.});
                    state++;
                } else if (state == 18) {
                    crolonome.replicaHolder.hold();
                    crolonome.locomotion.goToPointHolonomic({270., 1680., 150. * M_PI / 180.});
                    state++;
                } else if (state == 19) {
                    crolonome.statuetteArm.deployArm();
                    crolonome.locomotion.goToPointHolonomic({270., 1820., 150. * M_PI / 180.});
                    state++;
                } else if (state == 20) {
                    crolonome.statuetteArm.magnet(false);
                    crolonome.locomotion.goToPointHolonomic({300, 1500., 0.});
                    state++;
                } else if (state == 21) {
                    crolonome.statuetteArm.retractArm();
                    state++;
                }
            }
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
            udpClientJugglerPlot.sendSpeedJson(crolonome.locomotion.robotSpeed(), "robot_speed");
            udpClientJugglerPlot.sendPointOrientedJson(crolonome.locomotion.robotPose(), "robot_pose");
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
