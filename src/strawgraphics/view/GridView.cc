#include "graphics/view/GridView.h"

#include "graphics/view/AttributesHelper.h"


sf::GridView::GridView(const Context& context) : ElementInherit(context) {
}

sf::GridView::~GridView() {

}

void sf::GridView::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::GridView::onDraw(SkCanvas* canvas) {

}

void sf::GridView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

