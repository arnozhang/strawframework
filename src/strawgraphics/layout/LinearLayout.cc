#include "graphics/layout/LinearLayout.h"

#include "graphics/view/AttributesHelper.h"
#include "graphics/view/Attributes.h"

#include <algorithm>


sf::LinearLayout::LinearLayout(const Context& context)
    : ElementInherit(context) {

    mWeightSum = 0;
    mGravity = Gravity::Default;
}

sf::LinearLayout::~LinearLayout() {
}

void sf::LinearLayout::setAttributesInternal(Attributes* attrs) {
    ViewGroup::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_ENUM(orientation, setOrientation, sf::Orientation)
        ATTR_ENUM(gravity, setGravity, sf::Gravity)
        ATTR_FLOAT(weightSum, setWeightSum)
    END_ATTRS()
}

void sf::LinearLayout::setLayoutType(LayoutType type) {
}

sf::LayoutSupporter* sf::LinearLayout::createDefaultLayoutSupporter() const {
    return new LinearLayoutSupporter();
}

void sf::LinearLayout::setWeightSum(float weightSum) {
    mWeightSum = weightSum;
    requestLayout();
}

void sf::LinearLayout::setGravity(Gravity gravity) {
    mGravity = gravity;
    requestLayout();
}

void sf::LinearLayout::setOrientation(Orientation orientation) {
    ensureLayouter();
    getLayouter()->setOrientation(orientation);
}

sf::Orientation sf::LinearLayout::getOrientation() {
    ensureLayouter();
    return getLayouter()->getOrientation();
}

sf::Gravity sf::LinearLayout::getGravity() const {
    return mGravity;
}

void sf::LinearLayout::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    if (getOrientation() == sf::Orientation::Vertical) {
        measureVertical(widthSpec, heightSpec);
    } else {
        measureHorizontal(widthSpec, heightSpec);
    }
}

void sf::LinearLayout::measureHorizontal(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    const int count = getChildCount();
    float totalWeight = 0;
    int totalWidth = 0;
    int maxHeight = 0;

    MeasureSpec childSpec;
    const bool selfWrapContent = mLayoutParams->isWidthWrapContent();
    for (auto i = 0; i < count; ++i) {
        View* view = getChildAt(i);
        LinearLayoutParams* params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        if (selfWrapContent && params->isWidthMatchParent()) {
            childSpec.unspecified();
        } else {
            childSpec = widthSpec;
        }

        measureChild(view, childSpec, heightSpec);
        totalWeight += params->weight;
        totalWidth += view->getSize().width + params->margin.horzSum();
        maxHeight = std::max(maxHeight, view->getSize().height + params->margin.vertSum());
    }

    int height = resolveAppropriateSize(
        heightSpec, mMinSize.height, maxHeight + mPadding.vertSum());

    int width = totalWidth + mPadding.horzSum();
    bool needAllocWeight = true;
    if (widthSpec.isExactly()) {
        width = widthSpec.dimension;
    } else {
        needAllocWeight = false;
    }

    if (mWeightSum > 0) {
        totalWeight = mWeightSum;
    }

    int remain = width - mPadding.horzSum() - totalWidth;
    const bool calcWeight = needAllocWeight && remain != 0 && totalWeight > 0;
    const float remainPerWeight = remain / totalWeight;

    MeasureSpec childWidthSpec;
    MeasureSpec childHeightSpec;

    for (auto i = 0; i < count; ++i) {
        View* view = getChildAt(i);
        LinearLayoutParams* params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        bool reMeasureChild = false;
        Size size = view->getSize();
        int newWidth = size.width;
        int newHeight = size.height;

        if (params->height == LayoutParams::MATCH_PARENT) {
            newHeight = maxHeight - view->getMargin().vertSum();
            reMeasureChild = true;
        }

        if (calcWeight && params->weight > 0) {
            newWidth = size.width + static_cast<int>(
                remainPerWeight * params->weight);
            reMeasureChild = true;
        }

        if (reMeasureChild) {
            childWidthSpec.exactly(newWidth);
            childHeightSpec.exactly(newHeight);
            view->dispatchMeasure(childWidthSpec, childHeightSpec);
        }
    }

    setMeasuredSize(
        resolveAppropriateSize(widthSpec, mMinSize.width, width),
        resolveAppropriateSize(heightSpec, mMinSize.height, height));
}

void sf::LinearLayout::measureVertical(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    const int count = getChildCount();
    float totalWeight = 0;
    int totalHeight = 0;
    int maxWidth = 0;

    MeasureSpec childSpec;
    const bool selfWrapContent = mLayoutParams->isHeightWrapContent();
    for (auto i = 0; i < count; ++i) {
        View* view = getChildAt(i);
        LinearLayoutParams* params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        if (selfWrapContent && params->isWidthMatchParent()) {
            childSpec.unspecified();
        } else {
            childSpec = heightSpec;
        }

        measureChild(view, widthSpec, childSpec);

        totalWeight += params->weight;
        totalHeight += view->getSize().height + params->margin.vertSum();
        maxWidth = std::max(maxWidth, view->getSize().width + params->margin.horzSum());
    }

    int width = resolveAppropriateSize(
        widthSpec, mMinSize.width, maxWidth + mPadding.horzSum());

    int height = totalHeight + mPadding.vertSum();
    bool needAllocWeight = true;
    if (heightSpec.isExactly()) {
        height = heightSpec.dimension;
    } else {
        needAllocWeight = false;
    }

    if (mWeightSum > 0) {
        totalWeight = mWeightSum;
    }

    int remain = height - mPadding.vertSum() - totalHeight;
    const bool calcWeight = needAllocWeight && remain != 0 && totalWeight > 0;
    const float remainPerWeight = remain / totalWeight;

    MeasureSpec childWidthSpec;
    MeasureSpec childHeightSpec;

    for (auto i = 0; i < count; ++i) {
        View* view = getChildAt(i);
        LinearLayoutParams* params = reinterpret_cast<LinearLayoutParams*>(
            &view->getLayoutParams());

        bool reMeasureChild = false;
        Size size = view->getSize();
        int newWidth = size.width;
        int newHeight = size.height;

        if (params->width == LayoutParams::MATCH_PARENT) {
            newWidth = maxWidth - view->getMargin().horzSum();
            reMeasureChild = true;
        }

        if (calcWeight && params->weight > 0) {
            newHeight = size.height + static_cast<int>(
                remainPerWeight * params->weight);
            reMeasureChild = true;
        }

        if (reMeasureChild) {
            childWidthSpec.exactly(newWidth);
            childHeightSpec.exactly(newHeight);
            view->dispatchMeasure(childWidthSpec, childHeightSpec);
        }
    }

    setMeasuredSize(
        resolveAppropriateSize(widthSpec, mMinSize.width, width),
        resolveAppropriateSize(heightSpec, mMinSize.height, height));
}

sf::LinearLayoutSupporter* sf::LinearLayout::getLayouter() const {
    if (mLayouter->getLayoutType() == LinearLayout::getElemenetName()) {
        return reinterpret_cast<LinearLayoutSupporter*>(
            static_cast<LayoutSupporter*>(mLayouter));
    }

    return nullptr;
}
