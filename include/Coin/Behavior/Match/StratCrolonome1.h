#include <algorithm>

#include "Coin/Behavior/Match/ActionJuggler.h"
#include "Coin/Behavior/Match/ExcavationAction.h"
#include "Coin/Behavior/Match/ExposeStatuetteAction.h"
#include "Coin/Behavior/Match/IndianaJones.h"
#include "Coin/Behavior/Match/PreMatchAction.h"
#include "Coin/Table/Table.h"

namespace rd {
ActionJuggler createStratCrolonome1(Robot& robot, Table& table) {
    ActionJuggler juggler(robot);
    auto preMatch = std::make_shared<PreMatchAction>(PointOriented({90., 1138.15, 0.0}), PointOriented({2910., 1138.15, M_PI}));

    std::vector<ExcavationActionPtr> excavationActionsYellow;
    std::vector<ExcavationActionPtr> excavationActionsPurple;
    for (int i = 6; i >= 0; i--) {
        std::cout << table.getExcavationSquare(i)->id() << ":" << i << std::endl;
        excavationActionsYellow.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i)));
    }
    for (size_t i = 3; i < 10; i++) {
        std::cout << table.getExcavationSquare(i)->id() << ":" << i << std::endl;
        excavationActionsPurple.push_back(std::make_shared<ExcavationAction>(table.getExcavationSquare(i)));
    }
    for (size_t i = 0; i < excavationActionsYellow.size() - 1; i++) {
        excavationActionsYellow.at(i)->setNextSquares(std::vector<ExcavationActionPtr>(excavationActionsYellow.begin() + i + 1, excavationActionsYellow.end()));
    }
    for (size_t i = 0; i < excavationActionsPurple.size() - 1; i++) {
        excavationActionsPurple.at(i)->setNextSquares(std::vector<ExcavationActionPtr>(excavationActionsPurple.begin() + i + 1, excavationActionsPurple.end()));
    }

    preMatch->setOnStartYellow(excavationActionsYellow.at(0));
    preMatch->setOnStartPurple(excavationActionsPurple.at(0));

    auto takeStatuetteYellow =
        std::make_shared<IndianaJones>("Indiana Jones Yellow", PointOriented(320., 420., -70. * M_PI / 180.), PointOriented(280., 380., -70. * M_PI / 180.),
                                       PointOriented(336.5, 333.5, -70. * M_PI / 180.), PointOriented(376.5, 373.5, -70. * M_PI / 180.));

    auto takeStatuettePurple =
        std::make_shared<IndianaJones>("Indiana Jones Purple", PointOriented(2680., 420., 70. * M_PI / 180.), PointOriented(2720., 380., 70. * M_PI / 180.),
                                       PointOriented(2663.5, 333.5, 70. * M_PI / 180.), PointOriented(2623.5, 373.5, 70. * M_PI / 180.));

    for (auto& yellowExcAct : excavationActionsYellow) {
        yellowExcAct->setOnSuccess(takeStatuetteYellow);
    }
    for (auto& purpleExcAct : excavationActionsPurple) {
        purpleExcAct->setOnSuccess(takeStatuettePurple);
    }

    auto exposeStatuetteYellow =
        std::make_shared<ExposeStatuetteAction>("Expose Statuette Yellow", PointOriented(270., 1680., 150. * M_PI / 180.),
                                                PointOriented(270., 1820., 150. * M_PI / 180.), PointOriented(270., 1680., 150. * M_PI / 180.));

    auto exposeStatuettePurple =
        std::make_shared<ExposeStatuetteAction>("Expose Statuette Purple", PointOriented(2730., 1680., 150. * M_PI / 180.),
                                                PointOriented(2730., 1820., 150. * M_PI / 180.), PointOriented(2730., 1680., 150. * M_PI / 180.));

    takeStatuetteYellow->setOnSuccess(exposeStatuetteYellow);
    takeStatuettePurple->setOnSuccess(exposeStatuettePurple);

    auto endMatch = std::make_shared<EndMatchAction>();
    exposeStatuetteYellow->setOnSuccess(endMatch);
    exposeStatuettePurple->setOnSuccess(endMatch);
    juggler.setFirstAction(preMatch);
    return juggler;
}
}  // namespace rd
