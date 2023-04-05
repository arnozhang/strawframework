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
#include "graphics/menu/BaseMenuItem.h"


namespace sf {

    class STRAW_FRAMEWORK_API StateListColor;


    class STRAW_FRAMEWORK_API MenuItem
        : public ElementInherit<MenuItem, BaseMenuItem> {

    public:
        MenuItem(const Context& context);
        ~MenuItem();

        virtual void setAttributes(Attributes* attrs) override;
        virtual bool isDividerLine() const override;
        virtual View* createItemLayout() override;

        void setTitle(const std::string& title);
        void setGrayTips(const std::string& grayTips);
        void setIcon(Drawable* drawable);
        void setAccelKey(const std::string& key);
        void setMenuTextSize(int textSize);
        void setMenuItemBackground(Drawable* drawable);
        void setMenuTitleColor(StateListColor* color);
        void setGrayTipsColor(StateListColor* color);
        void setEnable(bool enabled);
        void setItemHeight(int height);
        void setIsChecked(bool checked);

    public:
        std::string mTitle;
        std::string mGrayTips;
        int mAccelKeyCode;
        bool mIsChecked;
    };
}
