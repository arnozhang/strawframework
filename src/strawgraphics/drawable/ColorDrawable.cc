#include "graphics/drawable/ColorDrawable.h"

#include "graphics/view/AttributesHelper.h"


sf::ColorDrawable::ColorDrawable(const Context& context)
    : ColorDrawable(context, SK_ColorBLACK) {
}

sf::ColorDrawable::ColorDrawable(const Context& context, SkColor color)
    : ElementInherit(context) {

    mColor = color;
}

sf::ColorDrawable::~ColorDrawable() {
}

void sf::ColorDrawable::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_COLOR(color, setColor)
    END_ATTRS()
}

void sf::ColorDrawable::setColor(SkColor color) {
    mColor = color;
}

void sf::ColorDrawable::draw(SkCanvas* canvas) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(mColor);
    canvas->drawRect(SkRect::MakeWH(mSize.width, mSize.height), paint);
}
