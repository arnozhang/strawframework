/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/listener/ListenerList.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/view/RootView.h"
#include "graphics/window/WindowListener.h"

#include "impl/WindowImpl.h"



namespace sf {

    class STRAW_FRAMEWORK_API Menu;


    class STRAW_FRAMEWORK_API Window
        : public ElementInherit<Window, BaseWindow>
        , public ListenerList<WindowListener>
        , private WindowHost
        , private RootViewListener {

        IMPL_ELEMENT(Window);

    public:
        Window(const Context& context);
        virtual ~Window();

    public:
        // Operation.
        //
        static Window* createByFile(
            const Context& context,
            const std::string& file,
            Window* parent = nullptr);
        static void onAppTerminate();

        virtual void create(
            Window* parent = nullptr,
            Attributes* attrs = nullptr,
            bool modal = false);

        virtual void setContentView(View* contentView);
        virtual void setAttributes(Attributes* attrs) override;

        virtual ViewGroup* getContentParent() const;
        virtual View* findViewById(const std::string& id) const;
        virtual RootView* getRootView() const;

        virtual void invalidate();
        virtual void scheduleLayout() override;
        virtual void scheduleInvalidate(
            Rect* dirty = nullptr, bool immediately = false) override;

        virtual void close() override;
        virtual void show() override;
        virtual void hide() override;

        virtual Menu* trackPopupMenu(
            const std::string& menuRes, bool showImmediately = true);
        virtual bool isSameWindow(NativeWindow wnd) const;

    public:
        // Attrs.
        //
        virtual void setFocus() override;
        virtual void setAlpha(float alpha) override;
        virtual void setPosition(Point position);
        virtual void setPosition(int x, int y) override;
        virtual void setCenterInScreen(bool center) override;
        virtual void setSizeable(SizeableType type) override;

        virtual void setSize(Size size);
        virtual void setSize(int width, int height) override;
        virtual void setWidth(int width);
        virtual void setHeight(int height);

        virtual void setMinWidth(int width);
        virtual void setMinHeight(int height);
        virtual void setMinSize(Size size);

        virtual void setMaxWidth(int width);
        virtual void setMaxHeight(int height);
        virtual void setMaxSize(Size size);

        virtual void setWidthWrapContent(bool wrapContent);
        virtual void setHeightWrapContent(bool wrapContent);

        virtual void setMinimizable(bool enable) override;
        virtual void setMaximizable(bool enable) override;
        virtual bool getMinimizable() const override;
        virtual bool getMaximizable() const override;
        virtual Rect getWindowShadow() const override;

        virtual Point getPosition() const override;
        virtual Size getSize() const override;
        virtual float getAlpha() const override;
        virtual Rect getGlobalRect() const;

        virtual void setSmallIcon(const std::string& resPath);
        virtual void setLargeIcon(const std::string& resPath);
        virtual void setAllIcon(const std::string& resPath);
        virtual void setIcon(const std::string& resPath, bool large);
        virtual void setCursor(const std::string& resPath);
        virtual void setSystemCursor(SystemCursorType type);
        virtual void setTitle(const std::string& title) override;
        virtual std::string getTitle() const override;

        virtual void setTopMost(bool topMost) const;
        virtual void bringToTopMost() const override;
        virtual void cancelTopMost() const override;

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

    public:
        // Events & Messages.
        //
        void addNativeMsgMonitor(NativeWindowMessageMonitor& monitor);


    private:
        void createWindowImpl();
        void draw(SkCanvas* canvas);
        void resizeInternal(int width, int height);

        void handleMouseButtonDown(
            const MouseMessage& msg, MessageDispatcher*& savedDispatcher);
        void handleMouseButtonUp(
            const MouseMessage& msg, MessageDispatcher*& savedDispatcher);
        void handleMouseButtonDblClick(
            MessageType type, int keyDownFlag, int x, int y);


    protected:
        virtual void onLButtonDown(int keyDownFlag, int x, int y) override;
        virtual void onLButtonUp(int keyDownFlag, int x, int y) override;

        virtual void onRButtonDown(int keyDownFlag, int x, int y) override;
        virtual void onRButtonUp(int keyDownFlag, int x, int y) override;

        virtual void onMButtonDown(int keyDownFlag, int x, int y) override;
        virtual void onMButtonUp(int keyDownFlag, int x, int y) override;

        virtual void onLButtonDblClick(int keyDownFlag, int x, int y) override;
        virtual void onRButtonDblClick(int keyDownFlag, int x, int y) override;
        virtual void onMButtonDblClick(int keyDownFlag, int x, int y) override;

        virtual void onKeyDown(int vkCode) override;
        virtual void onKeyUp(int vkCode) override;
        virtual void onChar(int uniChar) override;

        virtual void onMouseMove(int keyDownFlag, int x, int y) override;
        virtual void onMouseWheel() override;
        virtual void onMouseHorizonWheel() override;

        virtual void onShown() override;
        virtual void onHidden() override;
        virtual void onSetFocus(NativeWindow lostFocusWnd) override;
        virtual void onKillFocus(NativeWindow gotFocusWnd) override;

        virtual void onActived() override;
        virtual void onDeactived() override;

        virtual void onEnabled() override;
        virtual void onDisabled() override;

        virtual void onMoving() override;
        virtual void onSizing() override;
        virtual void onClosing(bool* cancel) override;
        virtual void onClosed() override;

        virtual void onMoved(int x, int y) override;
        virtual void onSized(int width, int height) override;

        virtual void onMaximized() override;
        virtual void onMinimized() override;
        virtual void onRestored() override;

        virtual void onMouseHovered() override;
        virtual void onMouseLeaved() override;

        virtual void onSystemPrepareTerminate() override;

        virtual void onPaint() override;
        virtual HitTestZone onHitTest(int x, int y) override;
        virtual void onHandleNativeMessage(NativeWindowMessage* msg) override;

        virtual SkBitmap* getPaintBitmap() override;
        virtual Size getMinSize() override;
        virtual Size getMaxSize() override;

        virtual void createInternal(Attributes* attrs);
        virtual void setAttributesInternal(Attributes* attrs);

    protected:
        ref_ptr<WindowImpl> mWindowImpl;
        ref_ptr<RootView> mRootView;

        SkBitmap mBitmap;
        SkRegion mDirtyRgn;
        Size mMinSize;
        Size mMaxSize;
        bool mMinimizable;
        bool mMaximizable;
        bool mWidthWrapContent;
        bool mHeightWrapContent;
        SizeableType mSizeableType;
        ListenerList<NativeWindowMessageMonitor> mNativeMsgMonitorList;

        MessageDispatcher* mLButtonDispatcher;
        MessageDispatcher* mRButtonDispatcher;
        MessageDispatcher* mMButtonDispatcher;
        MessageDispatcher* mMouseMoveDispatcher;
    };

}
