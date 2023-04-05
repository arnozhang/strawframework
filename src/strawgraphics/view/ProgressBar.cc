#include "graphics/view/ProgressBar.h"

#include "graphics/view/AttributesHelper.h"


sf::ProgressBar::ProgressBar(const Context& context) : ElementInherit(context) {
}

sf::ProgressBar::~ProgressBar() {

}

void sf::ProgressBar::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::ProgressBar::onDraw(SkCanvas* canvas) {

}

void sf::ProgressBar::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

