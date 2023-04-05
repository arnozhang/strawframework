#include "graphics/menu/BaseMenuItem.h"

#include "graphics/view/View.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/window/Window.h"
#include "graphics/menu/Menu.h"


sf::BaseMenuItem::BaseMenuItem(const Context& context)
    : ElementInherit(context) {

    mSubMenu = nullptr;
}

sf::BaseMenuItem::~BaseMenuItem() {
}

void sf::BaseMenuItem::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_RAW_STRING(id, setId)
        ATTR_DIMENSION(dividerLineHeight, setDividerLineHeight)
        ATTR_DIMENSION(dividerLineMargin, setDividerLineMargin)
        ATTR_COLOR(dividerLineColor, setDividerLineColor)
    END_ATTRS()
}

void sf::BaseMenuItem::createMenuItem() {
    mItemLayout = createItemLayout();
}

sf::View* sf::BaseMenuItem::createItemLayout() {
    View* line = new View(mContext);
    line->setSize(LayoutParams::MATCH_PARENT, 1);
    return line;
}

bool sf::BaseMenuItem::isDividerLine() const {
    return true;
}

void sf::BaseMenuItem::setId(const std::string& id) {
    mId = id;
    mItemLayout->setId(id);
}

void sf::BaseMenuItem::setDividerLineHeight(int height) {
    mItemLayout->setHeight(height);
}

void sf::BaseMenuItem::setDividerLineMargin(int margin) {
    if (isDividerLine()) {
        mItemLayout->setTopMargin(margin);
        mItemLayout->setBottomMargin(margin);
    }
}

void sf::BaseMenuItem::setDividerLineColor(SkColor color) {
    mItemLayout->setBackgroundColor(color);
}

bool sf::BaseMenuItem::isSubMenuWindow(NativeWindow wnd) const {
    if (!mSubMenu) {
        return false;
    }

    Window* window = mSubMenu->getWindow();
    return window && window->isSameWindow(wnd);
}

bool sf::BaseMenuItem::isSubMenuShown() const {
    bool s = mSubMenu->getWindow()->isShown();
    return mSubMenu && mSubMenu->getWindow() && mSubMenu->getWindow()->isShown();
}

bool sf::BaseMenuItem::isCursorInMenuItemWnd() const {
    Window* wnd = mSubMenu->getParent()->getWindow();
    Point pos = wnd->getCursorPositionInScreen();
    Rect rc = wnd->getGlobalRect();

    Rect itemRc = mItemLayout->getGlobalRect();
    itemRc.move(wnd->getPosition());
    return rc.contains(pos) && !itemRc.contains(pos);
}

void sf::BaseMenuItem::showSubMenu() const {
    if (mSubMenu) {
        mSubMenu->showMenu(getMenuItemEndPosition());
    }
}

void sf::BaseMenuItem::hideSubMenu() const {
    if (mSubMenu) {
        mSubMenu->hideMenu();
    }
}

sf::Point sf::BaseMenuItem::getMenuItemEndPosition() const {
    Point pt = mItemLayout->getWindow()->getPosition();
    Rect rc = mItemLayout->getGlobalRect();
    pt.x += rc.horzDistance();
    pt.y += rc.top;
    return pt;
}
