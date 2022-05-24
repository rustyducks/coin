#ifndef TABLE_H
#define TABLE_H

#include <unordered_map>

#include "Coin/Table/Colors.h"
#include "Coin/Table/Dispenser.h"
#include "Coin/Table/ExcavationSquare.h"
#include "Coin/Table/Gallery.h"
#include "Coin/Table/Hexa.h"
#include "Coin/Table/WorkShed.h"

namespace rd {
class Table {
   public:
    const int BASE_SCORE_CROLONOME = 2 + 2;  // Statuette on pedestal, display cabinet installed
    enum eDispenser { HORIZONTAL_YELLOW, HORIZONTAL_PURPLE, VERTICAL_YELLOW, VERTICAL_PURPLE, VERTICAL_GALLERY_YELLOWISH, VERTICAL_GALLERY_PURPLISH };

    Table();

    HexaPtr getHexaByName(const std::string& name) { return nameToHexa_.at(name); }
    ExcavationSquarePtr getExcavationSquare(const size_t i) { return excavationSquares_.at(i); }
    const std::vector<ExcavationSquarePtr> getExcavationSquares() { return excavationSquares_; }

    GalleryPtr getGallery(const size_t i) { return galleries_.at(i); }

    void reasonExcavationSquareColors();

    bool isStatuetteOnPedestal;
    bool isReplicaOnPedestal;
    bool isStatuetteInCabinet;
    bool isCabinetActivated;

    int countPoints(bool isCrolonome, eColor robotColor, PointOriented robotPose) const;

   protected:
    const ExcavationSquare::eColor possibleSquaresLayouts[4][10] = {
        {ExcavationSquare::YELLOW, ExcavationSquare::YELLOW, ExcavationSquare::RED, ExcavationSquare::PURPLE, ExcavationSquare::YELLOW,
         ExcavationSquare::YELLOW, ExcavationSquare::PURPLE, ExcavationSquare::RED, ExcavationSquare::PURPLE, ExcavationSquare::PURPLE},
        {ExcavationSquare::RED, ExcavationSquare::YELLOW, ExcavationSquare::YELLOW, ExcavationSquare::YELLOW, ExcavationSquare::PURPLE,
         ExcavationSquare::PURPLE, ExcavationSquare::YELLOW, ExcavationSquare::PURPLE, ExcavationSquare::PURPLE, ExcavationSquare::RED},
        {ExcavationSquare::YELLOW, ExcavationSquare::YELLOW, ExcavationSquare::RED, ExcavationSquare::YELLOW, ExcavationSquare::PURPLE,
         ExcavationSquare::PURPLE, ExcavationSquare::YELLOW, ExcavationSquare::RED, ExcavationSquare::PURPLE, ExcavationSquare::PURPLE},
        {ExcavationSquare::RED, ExcavationSquare::YELLOW, ExcavationSquare::YELLOW, ExcavationSquare::PURPLE, ExcavationSquare::YELLOW,
         ExcavationSquare::YELLOW, ExcavationSquare::PURPLE, ExcavationSquare::PURPLE, ExcavationSquare::PURPLE, ExcavationSquare::RED}};
    std::vector<std::shared_ptr<Hexa>> hexas_;
    std::unordered_map<std::string, HexaPtr> nameToHexa_;
    std::unordered_map<eDispenser, Dispenser> dispensers_;
    std::vector<std::shared_ptr<Gallery>> galleries_;
    std::vector<ExcavationSquarePtr> excavationSquares_;
    std::unordered_map<eColor, WorkShed> sheds_;
    // std::unordered_map<eColor, void> statuetteStands_;  // TODO
    // std::unordered_map<eColor, void> displayCabinet_;   // TODO

    void createDispensers();
    void fillDispensersWithHexas();
    void createFreeHexas();
    void createSheds();
    void fillShedsWithHexas();
    void createExcavationSquares();
    void createGalleries();

    bool isOneExcavationSquareUnknown(const std::vector<size_t>& amongst) const;
    bool isOneExcavationSquareKnown(const std::vector<size_t>& amongst) const;
    void reasonExcavationSquares(const std::vector<size_t>& amongst);
};
}  // namespace rd

#endif /* TABLE_H */
