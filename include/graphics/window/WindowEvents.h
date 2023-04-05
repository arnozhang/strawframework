/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/29
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/listener/ListenerBinder.h"
#include "graphics/window/BaseWindow.h"


namespace sf {

    class STRAW_FRAMEWORK_API WindowEvents {

    public:
        virtual ~WindowEvents() {
        }

        virtual void onShown() = 0;
        virtual void onHidden() = 0;
        virtual void onSetFocus(NativeWindow lostFocusWnd) = 0;
        virtual void onKillFocus(NativeWindow gotFocusWnd) = 0;

        virtual void onActived() = 0;
        virtual void onDeactived() = 0;

        virtual void onEnabled() = 0;
        virtual void onDisabled() = 0;

        virtual void onMoving() = 0;
        virtual void onSizing() = 0;
        virtual void onClosing(bool* cancel) = 0;
        virtual void onClosed() = 0;

        virtual void onMoved(int x, int y) = 0;
        virtual void onSized(int width, int height) = 0;

        virtual void onMaximized() = 0;
        virtual void onMinimized() = 0;
        virtual void onRestored() = 0;

        virtual void onMouseHovered() = 0;
        virtual void onMouseLeaved() = 0;

        virtual void onSystemPrepareTerminate() = 0;
    };
}
