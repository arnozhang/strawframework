#include "graphics/layout/FrameLayout.h"

#include "graphics/view/AttributesHelper.h"
#include "graphics/view/Attributes.h"


sf::FrameLayout::FrameLayout(const Context& context)
    : ElementInherit(context) {
}

sf::FrameLayout::~FrameLayout() {
}

void sf::FrameLayout::setAttributesInternal(Attributes* attrs) {
    ViewGroup::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
    END_ATTRS()
}

void sf::FrameLayout::setLayoutType(LayoutType type) {
}

sf::LayoutSupporter* sf::FrameLayout::createDefaultLayoutSupporter() const {
    return new FrameLayoutSupporter();
}

void sf::FrameLayout::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    measureChildren(widthSpec, heightSpec);

    int width = 0;
    int height = 0;

    for (auto& child : mChildren) {
        Size size = child->getSize();
        if (size.width > width) {
            width = size.width;
        }

        if (size.height > height) {
            height = size.height;
        }
    }

    width = resolveAppropriateSize(widthSpec, mMinSize.width, width);
    height = resolveAppropriateSize(heightSpec, mMinSize.height, height);
    setMeasuredSize(width, height);
}

sf::FrameLayoutSupporter* sf::FrameLayout::getLayouter() const {
    if (mLayouter->getLayoutType() == FrameLayout::getElemenetName()) {
        return reinterpret_cast<FrameLayoutSupporter*>(
            static_cast<LayoutSupporter*>(mLayouter));
    }

    return nullptr;
}
