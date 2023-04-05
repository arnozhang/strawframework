/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/29
 */

#pragma once

#include "base/ptr/RefPtr.h"
#include "graphics/view/Element.h"
#include "graphics/window/Window.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/menu/MenuItem.h"
#include "graphics/menu/MenuListener.h"


namespace sf {


    class STRAW_FRAMEWORK_API Menu
        : public ElementInherit<Menu, Element>
        , public ListenerList<MenuListener> {

        IMPL_ELEMENT(Menu);

    public:
        Menu(const Context& context);
        virtual ~Menu();

        virtual void setAttributes(Attributes* attrs) override;
        virtual Attributes* getAttributes() const override;
        virtual ElementType getElementType() const override;

        virtual Window* getWindow() const;
        virtual Menu* getParent() const;
        virtual void attachWindow(Window* window, Menu* parent);
        virtual void addItem(BaseMenuItem* item);

        virtual void showMenu(int x, int y);
        virtual void showMenu(Point pt);
        virtual void hideMenu();

        virtual BaseMenuItem* getMenuItemById(const std::string& id) const;

    protected:
        virtual void createMenuInternal();

    private:
        void requestMenuFocus();
        void destroyMenu();
        void notifyClicked(MenuItem* item);

    private:
        ref_ptr<Window> mMenuWnd;
        ref_ptr<LinearLayout> mMenuRootLayout;
        Menu* mParent;
        std::vector<std::unique_ptr<BaseMenuItem>> mItemList;
        std::unique_ptr<Attributes> mAttrs;
        Window* mAttachWnd;
    };
}
