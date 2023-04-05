#include "graphics/menu/Menu.h"

#include "graphics/view/Attributes.h"


sf::Menu::Menu(const Context& context)
    : ElementInherit(context) {
    mAttachWnd = nullptr;
}

sf::Menu::~Menu() {
    destroyMenu();
}

void sf::Menu::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    mAttrs.reset(attrs);
    mMenuRootLayout->setAttributes(attrs);
}

sf::Attributes* sf::Menu::getAttributes() const {
    return mAttrs.get();
}

sf::ElementType sf::Menu::getElementType() const {
    return ElementType::Element_Menu;
}

sf::Window* sf::Menu::getWindow() const {
    return mMenuWnd.get();
}

sf::Menu* sf::Menu::getParent() const {
    return mParent;
}

void sf::Menu::attachWindow(Window* window, Menu* parent) {
    mParent = parent;
    mAttachWnd = window;

    mMenuRootLayout = new LinearLayout(mContext);
    mMenuRootLayout->setOrientation(Orientation::Vertical);
    mMenuRootLayout->setWidth(LayoutParams::WRAP_CONTENT);
    mMenuRootLayout->setHeight(LayoutParams::WRAP_CONTENT);

    createMenuInternal();
}

void sf::Menu::createMenuInternal() {
    mMenuWnd = new Window(mContext);
    ViewUpdateLock lock(mMenuWnd->getRootView());

    mMenuWnd->create(mAttachWnd);
    mMenuWnd->setWidthWrapContent(true);
    mMenuWnd->setHeightWrapContent(true);
    mMenuWnd->addListener(WindowListener::create(
        &WindowListener::onKillFocus, [this](NativeWindow gotFocusWnd) {

        for (auto& item : mItemList) {
            if (item->isSubMenuWindow(gotFocusWnd)) {
                return;
            }
        }

        if (mParent) {
            hideMenu();
            mParent->requestMenuFocus();
        } else {
            destroyMenu();
        }
    }));

    mMenuWnd->setContentView(mMenuRootLayout);

    for (auto listener : mListeners) {
        listener->onMenuCreated(this);
    }
}

void sf::Menu::requestMenuFocus() {
    mMenuWnd->requestFocus();
}

void sf::Menu::destroyMenu() {
    mMenuWnd->close();
    mMenuWnd = nullptr;
    mMenuRootLayout = nullptr;
    mItemList.clear();

    for (auto listener : mListeners) {
        listener->onMenuDestroyed();
    }
}

void sf::Menu::notifyClicked(MenuItem* item) {
    for (auto listener : mListeners) {
        listener->onMenuItemClicked(item);
    }

    if (mParent) {
        mParent->notifyClicked(item);
    }
}

void sf::Menu::showMenu(int x, int y) {
    if (!mMenuWnd) {
        createMenuInternal();
    }

    mMenuWnd->setPosition(x, y);
    mMenuWnd->show();

    for (auto listener : mListeners) {
        listener->onMenuShown();
    }
}

void sf::Menu::showMenu(Point pt) {
    showMenu(pt.x, pt.y);
}

void sf::Menu::hideMenu() {
    mMenuWnd->hide();

    for (auto listener : mListeners) {
        listener->onMenuHidden();
    }
}

sf::BaseMenuItem* sf::Menu::getMenuItemById(const std::string& id) const {
    for (auto& item: mItemList) {
        if (item->mId == id) {
            return item.get();
        }
    }

    for (auto& item : mItemList) {
        if (item->mSubMenu) {
            BaseMenuItem* sub = item->mSubMenu->getMenuItemById(id);
            if (sub) {
                return sub;
            }
        }
    }

    return nullptr;
}

void sf::Menu::addItem(BaseMenuItem* item) {
    if (!item) {
        return;
    }

    mItemList.push_back(std::unique_ptr<BaseMenuItem>(item));
    item->setAttributes(mAttrs.get());
    mMenuRootLayout->addView(item->mItemLayout);

    if (!item->isDividerLine()) {
        MenuItem* menuItem = reinterpret_cast<MenuItem*>(item);
        ViewListener& listener = ViewListener::create();

        if (menuItem->mSubMenu) {
            listener.bind(&ViewListener::onMouseHover, [menuItem]() {
                menuItem->showSubMenu();
            });

            listener.bind(&ViewListener::onMouseLeave, [menuItem]() {
                if (menuItem->isSubMenuShown() && menuItem->isCursorInMenuItemWnd()) {
                    menuItem->hideSubMenu();
                }
            });
        } else {
            listener.bind(&ViewListener::onClicked, [this, menuItem](View*) {
                notifyClicked(menuItem);
            });
        }

        menuItem->mItemLayout->addListener(listener);
    }
}
