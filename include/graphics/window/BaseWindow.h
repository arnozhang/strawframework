/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/ptr/RefPtrObject.h"
#include "graphics/view/Element.h"

#ifdef STRAW_BUILD_FOR_WIN32
#include <Windows.h>
#endif // STRAW_BUILD_FOR_WIN32


namespace sf {


    enum SizeableType {
        Disabled = 0x00,
        VerticalSizeable = 0x02,
        HorizontalSizeable = 0x01,
        BothSizeable = HorizontalSizeable | VerticalSizeable
    };

    struct NativeWindow {
#ifdef STRAW_BUILD_FOR_WIN32
        HWND hwnd;

        bool operator== (const NativeWindow& rhs) const {
            return hwnd == rhs.hwnd;
        }
#else
#endif // STRAW_BUILD_FOR_WIN32
    };


    class STRAW_FRAMEWORK_API BaseWindow
        : public ElementInherit<BaseWindow, Element, true> {

    public:
        BaseWindow(const Context& context) : ElementInherit(context) {}

        virtual ElementType getElementType() const {
            return sf::ElementType::Element_Window;
        }

        virtual void close() = 0;
        virtual void show() = 0;
        virtual void hide() = 0;

        virtual void setFocus() = 0;
        virtual void setPosition(int x, int y) = 0;
        virtual void setSize(int width, int height) = 0;
        virtual void setAlpha(float alpha) = 0;
        virtual void setSizeable(SizeableType type) = 0;
        virtual void setCenterInScreen(bool center) = 0;

        virtual void setMinimizable(bool enable) = 0;
        virtual void setMaximizable(bool enable) = 0;

        virtual Point getPosition() const = 0;
        virtual Size getSize() const = 0;
        virtual float getAlpha() const = 0;

        virtual void setTitle(const std::string& title) = 0;
        virtual std::string getTitle() const = 0;

        virtual void bringToTopMost() const = 0;
        virtual void cancelTopMost() const = 0;

        virtual void minWindow() const = 0;
        virtual void maxOrResotreWindow() const = 0;
        virtual bool isMinimized() const = 0;
        virtual bool isMaximized() const = 0;

        virtual bool isShown() const = 0;
        virtual bool isActived() const = 0;
        virtual void requestFocus() const = 0;

        virtual Point getCursorPosition() const = 0;
        virtual Point getCursorPositionInScreen() const = 0;
        virtual NativeWindow getNativeWindowInfo() const = 0;
    };
}
