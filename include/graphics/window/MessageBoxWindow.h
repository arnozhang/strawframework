/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/21
 */

#pragma once

#include "graphics/window/MessageBox.h"
#include "graphics/window/CaptionWindow.h"


namespace sf {

    class STRAW_FRAMEWORK_API MessageBoxWindow
        : public ElementInherit<MessageBoxWindow, CaptionWindow> {

        IMPL_ELEMENT(MessageBoxWindow);

    public:
        MessageBoxWindow(const Context& context);
        virtual ~MessageBoxWindow();

        virtual void createInternal(Attributes* attrs);
        virtual void dismiss();

        void setLeftBtnText(const std::string& text);
        void setRightBtnText(const std::string& text);

        void setLeftBtnClickListener(msgbox::MsgBoxBtnListener listener);
        void setRightBtnClickListener(msgbox::MsgBoxBtnListener listener);

        void setSingleBtn(bool single);
        void setIconType(msgbox::IconType iconType);
        void setContentText(const std::string& content);
    };

}
