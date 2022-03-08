#include "Coin/Table/Table.h"

namespace rd {
Table::Table() {
    createDispensers();
    fillDispensersWithHexas();
    createSheds();
    fillShedsWithHexas();
    createFreeHexas();
    assert(hexas_.size() == 3 * 2 + 3 * 2 + 1 * 2 + 2 * 2 + 3 * 2 + 3 * 2);
}

void Table::createDispensers() {
    assert(dispensers_.empty());
    // Dispenser creation
    dispensers_.insert(
        std::make_pair(eDispenser::HORIZONTAL_YELLOW, Dispenser({-64.95, 1700.}, Dispenser::eAccessibleColor::YELLOW, Dispenser::eOrientation::HORIZONTAL)));
    dispensers_.insert(
        std::make_pair(eDispenser::HORIZONTAL_PURPLE, Dispenser({3064.95, 1700.}, Dispenser::eAccessibleColor::PURPLE, Dispenser::eOrientation::HORIZONTAL)));
    dispensers_.insert(
        std::make_pair(eDispenser::VERTICAL_YELLOW, Dispenser({40., 750.}, Dispenser::eAccessibleColor::YELLOW, Dispenser::eOrientation::VERTICAL)));
    dispensers_.insert(
        std::make_pair(eDispenser::VERTICAL_PURPLE, Dispenser({2960., 750.}, Dispenser::eAccessibleColor::PURPLE, Dispenser::eOrientation::HORIZONTAL)));
    dispensers_.insert(std::make_pair(eDispenser::VERTICAL_GALLERY_YELLOWISH,
                                      Dispenser({1350., 1960.}, Dispenser::eAccessibleColor::BOTH, Dispenser::eOrientation::VERTICAL)));
    dispensers_.insert(
        std::make_pair(eDispenser::VERTICAL_GALLERY_PURPLISH, Dispenser({1650., 1960.}, Dispenser::eAccessibleColor::BOTH, Dispenser::eOrientation::VERTICAL)));
}

void Table::fillDispensersWithHexas() {
    assert(hexas_.empty());

    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::HORIZONTAL_YELLOW).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::HORIZONTAL_YELLOW).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::HORIZONTAL_PURPLE).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::HORIZONTAL_PURPLE).pushHexa(hexas_.back());

    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_YELLOW).position(), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    dispensers_.at(eDispenser::VERTICAL_YELLOW).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_YELLOW).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::VERTICAL_YELLOW).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_YELLOW).position(), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    dispensers_.at(eDispenser::VERTICAL_YELLOW).pushHexa(hexas_.back());

    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_PURPLE).position(), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    dispensers_.at(eDispenser::VERTICAL_PURPLE).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_PURPLE).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::VERTICAL_PURPLE).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_PURPLE).position(), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    dispensers_.at(eDispenser::VERTICAL_PURPLE).pushHexa(hexas_.back());

    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).pushHexa(hexas_.back());

    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).pushHexa(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).position(), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).pushHexa(hexas_.back());

    assert(dispensers_.at(eDispenser::HORIZONTAL_YELLOW).hexaLeft() == 1);
    assert(dispensers_.at(eDispenser::HORIZONTAL_PURPLE).hexaLeft() == 1);
    assert(dispensers_.at(eDispenser::VERTICAL_YELLOW).hexaLeft() == 3);
    assert(dispensers_.at(eDispenser::VERTICAL_PURPLE).hexaLeft() == 3);
    assert(dispensers_.at(eDispenser::VERTICAL_GALLERY_YELLOWISH).hexaLeft() == 3);
    assert(dispensers_.at(eDispenser::VERTICAL_GALLERY_PURPLISH).hexaLeft() == 3);
}

void Table::createFreeHexas() {
    // In front of the campsites
    hexas_.push_back(std::make_shared<Hexa>(Point(900., 1445.), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    hexas_.push_back(std::make_shared<Hexa>(Point(830., 1325.), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    hexas_.push_back(std::make_shared<Hexa>(Point(900., 1205.), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));

    hexas_.push_back(std::make_shared<Hexa>(Point(2100., 1445.), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    hexas_.push_back(std::make_shared<Hexa>(Point(2170., 1325.), Hexa::eFace::ROCKS, Hexa::eColor::GREEN, true));
    hexas_.push_back(std::make_shared<Hexa>(Point(2100., 1205.), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));

    // In the excavation sites
    hexas_.push_back(std::make_shared<Hexa>(Point(975., 625.), Hexa::eFace::TREASURE, Hexa::eColor::BLUE, false));
    hexas_.push_back(std::make_shared<Hexa>(Point(975., 625.), Hexa::eFace::TREASURE, Hexa::eColor::GREEN, false));
    hexas_.push_back(std::make_shared<Hexa>(Point(975., 625.), Hexa::eFace::TREASURE, Hexa::eColor::RED, false));

    hexas_.push_back(std::make_shared<Hexa>(Point(2025., 625.), Hexa::eFace::TREASURE, Hexa::eColor::BLUE, false));
    hexas_.push_back(std::make_shared<Hexa>(Point(2025., 625.), Hexa::eFace::TREASURE, Hexa::eColor::GREEN, false));
    hexas_.push_back(std::make_shared<Hexa>(Point(2025., 625.), Hexa::eFace::TREASURE, Hexa::eColor::RED, false));
}

void Table::createSheds() {
    sheds_.insert(std::make_pair(eColor::YELLOW, WorkShed(Point(255., 251.), eColor::YELLOW)));
    sheds_.insert(std::make_pair(eColor::PURPLE, WorkShed(Point(2745., 251.), eColor::PURPLE)));
}

void Table::fillShedsWithHexas() {
    hexas_.push_back(std::make_shared<Hexa>(Point(120.6, 311.6), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    sheds_.at(eColor::YELLOW).setRightSlot(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(Point(311.6, 120.6), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    sheds_.at(eColor::YELLOW).setLeftSlot(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(Point(2688.4, 120.6), Hexa::eFace::ROCKS, Hexa::eColor::RED, true));
    sheds_.at(eColor::PURPLE).setRightSlot(hexas_.back());
    hexas_.push_back(std::make_shared<Hexa>(Point(2879.4, 311.6), Hexa::eFace::ROCKS, Hexa::eColor::BLUE, true));
    sheds_.at(eColor::PURPLE).setLeftSlot(hexas_.back());
}

}  // namespace rd
