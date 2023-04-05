#include "graphics/view/RootView.h"

#include "graphics/window/Window.h"


sf::RootView::RootView(Window* window)
    : ViewGroup(window->getContext()) {

    mWindow = window;
}

sf::RootView::~RootView() {
}

void sf::RootView::requestLayout() {
    ViewGroup::requestLayout();

    if (!isViewLocked()) {
        mWindow->scheduleLayout();
    }
}

void sf::RootView::invalidate(
    Rect* dirty /*= nullptr*/, bool immediately /*= false*/) {

    ViewGroup::invalidate(dirty, immediately);

    if (!isViewLocked()) {
        mWindow->scheduleInvalidate(dirty, immediately);
    }
}

sf::Window* sf::RootView::getWindow() const {
    return mWindow;
}

void sf::RootView::addView(View* view, int index, LayoutParams* params) {
    if (getChildCount() > 0) {
        return;
    }

    ViewGroup::addView(view, index, params);
}

void sf::RootView::addView(View* view) {
    ViewGroup::addView(view);
}

void sf::RootView::addView(View* view, int index) {
    ViewGroup::addView(view, index);
}

void sf::RootView::addView(View* view, LayoutParams* params) {
    ViewGroup::addView(view, params);
}

void sf::RootView::dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    measureChildren(widthSpec, heightSpec);

    if (widthSpec.isExactly() && heightSpec.isExactly()) {
        setMeasuredSize(widthSpec.dimension, heightSpec.dimension);
        return;
    }

    int width = 0;
    int height = 0;
    for (auto& view : mChildren) {
        Size size = view->getSize();
        if (size.width > width) {
            width = size.width;
        }

        if (size.height > height) {
            height = size.height;
        }
    }

    setMeasuredSize(
        resolveAppropriateSize(widthSpec, 0, width),
        resolveAppropriateSize(heightSpec, 0, height));
}

void sf::RootView::dispatchLayout() {
    for (auto& view : mChildren) {
        view->layoutTo(0, 0);
        view->dispatchLayout();
    }
}
