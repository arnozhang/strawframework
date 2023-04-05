#include "graphics/layout/LayoutParams.h"

#include "graphics/view/Attributes.h"
#include "graphics/view/AttributesHelper.h"


sf::LayoutParams::LayoutParams() {
    width = MATCH_PARENT;
    height = WRAP_CONTENT;
}

sf::LayoutParams::LayoutParams(int _width, int _height) {
    width = _width;
    height = _height;
}

sf::LayoutParams::LayoutParams(const LayoutParams& rhs) {
    *this = rhs;
}

sf::LayoutParams::LayoutParams(const LayoutParams* rhs) : LayoutParams() {
    if (rhs) {
        *this = *rhs;
    }
}

sf::LayoutParams::~LayoutParams() {
}

void sf::LayoutParams::setAttributes(Attributes* attrs) {
    BEGIN_ATTRS(attrs)
        ATTR_DIMENSION(width, setWidth)
        ATTR_DIMENSION(height, setHeight)
        ATTR_DIRECTION_X4(margin, setMargin)
    END_ATTRS()
}

sf::LayoutType sf::LayoutParams::getLayoutType() const {
    return "";
}

sf::LayoutParams& sf::LayoutParams::operator=(const LayoutParams& rhs) {
    width = rhs.width;
    height = rhs.height;
    margin = rhs.margin;
    return *this;
}
