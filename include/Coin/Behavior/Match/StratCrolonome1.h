#include <algorithm>

#include "Coin/Behavior/Match/ActionJuggler.h"
#include "Coin/Behavior/Match/AlignDiffAction.h"
#include "Coin/Behavior/Match/ExcavationAction.h"
#include "Coin/Behavior/Match/ExposeStatuetteAction.h"
#include "Coin/Behavior/Match/GoToAction.h"
#include "Coin/Behavior/Match/IndianaJones.h"
#include "Coin/Behavior/Match/PreMatchAction.h"
#include "Coin/Behavior/Match/WaitForMatchTimeAction.h"
#include "Coin/Table/Table.h"

namespace rd {
ActionJuggler createStratCrolonome1(Robot& robot, Table& table) {
    ActionJuggler juggler(robot);
    auto preMatch = std::make_shared<PreMatchAction>(PointOriented({90., 1138.15, 0.0}), PointOriented({2890., 1138.15, M_PI}));

    auto avoidExcavationSiteYellow1 = std::make_shared<GoToAction>("Avoid Excavation Site Yellow 1", PointOriented(700., 300., -150. * M_PI / 180.));
    auto avoidExcavationSitePurple1 = std::make_shared<GoToAction>("Avoid Excavation Site Purple 1", PointOriented(2300., 300., -150. * M_PI / 180.));

    preMatch->setOnStartYellow(avoidExcavationSiteYellow1);
    preMatch->setOnStartPurple(avoidExcavationSitePurple1);

    std::vector<ExcavationActionPtr> excavationActionsYellow;
    std::vector<ExcavationActionPtr> excavationActionsPurple;
    for (int i = 6; i >= 0; i--) {
        std::cout << table.getExcavationSquare(i)->id() << ":" << i << std::endl;
        if (i == 6) {
            excavationActionsYellow.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i), 0.));
        } else {
            excavationActionsYellow.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i)));
        }
    }
    for (size_t i = 3; i < 10; i++) {
        std::cout << table.getExcavationSquare(i)->id() << ":" << i << std::endl;

        if (i == 3 || i == 4) {
            excavationActionsPurple.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i), 50.));
        } else {
            excavationActionsPurple.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i)));
        }
    }
    for (size_t i = 0; i < excavationActionsYellow.size() - 1; i++) {
        excavationActionsYellow.at(i)->setNextSquares(std::vector<ExcavationActionPtr>(excavationActionsYellow.begin() + i + 1, excavationActionsYellow.end()));
    }
    for (size_t i = 0; i < excavationActionsPurple.size() - 1; i++) {
        excavationActionsPurple.at(i)->setNextSquares(std::vector<ExcavationActionPtr>(excavationActionsPurple.begin() + i + 1, excavationActionsPurple.end()));
    }

    avoidExcavationSiteYellow1->setOnSuccess(excavationActionsYellow.at(0));
    avoidExcavationSitePurple1->setOnSuccess(excavationActionsPurple.at(0));

    auto takeStatuetteYellow =
        std::make_shared<IndianaJones>("Indiana Jones Yellow", PointOriented(340., 400., -75. * M_PI / 180.), PointOriented(302., 357., -75. * M_PI / 180.),
                                       PointOriented(345.5, 322.5, -75. * M_PI / 180.), PointOriented(376.5, 373.5, -75. * M_PI / 180.));

    auto takeStatuettePurple =
        std::make_shared<IndianaJones>("Indiana Jones Purple", PointOriented(2560., 330., 15. * M_PI / 180.), PointOriented(2600., 290., 15. * M_PI / 180.),
                                       PointOriented(2685., 365., 15. * M_PI / 180.), PointOriented(2590., 450., 15. * M_PI / 180.));

    for (auto& yellowExcAct : excavationActionsYellow) {
        yellowExcAct->setOnSuccess(takeStatuetteYellow);
    }
    for (auto& purpleExcAct : excavationActionsPurple) {
        purpleExcAct->setOnSuccess(takeStatuettePurple);
    }

    // To remove
    /*auto avoidDebug1 = std::make_shared<GoToAction>("Avoid Excavation Site Yellow 1", PointOriented(500., 600., -75. * M_PI / 180.));
    preMatch->setOnStartYellow(avoidDebug1);
    avoidDebug1->setOnSuccess(takeStatuetteYellow);
    preMatch->setOnStartPurple(takeStatuettePurple);*/

    auto waitForDalekYellow = std::make_shared<WaitForMatchTimeAction>("Wait for Dalek Yellow", 60.);
    auto waitForDalekPurple = std::make_shared<WaitForMatchTimeAction>("Wait for Dalek Purple", 60.);

    takeStatuetteYellow->setOnSuccess(waitForDalekYellow);
    takeStatuettePurple->setOnSuccess(waitForDalekPurple);

    auto alignYellow =
        std::make_shared<AlignDiffAction>("Align Yellow", PointOriented(300., 1600., 150. * M_PI / 180.), 135., false, robot.borderSensorYellow, 5.);
    auto alignPurple =
        std::make_shared<AlignDiffAction>("Align Purple", PointOriented(2810., 1600., 150. * M_PI / 180.), 80., true, robot.borderSensorPurple, 5.);

    waitForDalekYellow->setOnTime(alignYellow);
    waitForDalekPurple->setOnTime(alignPurple);

    auto exposeStatuetteYellow =
        std::make_shared<ExposeStatuetteAction>("Expose Statuette Yellow", PointOriented(300., 1680., 150. * M_PI / 180.),
                                                PointOriented(300., 1870., 150. * M_PI / 180.), PointOriented(300., 1680., 150. * M_PI / 180.));

    auto exposeStatuettePurple =
        std::make_shared<ExposeStatuetteAction>("Expose Statuette Purple", PointOriented(2810., 1680., 150. * M_PI / 180.),
                                                PointOriented(2810., 1870., 150. * M_PI / 180.), PointOriented(2810., 1680., 150. * M_PI / 180.));

    alignYellow->setOnSuccess(exposeStatuetteYellow);
    alignPurple->setOnSuccess(exposeStatuettePurple);

    auto goToBercailYellow = std::make_shared<GoToAction>("Go To Bercail Yellow", PointOriented(200., 1000., 0.));
    auto goToBercailPurple = std::make_shared<GoToAction>("Go To Bercail Purple", PointOriented(2800., 1000., M_PI));
    exposeStatuetteYellow->setOnSuccess(goToBercailYellow);
    exposeStatuettePurple->setOnSuccess(goToBercailPurple);

    auto endMatch = std::make_shared<EndMatchAction>();

    goToBercailYellow->setOnSuccess(endMatch);
    goToBercailPurple->setOnSuccess(endMatch);

    juggler.setAlmostEndMatchPoints({200., 1000., 0.}, {2800., 1000., M_PI});
    juggler.setOvertimeAction(endMatch);
    juggler.setFirstAction(preMatch);
    return juggler;
}
}  // namespace rd
