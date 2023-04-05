#include "graphics/color/StateListColor.h"

#include "base/StrawUtils.h"
#include "graphics/view/View.h"
#include "graphics/context/ElementNode.h"
#include "graphics/app/DeclareHelper.h"


sf::StateListColor::StateListColor(const Context& context)
    : ElementInherit(context) {
}

sf::StateListColor::StateListColor(const Context& context, SkColor color)
    : ElementInherit(context) {
    mColorMap[StateNormal] = color;
}

sf::StateListColor::~StateListColor() {
}

sf::ElementType sf::StateListColor::getElementType() const {
    return ElementType::Element_Color;
}

void sf::StateListColor::addColor(int state, SkColor color) {
    mColorMap[state] = color;
}

SkColor sf::StateListColor::getColor(int state) const {
    SkColor color = SK_ColorTRANSPARENT;
    auto iter = mColorMap.find(state);
    if (iter != mColorMap.end()) {
        color = iter->second;
    } else {
        SkColor normal = SK_ColorTRANSPARENT;
        for (auto& ref : mColorMap) {
            if (ref.first == StateNormal) {
                normal = ref.second;
            }

            if (state & ref.first) {
                color = ref.second;
                break;
            }
        }

        if (!color) {
            color = normal;
        }
    }

    return color;
}

void sf::StateListColor::decodeByElementNode(ElementNode* node) {
    for (Serializable* iter : node->mChildren) {
        ElementNode* sub = reinterpret_cast<ElementNode*>(iter);
        const std::string& name = sub->mNodeName;
        if (name != sf::tag::ITEM) {
            straw_warning("Find invalid tag name: \"" + name
                + "\" when decode StateListColor!");
            continue;
        }

        SkColor color = SK_ColorTRANSPARENT;
        Attributes& attrs = sub->mAttributes;
        if (attrs.hasAttribute(sf::tag::COLOR)) {
            color = attrs.getColorAttr(sf::tag::COLOR);
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

        mColorMap[state] = color;
    }
}
