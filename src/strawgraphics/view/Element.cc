#include "graphics/view/Element.h"


sf::Element::Element(const Context& context)
    : mContext(context) {
}

sf::Element::~Element() {
}

void sf::Element::setAttributes(Attributes* attrs) {
}

sf::Attributes* sf::Element::getAttributes() const {
    return nullptr;
}

sf::ElementType sf::Element::getElementType() const {
    return sf::ElementType::Element_None;
}

const sf::Context& sf::Element::getContext() const {
    return mContext;
}
