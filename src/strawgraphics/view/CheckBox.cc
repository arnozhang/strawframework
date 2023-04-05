#include "graphics/view/CheckBox.h"

#include "graphics/view/AttributesHelper.h"


sf::CheckBox::CheckBox(const Context& context) : ElementInherit(context) {
    mIsChecked = false;
}

sf::CheckBox::~CheckBox() {
}

void sf::CheckBox::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_STRING(text, setText)
        ATTR_BOOL(checked, setChecked)
    END_ATTRS()
}

void sf::CheckBox::setChecked(bool checked) {
    mIsChecked = checked;
    invalidateSelf(true);
}

void sf::CheckBox::setText(const std::string& text) {
    mText = text;
    requestLayout();
}

const std::string& sf::CheckBox::getText() const {
    return mText;
}

bool sf::CheckBox::isChecked() const {
    return mIsChecked;
}

void sf::CheckBox::addCheckBoxListener(CheckBoxListener& listener) {
    mCheckBoxListeners.addListener(listener);
}

void sf::CheckBox::onDraw(SkCanvas* canvas) {

}

void sf::CheckBox::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {

}
