/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <Windows.h>

#include "graphics/window/impl/WindowImpl.h"


namespace sf {


    class WindowsWndImpl : public WindowImpl {

    public:
        WindowsWndImpl(const Context& context, WindowHost& listener);
        ~WindowsWndImpl();

        virtual bool create(
            BaseWindow* parent,
            bool modal,
            int xPosition, int yPosition,
            int width, int height) override;
        virtual void invalidate(bool immediately = false) override;

        virtual void close() override;
        virtual void show() override;
        virtual void hide() override;

        virtual void setFocus() override;
        virtual void setPosition(int x, int y) override;
        virtual void setSize(int width, int height) override;
        virtual void setAlpha(float alpha) override;
        virtual void setCenterInScreen(bool center) override;

        virtual Point getPosition() const override;
        virtual Size getSize() const override;
        virtual float getAlpha() const override;

        virtual void setTitle(const std::string& title) override;
        virtual std::string getTitle() const override;
        virtual void setIcon(Buffer* content, bool large) override;
        virtual void setCursor(Buffer* content) override;
        virtual void setSystemCursor(SystemCursorType type) override;

        virtual void bringToTopMost() const override;
        virtual void cancelTopMost() const override;

        virtual void setMinimizable(bool enable) override;
        virtual void setMaximizable(bool enable) override;

        virtual void minWindow() const override;
        virtual void maxOrResotreWindow() const override;
        virtual bool isMinimized() const override;
        virtual bool isMaximized() const override;

        virtual bool isShown() const override;
        virtual bool isActived() const override;
        virtual void requestFocus() const override;

        virtual Point getCursorPosition() const override;
        virtual Point getCursorPositionInScreen() const override;
        virtual NativeWindow getNativeWindowInfo() const override;

        static void onAppTerminate();

    private:
        void onPaint(HDC hdc);
        void trackMouseEvent();

        LRESULT handleMessage(
            HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK handleMessageProc(
            HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK handleHookMessageProc(
            int code, WPARAM wParam, LPARAM lParam);

    private:
        HWND mHwnd;
        float mAlpha;
        bool mWaitNCMouseMessage;
        bool mMouseHovered;
        bool mModalWnd;
        HCURSOR mCursorHandle;
        static HHOOK mMessageHook;
    };

}
