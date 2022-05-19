#include "Coin/Behavior/Match/ActionJuggler.h"
#include "Coin/Behavior/Match/HexaAction.h"
#include "Coin/Behavior/Match/PreMatchAction.h"
#include "Coin/Table/Table.h"

namespace rd {
ActionJuggler createStrat1(Robot& robot, Table& table) {
    auto nullTraj = [](const PointOriented&) { return nullptr; };

    ActionJuggler juggler(robot);
    auto preMatch = std::make_shared<PreMatchAction>(PointOriented({90., 1440., 0.0}), PointOriented({2910., 1440., M_PI}));
    auto startToYellowFirstBlue = [](const PointOriented& robotPose) {
        return std::make_shared<Path>(Path::cubicBezier(robotPose, {robotPose.x() + 20., robotPose.y()}, {735., 1700.}, {765, 1600, -1.3}, 100));
    };
    auto startToPurpleFirstBlue = [](const PointOriented& robotPose) {
        return std::make_shared<Path>(Path::cubicBezier(robotPose, {robotPose.x() - 20., robotPose.y()}, {2225., 1800.}, {2225., 1700, -1.3}, 100));
    };
    auto firstBlue = table.getHexaByName("Campsite Yellow Blue");
    assert(firstBlue != nullptr);
    auto yellowFirstBlue = std::make_shared<TakeHexaAction>(startToYellowFirstBlue, nullTraj, Arm::ARM_1, firstBlue);
    auto purpleFirstBlue = std::make_shared<TakeHexaAction>(startToPurpleFirstBlue, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite Purple Blue"));
    preMatch->setOnStartYellow(yellowFirstBlue);
    preMatch->setOnStartPurple(purpleFirstBlue);

    auto firstBlueTofirstGreenYellow = [](const PointOriented&) { return std::shared_ptr<Path>(new Path({{725., 1535., -1.5}})); };
    auto firstBlueTofirstGreenPurple = [](const PointOriented& robotPose) {
        return std::make_shared<Path>(Path::cubicBezier(robotPose, {2275., 1535.}, robotPose, {2275., 1535., -1.5}, 50));
    };
    auto yellowFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenYellow, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite Yellow Green"));
    auto purpleFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenPurple, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite Purple Green"));
    yellowFirstBlue->setOnSuccess(yellowFirstGreen);
    purpleFirstBlue->setOnSuccess(purpleFirstGreen);

    auto endMatch = std::make_shared<EndMatchAction>();
    yellowFirstGreen->setOnSuccess(endMatch);
    purpleFirstGreen->setOnSuccess(endMatch);
    juggler.setFirstAction(preMatch);
    return juggler;
}
}  // namespace rd
