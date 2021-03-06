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
        std::make_shared<GoToDiffAction>("Approach Yellow First Green", sGoToDiffTraj({PointOriented(2390., 1390., M_PI), false, false}));
    preMatch->setOnStartYellow(approachYellowFirstGreen);
    preMatch->setOnStartPurple(approachPurpleFirstGreen);

    auto yellowFirstGreen = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(650., 1250., 0.), false, false}),
                                                             sTakeHexaTraj({PointOriented(560., 1120., 0.), true, false}), Arm::ARM_1,
                                                             table.getHexaByName("Campsite Yellow Green"));
    auto purpleFirstGreen = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2340., 1390., M_PI), false, false}),
                                                             sTakeHexaTraj({PointOriented(2440., 1260., M_PI), true, false}), Arm::ARM_1,
                                                             table.getHexaByName("Campsite Purple Green"));

    approachYellowFirstGreen->setOnSuccess(yellowFirstGreen);
    approachPurpleFirstGreen->setOnSuccess(purpleFirstGreen);

    auto yellowFirstRed =
        std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(730., 1120., 0.), false, false}),
                                         sTakeHexaTraj({PointOriented(580., 1370., 0.), true, false}), Arm::ARM_1, table.getHexaByName("Campsite Yellow Red"));
    auto purpleFirstRed = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2270., 1260., M_PI), false, false}),
                                                           sTakeHexaTraj({PointOriented(2420., 1510., M_PI), true, false}), Arm::ARM_1,
                                                           table.getHexaByName("Campsite Purple Red"));

    yellowFirstGreen->setOnSuccess(yellowFirstRed);
    purpleFirstGreen->setOnSuccess(purpleFirstRed);
    yellowFirstGreen->setOnVacuumError(yellowFirstRed);
    purpleFirstGreen->setOnVacuumError(purpleFirstRed);

    auto yellowFirstBlue = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(710., 1370., 0.), false, false}),
                                                            sTakeHexaTraj({PointOriented(600., 1370., M_PI / 2.), true, false}), Arm::ARM_1,
                                                            table.getHexaByName("Campsite Yellow Blue"));
    auto purpleFirstBlue = std::make_shared<TakeHexaAction>(sTakeHexaTraj({PointOriented(2270., 1510., M_PI), false, false}),
                                                            sTakeHexaTraj({PointOriented(2540., 1510., M_PI / 2), true, false}), Arm::ARM_1,
                                                            table.getHexaByName("Campsite Purple Blue"));

    yellowFirstRed->setOnSuccess(yellowFirstBlue);
    purpleFirstRed->setOnSuccess(purpleFirstBlue);
    yellowFirstRed->setOnVacuumError(yellowFirstBlue);
    purpleFirstRed->setOnVacuumError(purpleFirstBlue);

    auto gallerySwitcherYellow = std::make_shared<GallerySwitcher>("Yellow Gallery Switcher");
    auto gallerySwitcherPurple = std::make_shared<GallerySwitcher>("Purple Gallery Switcher");

    yellowFirstBlue->setOnSuccess(gallerySwitcherYellow);
    purpleFirstBlue->setOnSuccess(gallerySwitcherPurple);
    yellowFirstBlue->setOnVacuumError(gallerySwitcherYellow);
    purpleFirstBlue->setOnVacuumError(gallerySwitcherPurple);

    auto goToInFrontOfYellowBlue =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Yellow Blue", sGoToDiffTraj({PointOriented(600., 1600., M_PI_2), false, true}));
    auto goToInFrontOfPurpleBlue =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Purple Blue", sGoToDiffTraj({PointOriented(2540., 1600., M_PI_2), false, true}));

    gallerySwitcherYellow->setOnBlue(goToInFrontOfYellowBlue);
    gallerySwitcherPurple->setOnBlue(goToInFrontOfPurpleBlue);

    auto dropInYellowBlue =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(600., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(600., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(0));
    auto dropInPurpleBlue =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(2540., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(2540., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(5));

    goToInFrontOfYellowBlue->setOnSuccess(dropInYellowBlue);
    goToInFrontOfPurpleBlue->setOnSuccess(dropInPurpleBlue);

    dropInYellowBlue->setOnSuccess(gallerySwitcherYellow);
    dropInPurpleBlue->setOnSuccess(gallerySwitcherPurple);
    dropInYellowBlue->setOnFailure(gallerySwitcherYellow);
    dropInPurpleBlue->setOnFailure(gallerySwitcherPurple);

    auto goToInFrontOfYellowRed =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Yellow Red", sGoToDiffTraj({PointOriented(1080., 1600., M_PI_2), false, true}));
    auto goToInFrontOfPurpleRed =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Purple Red", sGoToDiffTraj({PointOriented(2060., 1600., M_PI_2), false, true}));

    gallerySwitcherYellow->setOnRed(goToInFrontOfYellowRed);
    gallerySwitcherPurple->setOnRed(goToInFrontOfPurpleRed);

    auto dropInYellowRed =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(1080., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(1080., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(2));
    auto dropInPurpleRed =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(2060., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(2060., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(3));

    goToInFrontOfYellowRed->setOnSuccess(dropInYellowRed);
    goToInFrontOfPurpleRed->setOnSuccess(dropInPurpleRed);

    dropInYellowRed->setOnSuccess(gallerySwitcherYellow);
    dropInPurpleRed->setOnSuccess(gallerySwitcherPurple);
    dropInYellowRed->setOnFailure(gallerySwitcherYellow);
    dropInPurpleRed->setOnFailure(gallerySwitcherPurple);

    auto goToInFrontOfYellowGreen =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Yellow Green", sGoToDiffTraj({PointOriented(840., 1600., M_PI_2), false, true}));
    auto goToInFrontOfPurpleGreen =
        std::make_shared<GoToDiffAction>("Move In Front Gallery Purple Green", sGoToDiffTraj({PointOriented(2300., 1600., M_PI_2), false, true}));

    gallerySwitcherYellow->setOnGreen(goToInFrontOfYellowGreen);
    gallerySwitcherPurple->setOnGreen(goToInFrontOfPurpleGreen);

    auto dropInYellowGreen =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(840., 1760., M_PI / 2), false, false}),
                                            sDropHexaTraj({PointOriented(840., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(1));
    auto dropInPurpleGreen =
        std::make_shared<DropHexaInGallery>(sDropHexaTraj({PointOriented(2300., 1760., M_PI / 2), false, true}),
                                            sDropHexaTraj({PointOriented(2300., 1600., M_PI / 2), true, false}), Arm::ARM_1, table.getGallery(4));

    goToInFrontOfYellowGreen->setOnSuccess(dropInYellowGreen);
    goToInFrontOfPurpleGreen->setOnSuccess(dropInPurpleGreen);

    dropInYellowGreen->setOnSuccess(gallerySwitcherYellow);
    dropInPurpleGreen->setOnSuccess(gallerySwitcherPurple);
    dropInYellowGreen->setOnFailure(gallerySwitcherYellow);
    dropInPurpleGreen->setOnFailure(gallerySwitcherPurple);

    /*auto firstBlueTofirstGreenYellow = [](const PointOriented&) { return std::shared_ptr<Path>(new Path({{725., 1535., -1.5}})); };
    auto firstBlueTofirstGreenPurple = [](const PointOriented& robotPose) {
        return std::make_shared<Path>(Path::cubicBezier(robotPose, {2275., 1535.}, robotPose, {2275., 1535., -1.5}, 50));
    };
    auto yellowFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenYellow, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite Yellow
    Green")); auto purpleFirstGreen = std::make_shared<TakeHexaAction>(firstBlueTofirstGreenPurple, nullTraj, Arm::ARM_1, table.getHexaByName("Campsite
    Purple Green")); yellowFirstBlue->setOnSuccess(yellowFirstGreen); purpleFirstBlue->setOnSuccess(purpleFirstGreen);
    */

    // auto freePathForCroYellow = std::make_shared<GoToDiffAction>("Free Path Cro Yellow", sGoToDiffTraj({PointOriented(860., 1600., M_PI_2), false, true}));
    // auto freePathForCroPurple = std::make_shared<GoToDiffAction>("Free Path Cro Purple", sGoToDiffTraj({PointOriented(2140., 1600., M_PI_2), false, true}));

    auto endMatch = std::make_shared<EndMatchAction>();
    gallerySwitcherYellow->setOnSuccess(endMatch);
    gallerySwitcherPurple->setOnSuccess(endMatch);

    // freePathForCroYellow->setOnSuccess(endMatch);
    // freePathForCroPurple->setOnSuccess(endMatch);

    juggler.setAlmostEndMatchPoints({200., 1600., 0.}, {2800., 1600., M_PI});
    juggler.setOvertimeAction(endMatch);
    juggler.setFirstAction(preMatch);
    return juggler;
}
}  // namespace rd
