/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/def/FrameworkDef.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/window/BaseWindow.h"
#include "graphics/window/WindowEvents.h"
#include "graphics/window/WindowListener.h"

#include "thirdparty/skia/core/SkBitmap.h"


namespace sf {

    class STRAW_FRAMEWORK_API WindowHost
        : public WindowEvents
        , public NativeWindowMessageMonitor {

    public:
        virtual ~WindowHost() {}

        virtual void onLButtonDown(int keyDownFlag, int x, int y) = 0;
        virtual void onLButtonUp(int keyDownFlag, int x, int y) = 0;

        virtual void onRButtonDown(int keyDownFlag, int x, int y) = 0;
        virtual void onRButtonUp(int keyDownFlag, int x, int y) = 0;

        virtual void onMButtonDown(int keyDownFlag, int x, int y) = 0;
        virtual void onMButtonUp(int keyDownFlag, int x, int y) = 0;

        virtual void onLButtonDblClick(int keyDownFlag, int x, int y) = 0;
        virtual void onRButtonDblClick(int keyDownFlag, int x, int y) = 0;
        virtual void onMButtonDblClick(int keyDownFlag, int x, int y) = 0;

        virtual void onKeyDown(int vkCode) = 0;
        virtual void onKeyUp(int vkCode) = 0;
        virtual void onChar(int uniChar) = 0;

        virtual void onMouseMove(int keyDownFlag, int x, int y) = 0;
        virtual void onMouseWheel() = 0;
        virtual void onMouseHorizonWheel() = 0;

        virtual HitTestZone onHitTest(int x, int y) = 0;

        virtual void onPaint() = 0;
        virtual SkBitmap* getPaintBitmap() = 0;

        virtual Size getMinSize() = 0;
        virtual Size getMaxSize() = 0;

        virtual bool getMinimizable() const = 0;
        virtual bool getMaximizable() const = 0;
        virtual Rect getWindowShadow() const = 0;
    };


    class STRAW_FRAMEWORK_API WindowImpl : public BaseWindow {

    public:
        WindowImpl(const Context& context, WindowHost& listener);
        virtual ~WindowImpl();

        virtual bool create(
            BaseWindow* parent,
            bool modal,
            int xPosition, int yPosition,
            int width, int height) = 0;
        virtual void invalidate(bool immediately = false) = 0;
        virtual void setIcon(Buffer* content, bool large);
        virtual void setCursor(Buffer* content);
        virtual void setSizeable(SizeableType type) override;
        virtual void setSystemCursor(SystemCursorType type);
        virtual void setCenterInScreen(bool center) override;

    protected:
        WindowHost& mHost;
    };

}
