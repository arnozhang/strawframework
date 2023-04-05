#include "graphics/view/ListView.h"

#include "graphics/view/AttributesHelper.h"


sf::ListView::ListView(const Context& context) : ElementInherit(context) {
}

sf::ListView::~ListView() {

}

void sf::ListView::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::ListView::onDraw(SkCanvas* canvas) {

}

void sf::ListView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

