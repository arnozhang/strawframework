/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/29
 */

#pragma once

#include <memory>

#include "base/def/PlatformDefine.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/window/BaseWindow.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;
    class STRAW_FRAMEWORK_API Menu;


    class STRAW_FRAMEWORK_API BaseMenuItem
        : public ElementInherit<BaseMenuItem, Element> {

    public:
        BaseMenuItem(const Context& context);
        ~BaseMenuItem();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void createMenuItem();
        virtual bool isDividerLine() const;

        void setId(const std::string& id);
        void setDividerLineHeight(int height);
        void setDividerLineMargin(int margin);
        void setDividerLineColor(SkColor color);

        void showSubMenu() const;
        void hideSubMenu() const;
        bool isSubMenuWindow(NativeWindow wnd) const;
        bool isSubMenuShown() const;
        bool isCursorInMenuItemWnd() const;
        Point getMenuItemEndPosition() const;

    protected:
        virtual View* createItemLayout();

    public:
        std::string mId;
        Menu* mSubMenu;
        View* mItemLayout;
    };
}
