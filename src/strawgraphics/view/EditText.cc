#include "graphics/view/EditText.h"

#include "graphics/view/AttributesHelper.h"


sf::EditText::EditText(const Context& context) : ElementInherit(context) {
}

sf::EditText::~EditText() {

}

void sf::EditText::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::EditText::onDraw(SkCanvas* canvas) {

}

void sf::EditText::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

