#include "graphics/window/Window.h"

#include "base/StrawUtils.h"
#include "graphics/view/TextView.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/layout/LinearLayout.h"
#include "graphics/layout/FrameLayout.h"
#include "graphics/layout/FrameLayoutSupporter.h"
#include "graphics/layout/AbsoluteLayout.h"
#include "graphics/context/Context.h"
#include "graphics/menu/Menu.h"

#include "thirdparty/skia/core/SkSurface.h"
#include "thirdparty/skia/core/SkImageDecoder.h"

#include "impl/WindowsWndImpl.h"


namespace {

    const int DEFAULT_WND_WIDTH = 600;
    const int DEFAULT_WND_HEIGHT = 400;


} // anonymous namespace ends here.


sf::Window::Window(const Context& context)
    : ElementInherit(context) {

    mMinimizable = true;
    mMaximizable = true;
    mWidthWrapContent = false;
    mHeightWrapContent = false;

    mLButtonDispatcher = nullptr;
    mRButtonDispatcher = nullptr;
    mMButtonDispatcher = nullptr;
    mMouseMoveDispatcher = nullptr;
    mSizeableType = SizeableType::Disabled;

    SkImageInfo info = SkImageInfo::Make(
        0, 0, kN32_SkColorType, kPremul_SkAlphaType);
    mBitmap.setInfo(info);
    mBitmap.allocN32Pixels(0, 0);

    mRootView = new RootView(this);
    mRootView->setId(sf::window::id_wndRootView);
}

sf::Window::~Window() {
}

sf::Window* sf::Window::createByFile(
    const Context& context,
    const std::string& file,
    Window* parent /*= nullptr*/) {

    return context.getLayoutInflater().inflateWindow(file, parent);
}

void sf::Window::onAppTerminate() {
#ifdef STRAW_BUILD_FOR_WIN32
    WindowsWndImpl::onAppTerminate();
#endif // STRAW_BUILD_FOR_WIN32
}

void sf::Window::create(
    Window* parent /*= nullptr*/,
    Attributes* attrs /*= nullptr*/,
    bool modal /*= false*/) {

    Point position(-1, -10);
    int width = DEFAULT_WND_WIDTH;
    int height = DEFAULT_WND_HEIGHT;
    if (attrs) {
        width = attrs->getDimensionAttr(sf::layout::attr_width);
        height = attrs->getDimensionAttr(sf::layout::attr_height);
        if (attrs->hasAttribute(sf::layout::attr_position)) {
            position = attrs->getPointAttr(sf::layout::attr_position);
        }

        if (width < 0) {
            width = DEFAULT_WND_WIDTH;
        }

        if (height < 0) {
            height = DEFAULT_WND_HEIGHT;
        }
    }

    createWindowImpl();
    mWindowImpl->create(parent, modal, position.x, position.y, width, height);

    ViewUpdateLock lock(mRootView);
    createInternal(attrs);

    if (attrs && attrs->hasAttribute(sf::window::attr_wndClientBackground)) {
        getContentParent()->setBackground(
            attrs->getDrawableAttr(sf::window::attr_wndClientBackground));
    }
}

void sf::Window::createInternal(Attributes* attrs) {
    LayoutInflater& inflater = mContext.getLayoutInflater();
    inflater.inflate("@platform:layout/default_window", mRootView, true);
}

void sf::Window::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    ViewUpdateLock lock(mRootView);
    setAttributesInternal(attrs);
}

void sf::Window::setAttributesInternal(Attributes* attrs) {
    BEGIN_ATTRS(attrs)
        ATTR_STRING(title, setTitle)
        ATTR_RAW_STRING(cursor, setCursor)
        ATTR_RAW_STRING(icon, setAllIcon)
        ATTR_RAW_STRING(smallIcon, setSmallIcon)
        ATTR_RAW_STRING(largeIcon, setLargeIcon)
        ATTR_ENUM(systemCursor, setSystemCursor, SystemCursorType)

        ATTR_SIZE(minSize, setMinSize)
        ATTR_DIMENSION(minWidth, setMinWidth)
        ATTR_DIMENSION(minHeight, setMinHeight)

        ATTR_SIZE(maxSize, setMaxSize)
        ATTR_DIMENSION(maxWidth, setMaxWidth)
        ATTR_DIMENSION(maxHeight, setMaxHeight)

        ATTR_SIZE(size, setSize)
        ATTR_DIMENSION(width, setWidth)
        ATTR_DIMENSION(height, setHeight)

        ATTR_FLOAT(alpha, setAlpha)
        ATTR_BOOL(topMost, setTopMost)
        ATTR_POINT(position, setPosition)
        ATTR_BOOL(minimizable, setMinimizable)
        ATTR_BOOL(maximizable, setMaximizable)
        ATTR_BOOL(centerInScreen, setCenterInScreen)
        ATTR_ENUM(sizeable, setSizeable, SizeableType)
    END_ATTRS()
}

void sf::Window::setContentView(View* contentView) {
    ref_ptr<ViewGroup> contentRoot = getContentParent();
    if (contentRoot) {
        contentRoot->removeAllViews();
    }

    ref_ptr<LayoutParams> params = contentRoot->createDefaultLayoutParams();
    contentRoot->addView(contentView, params);
}

void sf::Window::setSizeable(SizeableType type) {
    mSizeableType = type;
    mWindowImpl->setSizeable(type);
}

sf::ViewGroup* sf::Window::getContentParent() const {
    View* view = mRootView->findViewById(sf::window::id_wndContentRoot);
    if (view && view->canAddChild()) {
        return reinterpret_cast<ViewGroup*>(view);
    }

    return mRootView;
}

sf::View* sf::Window::findViewById(const std::string& id) const {
    return mRootView->findViewById(id);
}

sf::RootView* sf::Window::getRootView() const {
    return mRootView;
}

void sf::Window::close() {
    mWindowImpl->close();
}

void sf::Window::show() {
    mWindowImpl->show();
}

void sf::Window::hide() {
    mWindowImpl->hide();
}

sf::Menu* sf::Window::trackPopupMenu(
    const std::string& menuRes, bool showImmediately /*= true*/) {
    Menu* menu = mContext.getLayoutInflater().inflateMenu(menuRes, this);
    if (menu && showImmediately) {
        menu->showMenu(getCursorPositionInScreen());
    }

    return menu;
}

bool sf::Window::isSameWindow(NativeWindow wnd) const {
    return wnd == getNativeWindowInfo();
}

void sf::Window::setFocus() {
    mWindowImpl->setFocus();
}

void sf::Window::setPosition(int x, int y) {
    mWindowImpl->setPosition(x, y);
}

void sf::Window::setCenterInScreen(bool center) {
    mWindowImpl->setCenterInScreen(center);
}

void sf::Window::setPosition(Point position) {
    setPosition(position.x, position.y);
}

void sf::Window::setSize(int width, int height) {
    setSize(Size(width, height));
}

void sf::Window::setSize(Size size) {
    if (mMinSize.width > 0 && size.width < mMinSize.width) {
        size.width = mMinSize.width;
    }

    if (mMinSize.height > 0 && size.height < mMinSize.height) {
        size.height = mMinSize.height;
    }

    if (mMaxSize.width > 0 && size.width > mMaxSize.width) {
        size.width = mMaxSize.width;
    }

    if (mMaxSize.width > 0 && size.width > mMaxSize.width) {
        size.width = mMaxSize.width;
    }

    mWindowImpl->setSize(size.width, size.height);
}

void sf::Window::setAlpha(float alpha) {
    mWindowImpl->setAlpha(alpha);
}

void sf::Window::setWidth(int width) {
    Size size = getSize();
    size.width = width;
    setSize(size);
}

void sf::Window::setHeight(int height) {
    Size size = getSize();
    size.height = height;
    setSize(size);
}

void sf::Window::setMinWidth(int width) {
    mMinSize.width = width;

    Size size = getSize();
    if (mMinSize.width > size.width) {
        setSize(mMinSize.width, size.height);
    }
}

void sf::Window::setMinHeight(int height) {
    mMinSize.height = height;

    Size size = getSize();
    if (mMinSize.height > size.height) {
        setSize(size.width, mMinSize.height);
    }
}

void sf::Window::setMinSize(Size size) {
    mMinSize = size;

    size = getSize();
    if (mMinSize.height > size.height) {
        size.height = mMinSize.height;
    }

    if (mMinSize.width > size.width) {
        size.width = mMinSize.width;
    }

    setSize(size);
}

void sf::Window::setMaxWidth(int width) {
    mMaxSize.width = width;

    Size size = getSize();
    if (mMaxSize.width < size.width) {
        setSize(mMaxSize.width, size.height);
    }
}

void sf::Window::setMaxHeight(int height) {
    mMaxSize.height = height;

    Size size = getSize();
    if (mMaxSize.height < size.height) {
        setSize(size.width, mMaxSize.height);
    }
}

void sf::Window::setMaxSize(Size size) {
    mMaxSize = size;

    size = getSize();
    if (mMaxSize.height < size.height) {
        size.height = mMaxSize.height;
    }

    if (mMaxSize.width < size.width) {
        size.width = mMaxSize.width;
    }

    setSize(size);
}

void sf::Window::setWidthWrapContent(bool wrapContent) {
    mWidthWrapContent = wrapContent;
    scheduleLayout();
}

void sf::Window::setHeightWrapContent(bool wrapContent) {
    mHeightWrapContent = wrapContent;
    scheduleLayout();
}

void sf::Window::setMinimizable(bool enable) {
    mMinimizable = enable;
    mWindowImpl->setMinimizable(enable);

    ref_ptr<View> minBtn = findViewById(sf::window::id_wndMinBtn);
    if (minBtn) {
        minBtn->setVisibility(enable ? Visibility::Visible : Visibility::Gone);
    }
}

void sf::Window::setMaximizable(bool enable) {
    mMinimizable = enable;
    mWindowImpl->setMaximizable(enable);

    ref_ptr<View> maxBtn = findViewById(sf::window::id_wndMaxBtn);
    if (maxBtn) {
        maxBtn->setVisibility(enable ? Visibility::Visible : Visibility::Gone);
    }
}

bool sf::Window::getMinimizable() const {
    return mMinimizable;
}

bool sf::Window::getMaximizable() const {
    return mMaximizable;
}

sf::Rect sf::Window::getWindowShadow() const {
    return getContentParent()->getPadding();
}

sf::Point sf::Window::getPosition() const {
    return mWindowImpl->getPosition();
}

sf::Size sf::Window::getSize() const {
    return mWindowImpl->getSize();
}

float sf::Window::getAlpha() const {
    return mWindowImpl->getAlpha();
}

sf::Rect sf::Window::getGlobalRect() const {
    Point pos = getPosition();
    Size size = getSize();
    Rect rc(0, 0, size.width, size.height);
    return rc.move(pos.x, pos.y);
}

void sf::Window::setSmallIcon(const std::string& resPath) {
    setIcon(resPath, false);
}

void sf::Window::setLargeIcon(const std::string& resPath) {
    setIcon(resPath, true);
}

void sf::Window::setAllIcon(const std::string& resPath) {
    setSmallIcon(resPath);
    setLargeIcon(resPath);
}

void sf::Window::setIcon(const std::string& resPath, bool large) {
    ref_ptr<Buffer> content = mContext.getResourceContent(resPath);
    if (content && !content->empty()) {
        mWindowImpl->setIcon(content, large);
    }
}

void sf::Window::setCursor(const std::string& resPath) {
    ResourceType resType = ResourceType::Binary;
    ref_ptr<Buffer> content = mContext.getResourceContent(resPath, &resType);
    if (content && !content->empty() && resType == ResourceType::Cursor) {
        mWindowImpl->setCursor(content);
    }
}

void sf::Window::setSystemCursor(SystemCursorType type) {
    mWindowImpl->setSystemCursor(type);
}

void sf::Window::setTitle(const std::string& title) {
    mWindowImpl->setTitle(title);

    ref_ptr<View> view = findViewById(sf::window::id_wndTitle);
    if (view) {
        TextView* titleView = reinterpret_cast<TextView*>(view.get());
        titleView->setText(title);
    }
}

std::string sf::Window::getTitle() const {
    return mWindowImpl->getTitle();
}

void sf::Window::setTopMost(bool topMost) const {
    if (topMost) {
        bringToTopMost();
    } else {
        cancelTopMost();
    }
}

void sf::Window::bringToTopMost() const {
    mWindowImpl->bringToTopMost();
}

void sf::Window::cancelTopMost() const {
    mWindowImpl->cancelTopMost();
}

void sf::Window::minWindow() const {
    mWindowImpl->minWindow();
}

void sf::Window::maxOrResotreWindow() const {
    mWindowImpl->maxOrResotreWindow();
}

bool sf::Window::isMinimized() const {
    return mWindowImpl->isMinimized();
}

bool sf::Window::isMaximized() const {
    return mWindowImpl->isMaximized();
}

bool sf::Window::isShown() const {
    return mWindowImpl->isShown();
}

bool sf::Window::isActived() const {
    return mWindowImpl->isActived();
}

void sf::Window::requestFocus() const {
    mWindowImpl->requestFocus();
}

sf::Point sf::Window::getCursorPosition() const {
    return mWindowImpl->getCursorPosition();
}

sf::Point sf::Window::getCursorPositionInScreen() const {
    return mWindowImpl->getCursorPositionInScreen();
}

void sf::Window::invalidate() {
    scheduleInvalidate(nullptr, true);
}

void sf::Window::createWindowImpl() {
#ifdef STRAW_BUILD_FOR_WIN32
    mWindowImpl = new WindowsWndImpl(mContext, *this);
#else 
#endif // STRAW_BUILD_FOR_WIN32
}

void sf::Window::handleMouseButtonDown(
    const MouseMessage& msg, MessageDispatcher*& savedDispatcher) {

    ref_ptr<MouseMessageResult> result = reinterpret_cast<MouseMessageResult*>(
        mRootView->routeMessage(msg));
    if (result && result->dispatcher) {
        if (msg.type == SM_LButtonDown) {
            result->dispatcher->doMousePressed();
        }
    }

    savedDispatcher = result ? result->dispatcher : nullptr;
}

void sf::Window::handleMouseButtonUp(
    const MouseMessage& msg, MessageDispatcher*& savedDispatcher) {

    ref_ptr<MouseMessageResult> result = reinterpret_cast<MouseMessageResult*>(
        mRootView->routeMessage(msg));
    if (savedDispatcher) {
        if (msg.type == SM_LButtonUp) {
            savedDispatcher->doMouseUnPressed();
        }

        if (result && result->dispatcher == savedDispatcher) {
            MouseMessage click(SM_None, msg.x, msg.y, msg.keyDownFlag);
            if (msg.type == SM_LButtonUp) {
                click.type = SM_LButtonClick;
            } else if (msg.type == SM_RButtonUp) {
                click.type = SM_RButtonClick;
            } else if (msg.type == SM_MButtonUp) {
                click.type = SM_MButtonClick;
            }

            result->dispatcher->dispatchMessage(click);
        }
    }

    savedDispatcher = nullptr;
}

void sf::Window::onLButtonDown(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_LButtonDown, x, y, keyDownFlag);
    handleMouseButtonDown(msg, mLButtonDispatcher);
}

void sf::Window::onLButtonUp(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_LButtonUp, x, y, keyDownFlag);
    handleMouseButtonUp(msg, mLButtonDispatcher);
}

void sf::Window::onRButtonDown(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_RButtonDown, x, y, keyDownFlag);
    handleMouseButtonDown(msg, mLButtonDispatcher);
}

void sf::Window::onRButtonUp(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_RButtonUp, x, y, keyDownFlag);
    handleMouseButtonUp(msg, mLButtonDispatcher);
}

void sf::Window::onMButtonDown(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_MButtonDown, x, y, keyDownFlag);
    handleMouseButtonDown(msg, mLButtonDispatcher);
}

void sf::Window::onMButtonUp(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_MButtonUp, x, y, keyDownFlag);
    handleMouseButtonUp(msg, mLButtonDispatcher);
}

void sf::Window::handleMouseButtonDblClick(
    MessageType type, int keyDownFlag, int x, int y) {

    MouseMessage msg(type, x, y, keyDownFlag);
    ref_ptr<MouseMessageResult> result = reinterpret_cast<MouseMessageResult*>(
        mRootView->routeMessage(msg));
    if (result && result->dispatcher) {
        result->dispatcher->dispatchMessage(msg);
    }
}

void sf::Window::onLButtonDblClick(int keyDownFlag, int x, int y) {
    handleMouseButtonDblClick(MessageType::SM_LButtonDblClick, keyDownFlag, x, y);
}

void sf::Window::onRButtonDblClick(int keyDownFlag, int x, int y) {
    handleMouseButtonDblClick(MessageType::SM_RButtonDblClick, keyDownFlag, x, y);
}

void sf::Window::onMButtonDblClick(int keyDownFlag, int x, int y) {
    handleMouseButtonDblClick(MessageType::SM_MButtonDblClick, keyDownFlag, x, y);
}

void sf::Window::onMouseHovered() {
    for (auto listener : mListeners) {
        listener->onMouseHovered();
    }
}

void sf::Window::onMouseLeaved() {
    for (auto listener : mListeners) {
        listener->onMouseLeaved();
    }
}

void sf::Window::onSystemPrepareTerminate() {
    for (auto listener : mListeners) {
        listener->onSystemPrepareTerminate();
    }
}

void sf::Window::onMouseMove(int keyDownFlag, int x, int y) {
    MouseMessage msg(MessageType::SM_MouseMove, x, y, keyDownFlag);
    ref_ptr<MouseMessageResult> result = reinterpret_cast<MouseMessageResult*>(
        mRootView->routeMessage(msg));

    if (!result || mMouseMoveDispatcher != result->dispatcher) {
        if (mMouseMoveDispatcher) {
            mMouseMoveDispatcher->doMouseLeave();
            mMouseMoveDispatcher = nullptr;
        }
    }

    if (result) {
        mMouseMoveDispatcher = result->dispatcher;
        if (mMouseMoveDispatcher) {
            mMouseMoveDispatcher->doMouseHover();
        }
    }
}

void sf::Window::onKeyDown(int vkCode) {
}

void sf::Window::onKeyUp(int vkCode) {
}

void sf::Window::onChar(int uniChar) {
}

void sf::Window::onMouseWheel() {
}

void sf::Window::onMouseHorizonWheel() {
}

void sf::Window::onShown() {
    for (auto listener : mListeners) {
        listener->onShown();
    }
}

void sf::Window::onHidden() {
    for (auto listener : mListeners) {
        listener->onHidden();
    }
}

void sf::Window::onSetFocus(NativeWindow lostFocusWnd) {
    for (auto listener : mListeners) {
        listener->onSetFocus(lostFocusWnd);
    }
}

void sf::Window::onKillFocus(NativeWindow gotFocusWnd) {
    if (mMouseMoveDispatcher) {
        mMouseMoveDispatcher->doMouseLeave();
        mMouseMoveDispatcher = nullptr;
    }

    for (auto listener : mListeners) {
        listener->onKillFocus(gotFocusWnd);
    }
}

void sf::Window::onActived() {
    for (auto listener : mListeners) {
        listener->onActived();
    }
}

void sf::Window::onDeactived() {
    for (auto listener : mListeners) {
        listener->onDeactived();
    }
}

void sf::Window::onEnabled() {
    for (auto listener : mListeners) {
        listener->onEnabled();
    }
}

void sf::Window::onDisabled() {
    for (auto listener : mListeners) {
        listener->onDisabled();
    }
}

void sf::Window::onMoving() {
    for (auto listener : mListeners) {
        listener->onMoving();
    }
}

void sf::Window::onSizing() {
    for (auto listener : mListeners) {
        listener->onSizing();
    }
}

void sf::Window::onClosing(bool* cancel) {
    for (auto listener : mListeners) {
        listener->onClosing(cancel);
    }
}

void sf::Window::onMoved(int x, int y) {
    for (auto listener : mListeners) {
        listener->onMoved(x, y);
    }
}

void sf::Window::onSized(int width, int height) {
    mRootView->invalidateAll();
    resizeInternal(width, height);
    scheduleLayout();

    for (auto listener : mListeners) {
        listener->onSized(width, height);
    }
}

void sf::Window::onClosed() {
    for (auto listener : mListeners) {
        listener->onClosed();
    }
}

void sf::Window::onMaximized() {
    for (auto listener : mListeners) {
        listener->onMaximized();
    }
}

void sf::Window::onMinimized() {
    for (auto listener : mListeners) {
        listener->onMinimized();
    }
}

void sf::Window::onRestored() {
    for (auto listener : mListeners) {
        listener->onRestored();
    }
}

void sf::Window::onPaint() {
    if (mDirtyRgn.isEmpty()) {
        return;
    }

    SkBitmap* bitmap = getPaintBitmap();
    if (bitmap->width() == 0 || bitmap->height() == 0 || !bitmap->rowBytes()) {
        return;
    }

    SkSurfaceProps props = SkSurfaceProps::kLegacyFontHost_InitType;
    SkAutoTUnref<SkSurface> surface(
        SkSurface::NewRasterDirect(
            bitmap->info(), bitmap->getPixels(), 
            bitmap->rowBytes(), &props));

    SkCanvas* canvas = surface->getCanvas();
    SkAutoCanvasRestore lock(canvas, true);
    canvas->clipRegion(mDirtyRgn);
    canvas->clear(SK_ColorTRANSPARENT);
    mDirtyRgn.setEmpty();

    draw(canvas);
}

sf::HitTestZone sf::Window::onHitTest(int x, int y) {
    ref_ptr<HitTestMessageResult> result = reinterpret_cast<HitTestMessageResult*>(
        mRootView->routeMessage(HitTestMessage(x, y)));
    if (result && result->zone != HitTestZone::None) {
        return result->zone;
    }

    if (mSizeableType == SizeableType::Disabled || isMaximized()) {
        return HitTestZone::Client;
    }

    Size size = getSize();

    const int leftPos = 0;
    const int rightPos = size.width;
    const int topPos = 0;
    const int bottomPos = size.height;
    const int JUDGE_OFFSET = static_cast<int>(6 * mContext.getPixelDensity());

    const bool horizontal = sf::checkFlag(mSizeableType, SizeableType::HorizontalSizeable);
    const bool vertical = sf::checkFlag(mSizeableType, SizeableType::VerticalSizeable);

    const bool left = x >= leftPos && x <= leftPos + JUDGE_OFFSET;
    const bool right = !left && x <= rightPos && x >= rightPos - JUDGE_OFFSET;
    const bool top = y >= topPos && y <= topPos + JUDGE_OFFSET;
    const bool bottom = !top && y <= bottomPos && y >= bottomPos - JUDGE_OFFSET;

    if (horizontal) {
        if (left) {
            if (vertical) {
                if (top) {
                    return HitTestZone::TopLeft;
                } else if (bottom) {
                    return HitTestZone::BottomLeft;
                }
            }

            return HitTestZone::Left;
        } else if (right) {
            if (vertical) {
                if (top) {
                    return HitTestZone::TopRight;
                } else if (bottom) {
                    return HitTestZone::BottomRight;
                }
            }

            return HitTestZone::Right;
        }
    }

    if (vertical) {
        if (top) {
            if (horizontal) {
                if (left) {
                    return HitTestZone::TopLeft;
                } else if (right) {
                    return HitTestZone::TopRight;
                }
            }

            return HitTestZone::Top;
        } else if (bottom) {
            if (horizontal) {
                if (left) {
                    return HitTestZone::BottomLeft;
                } else if (right) {
                    return HitTestZone::BottomRight;
                }
            }

            return HitTestZone::Bottom;
        }
    }

    return HitTestZone::Client;
}

void sf::Window::onHandleNativeMessage(NativeWindowMessage* msg) {
    for (auto listener : mNativeMsgMonitorList) {
        listener->onHandleNativeMessage(msg);
    }
}

void sf::Window::draw(SkCanvas* canvas) {
    mRootView->dispatchDraw(canvas);
}

void sf::Window::resizeInternal(int width, int height) {
    SkImageInfo info = mBitmap.info().makeWH(width, height);
    mBitmap.allocPixels(info);
}

SkBitmap* sf::Window::getPaintBitmap() {
    return &mBitmap;
}

sf::Size sf::Window::getMinSize() {
    return mMinSize;
}

sf::Size sf::Window::getMaxSize() {
    return mMaxSize;
}

void sf::Window::scheduleLayout() {
    straw_check_thread("Cannot scheduleLayout in sub thread!");

    Size size = getSize();
    MeasureSpec width;
    MeasureSpec height;
    if (mWidthWrapContent) {
        width.set(MeasureMode::Unspecified, LayoutParams::WRAP_CONTENT);
    } else {
        width.exactly(size.width);
    }

    if (mHeightWrapContent) {
        height.set(MeasureMode::Unspecified, LayoutParams::WRAP_CONTENT);
    } else {
        height.exactly(size.height);
    }

    mRootView->dispatchMeasure(width, height);
    mRootView->dispatchLayout();

    bool needUpdateSize = false;
    Size rootViewSize = mRootView->getSize();
    if (mWidthWrapContent && size.width != rootViewSize.width) {
        size.width = rootViewSize.width;
        needUpdateSize = true;
    }

    if (mHeightWrapContent && size.height != rootViewSize.height) {
        size.height = rootViewSize.height;
        needUpdateSize = true;
    }

    if (needUpdateSize) {
        setSize(size);
    }

    scheduleInvalidate();
}

void sf::Window::scheduleInvalidate(
    Rect* dirty /*= nullptr*/, bool immediately /*= false*/) {

    straw_check_thread("Cannot scheduleInvalidate in sub thread!");

    if (dirty) {
        mDirtyRgn.op(
            dirty->left, dirty->top,
            dirty->right, dirty->bottom,
            SkRegion::kUnion_Op);
    } else {
        Size size = getSize();
        mDirtyRgn.setRect(0, 0, size.width, size.height);
    }

    mWindowImpl->invalidate(immediately);
}

sf::NativeWindow sf::Window::getNativeWindowInfo() const {
    return mWindowImpl->getNativeWindowInfo();
}

void sf::Window::addNativeMsgMonitor(NativeWindowMessageMonitor& monitor) {
    mNativeMsgMonitorList.addListener(&monitor);
}
