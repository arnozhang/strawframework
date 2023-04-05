#include "graphics/view/ComboBox.h"

#include "graphics/view/AttributesHelper.h"


sf::ComboBox::ComboBox(const Context& context) : ElementInherit(context) {
}

sf::ComboBox::~ComboBox() {

}

void sf::ComboBox::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::ComboBox::onDraw(SkCanvas* canvas) {

}

void sf::ComboBox::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

