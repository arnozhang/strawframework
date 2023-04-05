#include "graphics/layout/AbsoluteLayoutSupporter.h"

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/AbsoluteLayout.h"
#include "graphics/view/AttributesHelper.h"


sf::AbsoluteLayoutParams::AbsoluteLayoutParams() {
}

sf::AbsoluteLayoutParams::AbsoluteLayoutParams(const LayoutParams* rhs)
    : AbsoluteLayoutParams() {

    if (rhs) {
        *this = *rhs;
    }
}

sf::AbsoluteLayoutParams::AbsoluteLayoutParams(const AbsoluteLayoutParams& rhs) {
    *this = rhs;
}

sf::AbsoluteLayoutParams::AbsoluteLayoutParams(const LayoutParams& rhs) {
    *this = rhs;
}

sf::AbsoluteLayoutParams::~AbsoluteLayoutParams() {
}

sf::LayoutType sf::AbsoluteLayoutParams::getLayoutType() const {
    return sf::AbsoluteLayout::getElemenetName();
}

void sf::AbsoluteLayoutParams::setAttributes(Attributes* attrs) {
    LayoutParams::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_POINT(position, setPosition)
    END_ATTRS()
}

void sf::AbsoluteLayoutParams::setPosition(const Point& pt) {
    position = pt;
}

sf::AbsoluteLayoutParams& sf::AbsoluteLayoutParams::operator=(const AbsoluteLayoutParams& rhs) {
    LayoutParams::operator= (rhs);

    position = rhs.position;
    return *this;
}

sf::AbsoluteLayoutParams& sf::AbsoluteLayoutParams::operator=(const LayoutParams& rhs) {
    LayoutParams::operator= (rhs);
    return *this;
}


sf::AbsoluteLayoutSupporter::AbsoluteLayoutSupporter() {
}

sf::AbsoluteLayoutSupporter::~AbsoluteLayoutSupporter() {
}

sf::LayoutType sf::AbsoluteLayoutSupporter::getLayoutType() const {
    return sf::AbsoluteLayout::getElemenetName();
}

sf::LayoutParams* sf::AbsoluteLayoutSupporter::createDefaultLayoutParams(
    LayoutParams* params /*= 0*/) {

    return new sf::AbsoluteLayoutParams(params);
}

void sf::AbsoluteLayoutSupporter::layout(ViewGroup* viewGroup) {
    const int count = viewGroup->getChildCount();
    if (count <= 0) {
        return;
    }

    const Directionx4& padding = viewGroup->getPadding();
    const Point& position = viewGroup->getPosition();

    for (auto i = 0; i < count; ++i) {
        ref_ptr<View> view = viewGroup->getChildAt(i);
        ref_ptr<AbsoluteLayoutParams> params = reinterpret_cast<AbsoluteLayoutParams*>(
            &view->getLayoutParams());

        view->layoutTo(
            padding.left + params->position.x,
            padding.top + params->position.y);
    }
}
