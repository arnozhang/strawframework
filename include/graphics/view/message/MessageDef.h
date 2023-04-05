/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/ptr/RefPtrObject.h"
#include "graphics/GraphicsDeclare.h"


namespace sf {

    enum MessageType {
        SM_None,

        SM_MouseBegin,

        SM_LButtonDown,
        SM_LButtonUp,
        SM_RButtonDown,
        SM_RButtonUp,
        SM_MButtonDown,
        SM_MButtonUp,

        SM_LButtonClick,
        SM_RButtonClick,
        SM_MButtonClick,

        SM_LButtonDblClick,
        SM_RButtonDblClick,
        SM_MButtonDblClick,

        SM_MouseHover,
        SM_MouseLeave,
        SM_MouseMove,
        SM_MouseWheel,
       
        SM_MouseEnd,

        SM_KeyboardBegin,
        SM_KeyDown,
        SM_KeyUp,
        SM_Char,
        
        SM_SetFocus,
        SM_KillFocus,
        SM_HitTest,

        SM_Size,

        SM_Close,
        SM_Destroy,

        // for User Customize Message.
        SM_UserMessage,
    };


    struct STRAW_FRAMEWORK_API BaseMessage {
        BaseMessage() {
            type = SM_None;
        }

        BaseMessage(MessageType _type) {
            type = _type;
        }

        virtual ~BaseMessage() {}

        MessageType type;
    };


    struct STRAW_FRAMEWORK_API SizeChangeMessage : public BaseMessage {
        SizeChangeMessage(int width, int height) : BaseMessage(SM_Size){
            size.set(width, height);
        }

        Size size;
    };

    struct STRAW_FRAMEWORK_API BaseMessageResult : public RefCountObject {
        BaseMessageResult() {
            handled = false;
        }

        virtual ~BaseMessageResult() {}

        bool handled;
    };


    struct STRAW_FRAMEWORK_API MouseMessage : public BaseMessage {

        MouseMessage(MessageType _type, int _x = 0, int _y = 0, int _flag = 0) {
            type = _type;
            x = _x;
            y = _y;
            keyDownFlag = _flag;
        }

        int x;
        int y;
        int keyDownFlag;
    };


    class STRAW_FRAMEWORK_API MessageDispatcher;

    struct STRAW_FRAMEWORK_API MouseMessageResult : public BaseMessageResult {

        MouseMessageResult(MessageDispatcher* _dispatcher) {
            dispatcher = _dispatcher;
        }

        MessageDispatcher* dispatcher;
    };

    struct STRAW_FRAMEWORK_API HitTestMessage : public BaseMessage {
        HitTestMessage(int _x, int _y) {
            type = SM_HitTest;
            x = _x;
            y = _y;
        }

        int x;
        int y;
    };

    struct STRAW_FRAMEWORK_API HitTestMessageResult : BaseMessageResult {
        HitTestMessageResult(HitTestZone _zone) {
            zone = _zone;
        }

        HitTestZone zone;
    };
}
