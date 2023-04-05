#include "graphics/menu/MenuItem.h"

#include "graphics/context/Context.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/view/TextView.h"
#include "graphics/view/AttributesHelper.h"


sf::MenuItem::MenuItem(const Context& context)
    : ElementInherit(context) {
    mAccelKeyCode = 0;
    mIsChecked = false;
}

sf::MenuItem::~MenuItem() {
}

void sf::MenuItem::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_STRING(title, setTitle)
        ATTR_DRAWABLE(icon, setIcon)
        ATTR_STRING(accelKey, setAccelKey)
        ATTR_STRING(grayTips, setGrayTips)
        ATTR_DIMENSION(itemHeight, setItemHeight)
        ATTR_DIMENSION(menuTextSize, setMenuTextSize)
        ATTR_STATE_COLOR(grayTipsColor, setGrayTipsColor)
        ATTR_STATE_COLOR(menuTitleColor, setMenuTitleColor)
        ATTR_DRAWABLE(menuItemBackground, setMenuItemBackground)
    END_ATTRS()
}

bool sf::MenuItem::isDividerLine() const {
    return false;
}

sf::View* sf::MenuItem::createItemLayout() {
    return mContext.getLayoutInflater().inflate(
        "@platform:layout/default_menu_item");
}

void sf::MenuItem::setTitle(const std::string& title) {
    mTitle = title;
    TextView* view = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuTitle));
    view->setText(mTitle);
}

void sf::MenuItem::setGrayTips(const std::string& grayTips) {
    mGrayTips = grayTips;
    TextView* view = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuGrayTips));
    view->setText(mGrayTips);
}

void sf::MenuItem::setIcon(Drawable* drawable) {
    View* view = mItemLayout->findViewById(sf::menu::id_menuIcon);
    view->setBackground(drawable);
}

void sf::MenuItem::setAccelKey(const std::string& key) {
    if (key.empty()) {
        mAccelKeyCode = 0;
    }

    char first = key[0];
    mAccelKeyCode = ::toupper(first);
}

void sf::MenuItem::setMenuTextSize(int textSize) {
    TextView* title = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuTitle));
    TextView* grayTips = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuGrayTips));

    title->setTextSize(textSize);
    grayTips->setTextSize(textSize);
}

void sf::MenuItem::setMenuItemBackground(Drawable* drawable) {
    mItemLayout->setBackground(drawable);
}

void sf::MenuItem::setMenuTitleColor(StateListColor* color) {
    TextView* view = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuTitle));
    view->setTextStateColor(color);
}

void sf::MenuItem::setGrayTipsColor(StateListColor* color) {
    TextView* view = reinterpret_cast<TextView*>(
        mItemLayout->findViewById(sf::menu::id_menuGrayTips));
    view->setTextStateColor(color);
}

void sf::MenuItem::setEnable(bool enabled) {
    mItemLayout->setEnable(enabled);
}

void sf::MenuItem::setItemHeight(int height) {
    mItemLayout->setHeight(height);
}

void sf::MenuItem::setIsChecked(bool checked) {
    mIsChecked = checked;
}
