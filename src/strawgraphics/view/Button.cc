#include "graphics/view/Button.h"

sf::Button::Button(const Context& context)
    : ElementInherit(context) {

    setClickable(true);
}

sf::Button::~Button() {
}
