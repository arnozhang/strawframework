#include "DefaultCaptionView.h"

#include "graphics/context/Context.h"


sf::impl::DefaultCaptionView::DefaultCaptionView(const Context& context)
    : ElementInherit(context) {

    setOrientation(Orientation::Horizontal);

    context.getLayoutInflater().inflate(
        "@platform:layout/default_caption", this, true);
}

sf::impl::DefaultCaptionView::~DefaultCaptionView() {
}

void sf::impl::DefaultCaptionView::setAttributesInternal(Attributes* attrs) {
    LinearLayout::setAttributesInternal(attrs);

    if (!attrs) {
        return;
    }

    ref_ptr<View> minBtn = findViewById(sf::window::id_wndMinBtn);
    ref_ptr<View> maxBtn = findViewById(sf::window::id_wndMaxBtn);
    ref_ptr<View> closeBtn = findViewById(sf::window::id_wndCloseBtn);

    minBtn->setBackground(attrs->getDrawableAttr(sf::window::attr_minBtnDrawable));
    maxBtn->setBackground(attrs->getDrawableAttr(sf::window::attr_maxBtnDrawable));
    closeBtn->setBackground(attrs->getDrawableAttr(sf::window::attr_closeBtnDrawable));
}
