#include "graphics/layout/FrameLayoutSupporter.h"

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/FrameLayout.h"
#include "graphics/view/AttributesHelper.h"


sf::FrameLayoutParams::FrameLayoutParams() {
    gravity = sf::Gravity::TopLeft;
}

sf::FrameLayoutParams::FrameLayoutParams(const LayoutParams* rhs)
    : FrameLayoutParams() {

    if (rhs) {
        *this = *rhs;
    }
}

sf::FrameLayoutParams::FrameLayoutParams(const FrameLayoutParams& rhs) {
    *this = rhs;
}

sf::FrameLayoutParams::FrameLayoutParams(const LayoutParams& rhs) {
    *this = rhs;
}

sf::FrameLayoutParams::~FrameLayoutParams() {
}

sf::LayoutType sf::FrameLayoutParams::getLayoutType() const {
    return sf::FrameLayout::getElemenetName();
}

void sf::FrameLayoutParams::setAttributes(Attributes* attrs) {
    LayoutParams::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_ENUM(layoutGravity, setLayoutGravity, Gravity)
    END_ATTRS()
}

void sf::FrameLayoutParams::setLayoutGravity(int _gravity) {
    gravity = _gravity;
}

sf::FrameLayoutParams& sf::FrameLayoutParams::operator=(const FrameLayoutParams& rhs) {
    LayoutParams::operator= (rhs);

    gravity = rhs.gravity;
    return *this;
}

sf::FrameLayoutParams& sf::FrameLayoutParams::operator=(const LayoutParams& rhs) {
    LayoutParams::operator= (rhs);
    return *this;
}


sf::FrameLayoutSupporter::FrameLayoutSupporter() {
}

sf::FrameLayoutSupporter::~FrameLayoutSupporter() {
}

sf::LayoutType sf::FrameLayoutSupporter::getLayoutType() const {
    return sf::FrameLayout::getElemenetName();
}

sf::LayoutParams* sf::FrameLayoutSupporter::createDefaultLayoutParams(
    LayoutParams* params /*= 0*/) {

    return new sf::FrameLayoutParams(params);
}

void sf::FrameLayoutSupporter::layout(ViewGroup* viewGroup) {
    const int count = viewGroup->getChildCount();
    if (count <= 0) {
        return;
    }

    const Directionx4& padding = viewGroup->getPadding();
    const Size& parentSize = viewGroup->getSize();

    for (auto i = 0; i < count; ++i) {
        ref_ptr<View> view = viewGroup->getChildAt(i);
        ref_ptr<FrameLayoutParams> params = reinterpret_cast<FrameLayoutParams*>(
            &view->getLayoutParams());

        const Size& size = view->getSize();
        const bool centerHorz = sf::checkFlag(params->gravity, Gravity::CenterHorizontal);
        const bool centerVert = sf::checkFlag(params->gravity, Gravity::CenterVertical);

        int x = padding.left - padding.right
             + params->margin.left - params->margin.right;
        int y = padding.top - padding.bottom
            + params->margin.top - params->margin.bottom;

        if (centerHorz && centerVert) {
            x += (parentSize.width - view->getWidth()) / 2;
            y += (parentSize.height - view->getHeight()) / 2;
        } else if (centerHorz) {
            x += (parentSize.width - view->getWidth()) / 2;

            if (sf::checkFlag(params->gravity, Gravity::Bottom)) {
                y = parentSize.height - size.height
                    - padding.bottom - params->margin.bottom;
            } else {
                y = padding.top + params->margin.top;
            }
        } else if (centerVert) {
            y += (parentSize.height - view->getHeight()) / 2;

            if (sf::checkFlag(params->gravity, Gravity::Right)) {
                x = parentSize.width - size.width
                    - padding.right - params->margin.right;
            } else {
                x = padding.left + params->margin.left;
            }
        } else {
            if (sf::checkFlag(params->gravity, Gravity::Bottom)) {
                y = parentSize.height - size.height
                    - padding.bottom - params->margin.bottom;
            } else {
                y = padding.top + params->margin.top;
            }

            if (sf::checkFlag(params->gravity, Gravity::Right)) {
                x = parentSize.width - size.width
                    - padding.right - params->margin.right;
            } else {
                x = padding.left + params->margin.left;
            }
        }

        view->layoutTo(x, y);
    }
}
