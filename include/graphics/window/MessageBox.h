/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/21
 */

#pragma once

#include <functional>

#include "base/def/PlatformDefine.h"


namespace sf {

    class STRAW_FRAMEWORK_API Context;
    class STRAW_FRAMEWORK_API Window;
    class STRAW_FRAMEWORK_API MessageBoxWindow;


    namespace msgbox {

        namespace layout {
            const char* const id_msgIcon = "msgIcon";
            const char* const id_msgContent = "msgContent";
            const char* const id_leftBtn = "leftBtn";
            const char* const id_rightBtn = "rightBtn";

            const char* const attr_msgBoxLayout = "msgBoxLayout";
        }


        enum IconType {
            None,
            Information,
            Question,
            Warning,
            Error,
            OK,
        };


        using MsgBoxBtnListener = std::function<void(MessageBoxWindow*)>;
        using MsgBoxDismissListener = std::function<void(MessageBoxWindow*)>;


        STRAW_FRAMEWORK_API MessageBoxWindow* showMsgBox(
            const Context& context,
            IconType iconType,
            const std::string& title,
            const std::string& content,
            const std::string& leftBtnText,
            const std::string& rightBtnText,
            MsgBoxBtnListener leftListener,
            MsgBoxBtnListener rightListener,
            Window* parent = nullptr,
            MsgBoxDismissListener* dismissListener = nullptr,
            const char* msgBoxLayoutRes = nullptr);

        STRAW_FRAMEWORK_API MessageBoxWindow* showSingleBtnMsgBox(
            const Context& context,
            IconType iconType,
            const std::string& content,
            const std::string& btnText,
            MsgBoxBtnListener btnListener,
            Window* parent = nullptr,
            MsgBoxDismissListener* dismissListener = nullptr);

        STRAW_FRAMEWORK_API MsgBoxBtnListener dismissListener();
    }
}
