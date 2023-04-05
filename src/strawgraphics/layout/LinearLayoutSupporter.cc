#include "graphics/layout/LinearLayoutSupporter.h"

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/view/AttributesHelper.h"


sf::LinearLayoutParams::LinearLayoutParams() {
    weight = 0.0f;
    gravity = sf::Gravity::Default;
}

sf::LinearLayoutParams::LinearLayoutParams(const LayoutParams* rhs)
    : LinearLayoutParams() {

    if (rhs) {
        *this = *rhs;
    }
}

sf::LinearLayoutParams::LinearLayoutParams(const LinearLayoutParams& rhs) {
    *this = rhs;
}

sf::LinearLayoutParams::LinearLayoutParams(const LayoutParams& rhs) {
    *this = rhs;
}

sf::LinearLayoutParams::~LinearLayoutParams() {
}

sf::LayoutType sf::LinearLayoutParams::getLayoutType() const {
    return sf::LinearLayout::getElemenetName();
}

void sf::LinearLayoutParams::setAttributes(Attributes* attrs) {
    LayoutParams::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_FLOAT(weight, setWeight)
        ATTR_ENUM(layoutGravity, setLayoutGravity, Gravity)
    END_ATTRS()
}

void sf::LinearLayoutParams::setLayoutGravity(int _gravity) {
    gravity = _gravity;
}

void sf::LinearLayoutParams::setWeight(float _weight) {
    weight = _weight;
}

sf::LinearLayoutParams& sf::LinearLayoutParams::operator=(const LinearLayoutParams& rhs) {
    LayoutParams::operator= (rhs);

    weight = rhs.weight;
    gravity = rhs.gravity;
    return *this;
}

sf::LinearLayoutParams& sf::LinearLayoutParams::operator=(const LayoutParams& rhs) {
    LayoutParams::operator= (rhs);
    return *this;
}


sf::LinearLayoutSupporter::LinearLayoutSupporter() {
    mOrientation = Horizontal;
}

sf::LinearLayoutSupporter::~LinearLayoutSupporter() {
}

sf::LayoutType sf::LinearLayoutSupporter::getLayoutType() const {
    return sf::LinearLayout::getElemenetName();
}

sf::LayoutParams* sf::LinearLayoutSupporter::createDefaultLayoutParams(
    LayoutParams* params /*= 0*/) {

    LinearLayoutParams* result = new sf::LinearLayoutParams(params);
    if (!params && mOrientation == sf::Orientation::Horizontal) {
        result->width = sf::LayoutParams::WRAP_CONTENT;
        result->height = sf::LayoutParams::MATCH_PARENT;
    }

    return result;
}

void sf::LinearLayoutSupporter::setOrientation(Orientation orientation) {
    mOrientation = orientation;
}

sf::Orientation sf::LinearLayoutSupporter::getOrientation() const {
    return mOrientation;
}

void sf::LinearLayoutSupporter::layout(ViewGroup* viewGroup) {
    ref_ptr<LinearLayout> linearLayout = 
        reinterpret_cast<LinearLayout*>(viewGroup);

    if (mOrientation == Vertical) {
        layoutVertical(linearLayout);
    } else {
        layoutHorizontal(linearLayout);
    }
}

void sf::LinearLayoutSupporter::layoutHorizontal(LinearLayout* viewGroup) {
    const int count = viewGroup->getChildCount();
    if (count <= 0) {
        return;
    }

    const Directionx4& padding = viewGroup->getPadding();
    const Size& size = viewGroup->getSize();

    int vertGravity = Gravity::Default;
    const int layoutGravity = viewGroup->getGravity();
    if (sf::checkFlag(layoutGravity, Gravity::Top)) {
        vertGravity = Gravity::Top;
    } else if (sf::checkFlag(layoutGravity, Gravity::Bottom)) {
        vertGravity = Gravity::Bottom;
    } else if (sf::checkFlag(layoutGravity, Gravity::CenterVertical)) {
        vertGravity = Gravity::CenterVertical;
    }

    int offset = padding.left;

    const bool hasLeft = sf::checkFlag(layoutGravity, Gravity::Left);
    const bool hasRight = sf::checkFlag(layoutGravity, Gravity::Right);
    const bool hasCenterHorz = sf::checkFlag(layoutGravity, Gravity::CenterHorizontal);
    if (!hasLeft && (hasRight || hasCenterHorz)) {
        int totalWidth = 0;
        for (auto i = 0; i < count; ++i) {
            ref_ptr<View> view = viewGroup->getChildAt(i);
            totalWidth += view->getWidth() + view->getMargin().horzSum();
        }

        if (hasRight) {
            offset = size.width - totalWidth - padding.right;
        } else if (hasCenterHorz) {
            offset = (size.width - totalWidth) / 2 + padding.left - padding.right;
        }
    }

    for (auto i = 0; i < count; ++i) {
        ref_ptr<View> view = viewGroup->getChildAt(i);
        if (view->isGone()) {
            continue;
        }

        ref_ptr<LinearLayoutParams> params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        const Size& viewSize = view->getSize();
        int gravity = params->gravity;
        gravity &= (~Gravity::Left | ~Gravity::Right | ~Gravity::CenterHorizontal);
        if (vertGravity != 0) {
            if (!sf::checkFlag(gravity, Gravity::Top)
                && !sf::checkFlag(gravity, Gravity::Bottom)
                && !sf::checkFlag(gravity, Gravity::CenterVertical)) {
                gravity |= vertGravity;
            }
        }

        int yPosition = 0;
        if (sf::checkFlag(gravity, Gravity::Bottom)) {
            yPosition = size.height - viewSize.height
                - padding.bottom - params->margin.bottom;
        } else if(sf::checkFlag(gravity, Gravity::CenterVertical)) {
            yPosition = (size.height - viewSize.height) / 2
                + padding.top + params->margin.top
                - padding.bottom - params->margin.bottom;
        } else {
            yPosition = padding.top + params->margin.top;
        }

        offset += view->getLeftMargin();
        view->layoutTo(offset, yPosition);

        offset += view->getWidth() + view->getRightMargin();
    }
}

void sf::LinearLayoutSupporter::layoutVertical(LinearLayout* viewGroup) {
    const int count = viewGroup->getChildCount();
    if (count <= 0) {
        return;
    }

    const Directionx4& padding = viewGroup->getPadding();
    const Size& size = viewGroup->getSize();

    int vertGravity = Gravity::Default;
    const int layoutGravity = viewGroup->getGravity();
    if (sf::checkFlag(layoutGravity, Gravity::Left)) {
        vertGravity = Gravity::Left;
    } else if (sf::checkFlag(layoutGravity, Gravity::Right)) {
        vertGravity = Gravity::Right;
    } else if (sf::checkFlag(layoutGravity, Gravity::CenterHorizontal)) {
        vertGravity = Gravity::CenterHorizontal;
    }

    int offset = padding.top;

    const bool hasTop = sf::checkFlag(layoutGravity, Gravity::Top);
    const bool hasBottom = sf::checkFlag(layoutGravity, Gravity::Bottom);
    const bool hasCenterVert = sf::checkFlag(layoutGravity, Gravity::CenterVertical);
    if (!hasTop && (hasBottom || hasCenterVert)) {
        int totalHeight = 0;
        for (auto i = 0; i < count; ++i) {
            ref_ptr<View> view = viewGroup->getChildAt(i);
            totalHeight += view->getHeight() + view->getMargin().vertSum();
        }

        if (hasBottom) {
            offset = size.height - totalHeight - padding.bottom;
        } else if (hasCenterVert) {
            offset = (size.height - totalHeight) / 2 + padding.top - padding.bottom;
        }
    }

    for (auto i = 0; i < count; ++i) {
        ref_ptr<View> view = viewGroup->getChildAt(i);
        if (view->isGone()) {
            continue;
        }

        ref_ptr<LinearLayoutParams> params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        const Size& viewSize = view->getSize();
        int gravity = params->gravity;
        gravity &= (~Gravity::Top | ~Gravity::Bottom | ~Gravity::CenterVertical);
        if (vertGravity != 0) {
            if (!sf::checkFlag(gravity, Gravity::Left)
                && !sf::checkFlag(gravity, Gravity::Right)
                && !sf::checkFlag(gravity, Gravity::CenterHorizontal)) {
                gravity |= vertGravity;
            }
        }

        int xPosition = 0;
        if (sf::checkFlag(gravity, Gravity::Right)) {
            xPosition = size.width - viewSize.width
                - padding.right - params->margin.right;
        } else if (sf::checkFlag(gravity, Gravity::CenterHorizontal)) {
            xPosition = (size.width - viewSize.width) / 2
                + padding.left + params->margin.left
                - padding.right - params->margin.right;
        } else {
            xPosition = padding.left + params->margin.left;
        }

        offset += view->getTopMargin();
        view->layoutTo(xPosition, offset);

        offset += view->getHeight() + view->getBottomMargin();
    }
}
