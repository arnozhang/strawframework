#include "graphics/view/ToggleButton.h"

#include "graphics/view/AttributesHelper.h"


sf::ToggleButton::ToggleButton(const Context& context) : ElementInherit(context) {
}

sf::ToggleButton::~ToggleButton() {

}

void sf::ToggleButton::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

        END_ATTRS()
}

void sf::ToggleButton::onDraw(SkCanvas* canvas) {

}

void sf::ToggleButton::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

