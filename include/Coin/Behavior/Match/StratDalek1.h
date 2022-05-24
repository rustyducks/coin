#include "Coin/Behavior/Match/ActionJuggler.h"
#include "Coin/Behavior/Match/DropHexaInGallery.h"
#include "Coin/Behavior/Match/GoToDiffAction.h"
#include "Coin/Behavior/Match/HexaAction.h"
#include "Coin/Behavior/Match/PreMatchAction.h"
#include "Coin/Table/Table.h"

namespace rd {
ActionJuggler createStratDalek1(Robot& robot, Table& table) {
    // auto nullTraj = [](const PointOriented&) { return nullptr; };

    ActionJuggler juggler(robot);
    auto preMatch = std::make_shared<PreMatchAction>(PointOriented({90., 1440., 0.0}), PointOriented({2910., 1440., M_PI}));
    auto approachYellowFirstGreen =
        std::make_shared<GoToDiffAction>("Approach Yellow First Green", sGoToDiffTraj({PointOriented(610., 1250., 0.), false, false}));
    auto approachPurpleFirstGreen =
        std::make_shared<GoToDiffAction>("Approach Yellow First Green", sGoToDiffTraj({PointOriented(2390., 1250., 0.), false, false}));
    preMatch->setOnStartYellow(approachYellowFirstGreen);
    preMatch->setOnStartPurple(approachPurpleFirstGreen);

    auto yellowFirstGreen = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(650., 1250., 0.), false, false}),
                                                             sTakeHexaTraj({PointOriented(560., 1120., 0.), true, false}), Arm::ARM_1,
                                                             table.getHexaByName("Campsite Yellow Green"));
    auto purpleFirstGreen = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2350., 1250., 0.), false, false}),
                                                             sTakeHexaTraj({PointOriented(2440., 1120., 0.), true, false}), Arm::ARM_1,
                                                             table.getHexaByName("Campsite Purple Green"));

    approachYellowFirstGreen->setOnSuccess(yellowFirstGreen);
    approachPurpleFirstGreen->setOnSuccess(purpleFirstGreen);

    auto yellowFirstRed =
        std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(730., 1120., 0.), false, false}),
                                         sTakeHexaTraj({PointOriented(580., 1370., 0.), true, false}), Arm::ARM_1, table.getHexaByName("Campsite Yellow Red"));
    auto purpleFirstRed =
        std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2270., 1120., 0.), false, false}),
                                         sTakeHexaTraj({PointOriented(2420., 1370., 0.), true, false}), Arm::ARM_1, table.getHexaByName("Campsite Purple Red"));

    yellowFirstGreen->setOnSuccess(yellowFirstRed);
    purpleFirstGreen->setOnSuccess(purpleFirstRed);

    auto yellowFirstBlue = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(710., 1370., 0.), false, false}),
                                                            sTakeHexaTraj({PointOriented(600., 1370., M_PI / 2.), true, false}), Arm::ARM_1,
                                                            table.getHexaByName("Campsite Yellow Blue"));
    auto purpleFirstBlue = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2290., 1370., 0.), false, false}),
                                                            sTakeHexaTraj({PointOriented(2400., 1370., M_PI / 2), true, false}), Arm::ARM_1,
                                                            table.getHexaByName("Campsite Purple Blue"));

    yellowFirstRed->setOnSuccess(yellowFirstBlue);
    purpleFirstRed->setOnSuccess(purpleFirstBlue);

    auto dropInYellowBlue =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(600., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(600., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(0));
    auto dropInPurpleBlue =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(2400., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(2400., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(5));
    yellowFirstBlue->setOnSuccess(dropInYellowBlue);
    purpleFirstBlue->setOnSuccess(dropInPurpleBlue);

    auto goToInFrontOfYellowRed =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Yellow Red", sGoToDiffTraj({PointOriented(1080., 1600., M_PI_2), false, true}));
    auto goToInFrontOfPurpleRed =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Purple Red", sGoToDiffTraj({PointOriented(1920., 1600., M_PI_2), false, true}));

    dropInYellowBlue->setOnSuccess(goToInFrontOfYellowRed);
    dropInPurpleBlue->setOnSuccess(goToInFrontOfPurpleRed);

    auto dropInYellowRed =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(1080., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(1080., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(2));
    auto dropInPurpleRed =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(1920., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(1920., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(3));

    goToInFrontOfYellowRed->setOnSuccess(dropInYellowRed);
    goToInFrontOfPurpleRed->setOnSuccess(dropInPurpleRed);

    auto goToInFrontOfYellowGreen =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Yellow Green", sGoToDiffTraj({PointOriented(840., 1600., M_PI_2), false, true}));
    auto goToInFrontOfPurpleGreen =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Purple Green", sGoToDiffTraj({PointOriented(1160., 1600., M_PI_2), false, true}));

    dropInYellowRed->setOnSuccess(goToInFrontOfYellowGreen);
    dropInPurpleRed->setOnSuccess(goToInFrontOfPurpleGreen);

    auto dropInYellowGreen =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(840., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(840., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(1));
    auto dropInPurpleGreen =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(1160., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(1160., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(4));

    goToInFrontOfYellowGreen->setOnSuccess(dropInYellowGreen);
    goToInFrontOfPurpleGreen->setOnSuccess(dropInPurpleGreen);

    /*auto firstBlueTofirstGreenYellow = [](const PointOriented&) { return std::shared_ptr<Path>(new Path({{725., 1535., -1.5}})); };
    auto firstBlueTofirstGreenPurple = [](const PointOriented& robotPose) {
        return std::make_shared<Path>(Path::cubicBezier(robotPose, {2275., 1535.}, robotPose, {2275., 1535., -1.5}, 50));
    };
    auto yellowFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenYellow, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite Yellow
    Green")); auto purpleFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenPurple, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite
    Purple Green")); yellowFirstBlue->setOnSuccess(yellowFirstGreen); purpleFirstBlue->setOnSuccess(purpleFirstGreen);
    */

    auto endMatch = std::make_shared<EndMatchAction>();

    dropInYellowGreen->setOnSuccess(endMatch);
    dropInPurpleGreen->setOnSuccess(endMatch);

    juggler.setAlmostEndMatchPoints({200., 1600., 0.}, {2800., 1600., M_PI});
    juggler.setOvertimeAction(endMatch);
    juggler.setFirstAction(preMatch);
    return juggler;
}
}  // namespace rd
