#include "Coin/Communication/IvyHandler.h"

namespace rd {
IvyHandler::IvyHandler() : ivy_(Ivy::getInstance()) {
    ivy_.subscribeGoTo([this](const Point& point) -> void { this->onGoToPoint(point); });
    ivy_.subscribeGoToOrient([this](const PointOriented& point) -> void { this->onGoToOrient(point); });
}

void IvyHandler::onGoToPoint(const Point& point) { pointInputs_.push_back(PointInput(eInput::GO_TO_POINT, point)); }

void IvyHandler::onGoToOrient(const PointOriented& point) { pointOrientedInputs_.push_back(PointOrientedInput(eInput::GO_TO_POINT_ORIENTED, point)); }

std::vector<std::unique_ptr<Input>> IvyHandler::getInputs() {
    std::vector<std::unique_ptr<Input>> toReturn;
    toReturn.reserve(pointInputs_.size() + pointOrientedInputs_.size());
    for (const auto& Input : pointInputs_) {
        toReturn.push_back(std::make_unique<PointInput>(Input));
    }
    for (const auto& Input : pointOrientedInputs_) {
        toReturn.push_back(std::make_unique<PointOrientedInput>(Input));
    }
    pointInputs_.clear();
    pointOrientedInputs_.clear();
    return toReturn;
}
void IvyHandler::sendSpeed(const Speed&) { return; }
void IvyHandler::sendPoseReport(const PointOriented&) { return; }
}  // namespace rd
