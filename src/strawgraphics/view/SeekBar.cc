#include "graphics/view/SeekBar.h"

#include "graphics/view/AttributesHelper.h"


sf::SeekBar::SeekBar(const Context& context) : ElementInherit(context) {
}

sf::SeekBar::~SeekBar() {

}

void sf::SeekBar::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)

    END_ATTRS()
}

void sf::SeekBar::onDraw(SkCanvas* canvas) {

}

void sf::SeekBar::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}

