#include "graphics/view/ViewGroup.h"

#include <algorithm>

#include "base/StrawUtils.h"
#include "graphics/view/Attributes.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/layout/FrameLayout.h"
#include "graphics/layout/AbsoluteLayout.h"


sf::ViewGroup::ViewGroup(const Context& context)
    : ElementInherit(context)
    , ExtensionHost(this) {
}

sf::ViewGroup::~ViewGroup() {
}

void sf::ViewGroup::setAttributesInternal(Attributes* attrs) {
    super::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_STRING(layoutType, setLayoutType)
    END_ATTRS()
}

void sf::ViewGroup::setLayoutType(LayoutType type) {
    if (type == sf::LinearLayout::getElemenetName()) {
        mLayouter = new LinearLayoutSupporter();
    } else if (type == sf::FrameLayout::getElemenetName()) {
        mLayouter = new FrameLayoutSupporter();
    } else if (type == sf::AbsoluteLayout::getElemenetName()) {
        mLayouter = new AbsoluteLayoutSupporter();
    } else {
        mLayouter = createDefaultLayoutSupporter();
    }
}

sf::LayoutParams* sf::ViewGroup::createDefaultLayoutParams(
    LayoutParams* params /*= nullptr*/) {

    ensureLayouter();
    if (params && params->getLayoutType() == mLayouter->getLayoutType()) {
        return params;
    }

    return mLayouter->createDefaultLayoutParams(params);
}

sf::LayoutSupporter* sf::ViewGroup::createDefaultLayoutSupporter() const {
    return new LinearLayoutSupporter();
}

bool sf::ViewGroup::canAddChild() const {
    return true;
}

sf::ElementType sf::ViewGroup::getElementType() const {
    return sf::ElementType::Element_ViewGroup;
}

sf::View* sf::ViewGroup::findViewById(const std::string& id) const {
    sf::View* view = super::findViewById(id);
    if (view) {
        return view;
    }

    for (auto& view : mChildren) {
        if (view->getId() == id) {
            return view;
        } else if (view->canAddChild()) {
            ViewGroup* group = reinterpret_cast<ViewGroup*>(static_cast<View*>(view));
            if (auto ptr = group->findViewById(id)) {
                return ptr;
            }
        }
    }

    return nullptr;
}

sf::View* sf::ViewGroup::getChildAt(int index) const {
    if (index < 0 || index >= getChildCount()) {
        return nullptr;
    }

    return mChildren[index];
}

int sf::ViewGroup::getChildCount() const {
    return mChildren.size();
}

void sf::ViewGroup::addView(View* view) {
    addView(view, getChildCount(), nullptr);
}

void sf::ViewGroup::addView(View* view, int index) {
    addView(view, index, nullptr);
}

void sf::ViewGroup::addView(View* view, LayoutParams* params) {
    addView(view, getChildCount(), params);
}

void sf::ViewGroup::addView(View* view, int index, LayoutParams* params) {
    if (!view) {
        return;
    }

    if (view->getParent()) {
        straw_warning("View already has Parent!");
        return;
    }

    if (!params) {
        params = &view->getLayoutParams();
    }

    params = createDefaultLayoutParams(params);
    view->setLayoutParams(params);

    if (index < 0 || index > getChildCount()) {
        index = getChildCount();
    }

    view->setParent(this);
    mChildren.insert(mChildren.begin() + index, view);
    requestLayout();
}

void sf::ViewGroup::removeView(View* view) {
    for (auto i = 0; i < getChildCount(); ++i) {
        if (view == mChildren[i]) {
            removeViewAt(i);
        }
    }
}

void sf::ViewGroup::removeViewAt(int index) {
    if (index < 0 || index >= getChildCount()) {
        return;
    }

    mChildren.erase(mChildren.begin() + index);
    requestLayout();
}

void sf::ViewGroup::removeAllViews() {
    mChildren.clear();
    requestLayout();
}

void sf::ViewGroup::onDraw(SkCanvas* canvas) {
    super::onDraw(canvas);

    for (auto& view : mChildren) {
        view->dispatchDraw(canvas);
    }
}

void sf::ViewGroup::dispatchLayout() {
    ensureLayouter();

    mLayouter->layout(this);
    for (auto& view : mChildren) {
        view->dispatchLayout();
    }
}

void sf::ViewGroup::dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    onMeasure(widthSpec, heightSpec);
}

void sf::ViewGroup::invalidateSelf(bool immediately /* = false */) {
    invalidateAll();
    super::invalidateSelf(immediately);
}

void sf::ViewGroup::invalidateAll() {
    super::invalidateAll();

    for (auto& view : mChildren) {
        view->invalidateAll();
    }
}

void sf::ViewGroup::invalidateChild(View* child, Rect* dirty, bool immediately) {
    if (dirty && !dirty->empty() && !mChildren.empty()) {
        Rect localDirty = *dirty;
        Point pos = child->getPosition();
        int xOffset = dirty->left - pos.x;
        int yOffset = dirty->top - pos.y;

        localDirty.move(-xOffset, -yOffset);

        bool find = false;
        for (auto& view : mChildren) {
            if (find) {
                Rect bounds = view->getLocalRect();
                if (localDirty.isIntersect(bounds)) {
                    view->markInvalidate();
                    localDirty.merge(bounds);
                }

                continue;
            }

            if (child == view) {
                find = true;
            }
        }

        if (find) {
            *dirty = localDirty.move(xOffset, yOffset);
        }
    }

    invalidate(dirty, immediately);
}

void sf::ViewGroup::measureChildren(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    for (auto& view : mChildren) {
        measureChild(view, widthSpec, heightSpec);
    }
}

void sf::ViewGroup::measureChild(
    View* view, MeasureSpec widthSpec, MeasureSpec heightSpec) {

    if (view->isGone()) {
        view->setMeasuredSize(0, 0);
        return;
    }

    const LayoutParams& params = view->getLayoutParams();
    view->dispatchMeasure(
        getChildMeasureSpec(widthSpec, mPadding.horzSum() + params.margin.horzSum(), params.width),
        getChildMeasureSpec(heightSpec, mPadding.vertSum() + params.margin.vertSum(), params.height));
}

sf::MeasureSpec sf::ViewGroup::getChildMeasureSpec(
    MeasureSpec parent, int padding, int childDimension) {

    const int size = std::max(0, parent.dimension - padding);
    MeasureMode mode = Unspecified;
    int dimension = 0;

    if (childDimension >= 0) {
        mode = Exactly;
        dimension = childDimension;
    } else {
        if (parent.isExactly()) {
            if (childDimension == LayoutParams::MATCH_PARENT) {
                mode = Exactly;
                dimension = size;
            } else if (childDimension == LayoutParams::WRAP_CONTENT) {
                mode = AtMost;
                dimension = size;
            }
        } else if (parent.isAtMost()) {
            mode = AtMost;
            dimension = size;
        } else if (parent.isUnspecified()) {
            mode = Unspecified;
            dimension = size;
        }
    }

    return MeasureSpec::make(mode, dimension);
}

void sf::ViewGroup::ensureLayouter() {
    if (!mLayouter) {
        mLayouter = createDefaultLayoutSupporter();
    }
}
