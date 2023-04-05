#include "graphics/drawable/StateListDrawable.h"

#include "base/StrawUtils.h"
#include "graphics/view/View.h"
#include "graphics/app/DeclareHelper.h"
#include "graphics/context/Context.h"
#include "graphics/context/ElementNode.h"


sf::StateListDrawable::StateListDrawable(const Context& context)
    : ElementInherit(context) {
}

sf::StateListDrawable::~StateListDrawable() {
}

void sf::StateListDrawable::decodeByElementNode(ElementNode* node) {
    for (Serializable* iter : node->mChildren) {
        ElementNode* sub = reinterpret_cast<ElementNode*>(iter);
        const std::string& name = sub->mNodeName;
        if (name != sf::tag::ITEM) {
            straw_warning("Find invalid tag name: \"" + name
                + "\" when decode StateListDrawable!");
            continue;
        }

        Drawable* drawable = nullptr;
        Attributes& attrs = sub->mAttributes;
        if (attrs.hasAttribute(sf::tag::DRAWABLE)) {
            drawable = attrs.getDrawableAttr(sf::tag::DRAWABLE);
        } else if (!sub->mChildren.empty()) {
            drawable = mContext.decodeDrawable(
                reinterpret_cast<ElementNode*>(sub->mChildren[0]));
        }

        int state = StateNormal;
        if (attrs.hasAttribute(sf::tag::MOUSE_HOVER)) {
            state |= attrs.getBoolAttr(sf::tag::MOUSE_HOVER) ? MouseHover : UnMouseHovered;
        }

        if (attrs.hasAttribute(sf::tag::PRESSED)) {
            state |= attrs.getBoolAttr(sf::tag::PRESSED) ? Pressed : UnPressed;
        }

        if (attrs.hasAttribute(sf::tag::ENABLED)) {
            state |= attrs.getBoolAttr(sf::tag::ENABLED) ? Enabled : ViewDisabled;
        }

        if (attrs.hasAttribute(sf::tag::SELECTED)) {
            state |= attrs.getBoolAttr(sf::tag::SELECTED) ? Selected : UnSelected;
        }

        addDrawable(state, drawable);
    }
}

sf::Size sf::StateListDrawable::getRawResourceSize() const {
    Drawable* drawable = nullptr;
    if (mAttachedView) {
        const int state = mAttachedView->getViewState();
        drawable = getDrawable(state);
    } else if (!mDrawableMap.empty()) {
        drawable = mDrawableMap.begin()->drawable;
    }

    if (drawable) {
        return drawable->getRawResourceSize();
    }

    return super::getRawResourceSize();
}

void sf::StateListDrawable::addDrawable(int state, Drawable* drawable) {
    mDrawableMap.push_back(StateDrawableItem(state, drawable));
}

sf::Drawable* sf::StateListDrawable::getDrawable(int state) const {
    Drawable* normal = nullptr;

    for (auto& item : mDrawableMap) {
        if (item.state == state) {
            return item.drawable;
        }

        if (item.state == StateNormal) {
            normal = item.drawable;
        }

        if (state & item.state) {
            return item.drawable;
        }
    }

    return normal;
}

void sf::StateListDrawable::draw(SkCanvas* canvas) {
    const int state = mAttachedView->getViewState();
    Drawable* drawable = getDrawable(state);

    if (drawable) {
        drawable->setSize(mSize);
        drawable->attachView(mAttachedView);
        drawable->draw(canvas);
    }
}
