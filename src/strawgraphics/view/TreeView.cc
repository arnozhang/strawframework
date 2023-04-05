#include "graphics/view/TreeView.h"

#include "graphics/view/AttributesHelper.h"


sf::TreeView::TreeView(const Context& context) : ElementInherit(context) {
}

sf::TreeView::~TreeView() {

}

void sf::TreeView::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

        END_ATTRS()
}

void sf::TreeView::onDraw(SkCanvas* canvas) {

}

void sf::TreeView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

