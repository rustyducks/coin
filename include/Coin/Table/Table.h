#ifndef TABLE_H
#define TABLE_H

#include <unordered_map>

#include "Coin/Table/Colors.h"
#include "Coin/Table/Dispenser.h"
#include "Coin/Table/Hexa.h"
#include "Coin/Table/WorkShed.h"

namespace rd {
class Table {
   public:
    enum eDispenser { HORIZONTAL_YELLOW, HORIZONTAL_PURPLE, VERTICAL_YELLOW, VERTICAL_PURPLE, VERTICAL_GALLERY_YELLOWISH, VERTICAL_GALLERY_PURPLISH };

    Table();

   protected:
    std::vector<std::shared_ptr<Hexa>> hexas_;
    std::unordered_map<eDispenser, Dispenser> dispensers_;
    // std::unordered_map<eColor, void> galleries_;        // TODO
    std::unordered_map<eColor, WorkShed> sheds_;
    // std::unordered_map<eColor, void> statuetteStands_;  // TODO
    // std::unordered_map<eColor, void> displayCabinet_;   // TODO

    void createDispensers();
    void fillDispensersWithHexas();
    void createFreeHexas();
    void createSheds();
    void fillShedsWithHexas();
};
}  // namespace rd

#endif /* TABLE_H */
