#include "graphics/drawable/Drawable.h"

#include "graphics/view/View.h"


sf::Drawable::Drawable(const Context& context)
    : ElementInherit(context) {

    mAttachedView = nullptr;
    mVisible = false;
}

sf::Drawable::~Drawable() {
}

sf::ElementType sf::Drawable::getElementType() const {
    return sf::ElementType::Element_Drawable;
}

void sf::Drawable::decodeByElementNode(ElementNode* node) {
}

sf::Size sf::Drawable::getSize() const {
    return mSize;
}

void sf::Drawable::setSize(Size size) {
    mSize = size;
}

sf::Size sf::Drawable::getRawResourceSize() const {
    return Size(0, 0);
}

void sf::Drawable::attachView(View* view) {
    mAttachedView = view;

    if (mAttachedView) {
        setVisible(mAttachedView->isVisible());
    }
}

sf::View* sf::Drawable::detachView() {
    View* view = mAttachedView;
    mAttachedView = nullptr;
    setVisible(false);
    return view;
}

void sf::Drawable::setVisible(bool visible) {
    bool changed = mVisible != visible;
    mVisible = visible;

    if (changed) {
        onVisibilityChanged();
    }
}

void sf::Drawable::onVisibilityChanged() {
}
