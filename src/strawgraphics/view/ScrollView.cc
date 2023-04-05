#include "graphics/view/ScrollView.h"

#include "graphics/view/AttributesHelper.h"


sf::ScrollView::ScrollView(const Context& context) : ElementInherit(context) {
}

sf::ScrollView::~ScrollView() {

}

void sf::ScrollView::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::ScrollView::onDraw(SkCanvas* canvas) {

}

void sf::ScrollView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

