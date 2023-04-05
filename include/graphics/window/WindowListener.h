/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/28
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/listener/ListenerBinder.h"
#include "graphics/window/WindowEvents.h"


namespace sf {


    struct STRAW_FRAMEWORK_API NativeWindowMessage {
#ifdef STRAW_BUILD_FOR_WIN32
        NativeWindowMessage(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) {
            hwnd = _hwnd;
            message = _msg;
            wParam = _wParam;
            lParam = _lParam;
            lResult = 0;
            handled = false;
        }

        HWND hwnd;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
        LRESULT lResult;
#else
#endif // STRAW_BUILD_FOR_WIN32

        void setHandled() {
            handled = true;
        }

        bool handled;
    };


    class STRAW_FRAMEWORK_API NativeWindowMessageMonitor
        : public ListenerBinder<NativeWindowMessageMonitor> {

    public:
        virtual ~NativeWindowMessageMonitor() {
        }

        virtual void onHandleNativeMessage(NativeWindowMessage* msg) {
            INVOKE(onHandleNativeMessage, msg);
        }
    };


    class STRAW_FRAMEWORK_API WindowListener
        : public ListenerBinder<WindowListener>
          , public WindowEvents {

    public:
        virtual void onShown() override {
            INVOKE(onShown);
        }

        virtual void onHidden() override {
            INVOKE(onHidden);
        }

        virtual void onSetFocus(NativeWindow lostFocusWnd) override {
            INVOKE(onSetFocus, lostFocusWnd);
        }

        virtual void onKillFocus(NativeWindow gotFocusWnd) override {
            INVOKE(onKillFocus, gotFocusWnd);
        }

        virtual void onActived() override {
            INVOKE(onActived);
        }

        virtual void onDeactived() override {
            INVOKE(onDeactived);
        }

        virtual void onEnabled() override {
            INVOKE(onEnabled);
        }

        virtual void onDisabled() override {
            INVOKE(onDisabled);
        }

        virtual void onMoving() override {
            INVOKE(onMoving);
        }

        virtual void onSizing() override {
            INVOKE(onSizing);
        }

        virtual void onClosing(bool* cancel) override {
            INVOKE(onClosing, cancel);
        }

        virtual void onClosed() override {
            INVOKE(onClosed);
        }

        virtual void onMoved(int x, int y) override {
            INVOKE(onMoved, x, y);
        }

        virtual void onSized(int width, int height) override {
            INVOKE(onSized, width, height);
        }

        virtual void onMaximized() override {
            INVOKE(onMaximized);
        }

        virtual void onMinimized() override {
            INVOKE(onMinimized);
        }

        virtual void onRestored() override {
            INVOKE(onRestored);
        }

        virtual void onMouseHovered() override {
            INVOKE(onMouseHovered);
        }

        virtual void onMouseLeaved() override {
            INVOKE(onMouseLeaved);
        }

        virtual void onSystemPrepareTerminate() override {
            INVOKE(onSystemPrepareTerminate);
        }
    };
}

