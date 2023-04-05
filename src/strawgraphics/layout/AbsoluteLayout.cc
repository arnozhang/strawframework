#include "graphics/layout/AbsoluteLayout.h"

#include "graphics/view/AttributesHelper.h"

#include <algorithm>


sf::AbsoluteLayout::AbsoluteLayout(const Context& context)
    : ElementInherit(context) {
}

sf::AbsoluteLayout::~AbsoluteLayout() {
}

void sf::AbsoluteLayout::setAttributesInternal(Attributes* attrs) {
    ViewGroup::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
    END_ATTRS()
}

void sf::AbsoluteLayout::setLayoutType(LayoutType type) {
}

sf::LayoutSupporter* sf::AbsoluteLayout::createDefaultLayoutSupporter() const {
    return new AbsoluteLayoutSupporter();
}

void sf::AbsoluteLayout::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    measureChildren(widthSpec, heightSpec);

    int maxWidth = 0;
    int maxHeight = 0;

    for (auto& view : mChildren) {
        if (view->isGone()) {
            continue;
        }

        ref_ptr<AbsoluteLayoutParams> params = reinterpret_cast<AbsoluteLayoutParams*>(
            &view->getLayoutParams());
        int right = params->position.x + view->getWidth();
        int bottom = params->position.y + view->getHeight();

        maxWidth = std::max(maxWidth, right);
        maxHeight = std::max(maxHeight, bottom);
    }

    maxWidth += mPadding.left + mPadding.right;
    maxHeight += mPadding.top + mPadding.bottom;

    maxWidth = resolveAppropriateSize(widthSpec, mMinSize.width, maxWidth);
    maxHeight = resolveAppropriateSize(heightSpec, mMinSize.height, maxHeight);

    setMeasuredSize(maxWidth, maxHeight);
}

sf::AbsoluteLayoutSupporter* sf::AbsoluteLayout::getLayouter() const {
    if (mLayouter->getLayoutType() == AbsoluteLayout::getElemenetName()) {
        return reinterpret_cast<AbsoluteLayoutSupporter*>(
            static_cast<LayoutSupporter*>(mLayouter));
    }

    return nullptr;
}
