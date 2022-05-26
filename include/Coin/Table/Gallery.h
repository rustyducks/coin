#ifndef GALLERY_H
#define GALLERY_H

#include <GeometryTools/Point.h>

#include <memory>
#include <vector>

#include "Coin/Table/Colors.h"
#include "Coin/Table/Hexa.h"

namespace rd {
class Hexa;

class Gallery {
   public:
    Gallery(const Point& position, const eColor& color, const Hexa::eColor galleryColor) : position_(position), color_(color), galleryColor_(galleryColor) {}

    void addHexa(std::shared_ptr<Hexa> hexa) { hexas_.push_back(hexa); }
    eColor sideColor() const { return color_; }
    Hexa::eColor galleryColor() const { return galleryColor_; }
    const std::vector<HexaPtr> getHexas() const { return hexas_; }

   protected:
    Point position_;
    eColor color_;
    Hexa::eColor galleryColor_;
    std::vector<std::shared_ptr<Hexa>> hexas_;
};

typedef std::shared_ptr<Gallery> GalleryPtr;

}  // namespace rd

#endif /* GALLERY_H */
