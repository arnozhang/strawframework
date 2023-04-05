#include "WindowsWndImpl.h"

#undef _T
#include <algorithm>
#include <windowsx.h>
#include <tchar.h>
#include <Gdiplus.h>
#include <atlcomcli.h>

#include "base/def/FrameworkDef.h"
#include "base/StrawUtils.h"
#include "base/data/Buffer.h"

#include "thirdparty/skia/core/SkImageEncoder.h"

#pragma comment(lib, "Gdiplus.lib")


namespace {

    const TCHAR* const CLASS_NAME = _T("StrawFramework_class");
    const TCHAR* const WINDOW_NAME = _T("StrawFramework_window");
    ULONG_PTR g_gdiplusToken = 0;


    int covertKeyDownFlag(int flag) {
        int downFlag = 0;
        if (flag & MK_CONTROL) {
            downFlag |= sf::KeyDownFlag::Control;
        }

        if (flag & MK_SHIFT) {
            downFlag |= sf::KeyDownFlag::Shift;
        }

        if (flag & MK_LBUTTON) {
            downFlag |= sf::KeyDownFlag::LButton;
        }

        if (flag & MK_MBUTTON) {
            downFlag |= sf::KeyDownFlag::MButton;
        }

        if (flag & MK_RBUTTON) {
            downFlag |= sf::KeyDownFlag::RButton;
        }

        return downFlag;
    }

    LRESULT covertHitTestZone(sf::HitTestZone zone) {
        switch (zone) {
        case sf::HitTestZone::Client:
            return HTCLIENT;
        case sf::HitTestZone::SysMenu:
            return HTSYSMENU;
        case sf::HitTestZone::CaptionBar:
            return HTCAPTION;
        case sf::HitTestZone::MinButton:
            return HTMINBUTTON;
        case sf::HitTestZone::MaxButton:
            return HTMAXBUTTON;
        case sf::HitTestZone::CloseButton:
            return HTCLOSE;
        case sf::HitTestZone::Left:
            return HTLEFT;
        case sf::HitTestZone::Top:
            return HTTOP;
        case sf::HitTestZone::Right:
            return HTRIGHT;
        case sf::HitTestZone::Bottom:
            return HTBOTTOM;
        case sf::HitTestZone::TopLeft:
            return HTTOPLEFT;
        case sf::HitTestZone::TopRight:
            return HTTOPRIGHT;
        case sf::HitTestZone::BottomLeft:
            return HTBOTTOMLEFT;
        case sf::HitTestZone::BottomRight:
            return HTBOTTOMRIGHT;
        default:
            break;
        }

        return HTCLIENT;
    }

    LPWSTR covertSystemCursor(sf::SystemCursorType type) {
        switch (type) {
        case sf::SmallWaiting:
            return IDC_APPSTARTING;
        case sf::LargeWaiting:
            return IDC_WAIT;
        case sf::Arrow:
            return IDC_ARROW;
        case sf::Cross:
            return IDC_CROSS;
        case sf::Hand:
            return IDC_HAND;
        case sf::Help:
            return IDC_HELP;
        case sf::IBeam:
            return IDC_IBEAM;
        case sf::SizeAll:
            return IDC_SIZEALL;
        case sf::SizeNESW:
            return IDC_SIZENESW;
        case sf::SizeNS:
            return IDC_SIZENS;
        case sf::SizeNWSE:
            return IDC_SIZENWSE;
        case sf::SizeWE:
            return IDC_SIZEWE;
        default:
            break;
        }

        return IDC_ARROW;
    }

    HICON createIconFromContent(sf::Buffer* content) {
        auto size = content->size();
        HGLOBAL mem = ::GlobalAlloc(GMEM_MOVEABLE, size);
        LPVOID buffer = ::GlobalLock(mem);
        ::memcpy(buffer, content->buffer(), size);
        ::GlobalUnlock(mem);

        HICON icon = NULL;
        ATL::CComPtr<IStream> stream;
        ::CreateStreamOnHGlobal(mem, TRUE, &stream);
        if (stream) {
            std::unique_ptr<Gdiplus::Bitmap> bitmap =
                std::make_unique<Gdiplus::Bitmap>(stream);
            if (bitmap) {
                bitmap->GetHICON(&icon);
            }
        }

        ::GlobalFree(mem);
        return icon;
    }

} // anonymous namespace ends here.


HHOOK sf::WindowsWndImpl::mMessageHook = NULL;


sf::WindowsWndImpl::WindowsWndImpl(const Context& context, WindowHost& listener)
    : WindowImpl(context, listener)
    , mHwnd(NULL)
    , mAlpha(1.0f)
    , mCursorHandle(NULL)
    , mModalWnd(false)
    , mWaitNCMouseMessage(false)
    , mMouseHovered(false) {

    if (!g_gdiplusToken) {
        Gdiplus::GdiplusStartupInput input;
        Gdiplus::GdiplusStartup(&g_gdiplusToken, &input, nullptr);
    }
}

sf::WindowsWndImpl::~WindowsWndImpl() {
}

bool sf::WindowsWndImpl::create(
    BaseWindow* parent,
    bool modal,
    int xPosition, int yPosition,
    int width, int height) {

    mModalWnd = modal;
    mCursorHandle = ::LoadCursor(NULL, IDC_ARROW);

    WNDCLASSEX wnd = { 0 };
    wnd.cbSize = sizeof(wnd);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = sizeof(this);
    wnd.lpszClassName = CLASS_NAME;
    wnd.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS | CS_IME;
    wnd.hInstance = ::GetModuleHandle(NULL);
    wnd.hCursor = mCursorHandle;
    wnd.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wnd.hbrBackground = (HBRUSH) ::GetStockObject(WHITE_BRUSH);
    wnd.lpfnWndProc = &WindowsWndImpl::handleMessageProc;

    if (!::RegisterClassEx(&wnd)) {
        DWORD error = ::GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            return false;
        }
    }

    if (xPosition < 0) {
        xPosition = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    }

    if (yPosition < 0) {
        yPosition = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    }

    int style = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    if (mHost.getMinimizable()) {
        style |= WS_MINIMIZEBOX;
    }

    if (mHost.getMaximizable()) {
        style |= WS_MAXIMIZEBOX;
    }

    HWND hParentWnd = NULL;
    if (parent) {
        sf::NativeWindow native = parent->getNativeWindowInfo();
        hParentWnd = native.hwnd;
    }

    mHwnd = ::CreateWindowEx(
        WS_EX_LAYERED,
        CLASS_NAME,
        WINDOW_NAME,
        style,
        xPosition,
        yPosition,
        width,
        height,
        hParentWnd,
        NULL,
        wnd.hInstance,
        this);

    if (mHwnd != NULL && hParentWnd != NULL && mModalWnd) {
        ::EnableWindow(hParentWnd, FALSE);
    }

    if (mMessageHook == NULL) {
        mMessageHook = ::SetWindowsHookEx(
            WH_GETMESSAGE, &handleHookMessageProc, NULL, ::GetCurrentThreadId());
    }

    return mHwnd != NULL;
}

void sf::WindowsWndImpl::invalidate(bool immediately /*= false*/) {
    if (immediately) {
        ::SendMessage(mHwnd, WM_PAINT, 0, 0);
    } else {
        ::PostMessage(mHwnd, WM_PAINT, 0, 0);
    }
}

void sf::WindowsWndImpl::close() {
    ::SendMessage(mHwnd, WM_CLOSE, 0, 0);
}

void sf::WindowsWndImpl::show() {
    ::ShowWindow(mHwnd, SW_SHOWNORMAL);
    ::UpdateWindow(mHwnd);
}

void sf::WindowsWndImpl::hide() {
    ::ShowWindow(mHwnd, SW_HIDE);
}

void sf::WindowsWndImpl::setFocus() {
    ::SetFocus(mHwnd);
}

void sf::WindowsWndImpl::setPosition(int x, int y) {
    ::SetWindowPos(mHwnd, NULL, x, y, 0, 0,
        SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void sf::WindowsWndImpl::setSize(int width, int height) {
    ::SetWindowPos(mHwnd, NULL, 0, 0, width, height,
        SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
}

void sf::WindowsWndImpl::setAlpha(float alpha) {
    mAlpha = max(0, min(alpha, 0xff));
    invalidate();
}

void sf::WindowsWndImpl::setCenterInScreen(bool center) {
    if (!center) {
        return;
    }

    Size size = getSize();
    Size workZone = sf::getWorkZoneScreenSize();
    Size screen = sf::getScreenSize();
    workZone.height += ::GetSystemMetrics(SM_CYCAPTION);

    Point pos(
        (workZone.width - size.width) / 2,
        (workZone.height - size.height) / 2);
    if (isMaximized()) {
        APPBARDATA data = { 0 };
        ::SHAppBarMessage(ABM_GETTASKBARPOS, &data);

        switch (data.uEdge) {
        case ABE_TOP:
            pos.y += screen.height - workZone.height;
            break;
        case ABE_LEFT:
            pos.x += screen.width - workZone.width;
            break;
        case ABE_RIGHT:
        case ABE_BOTTOM:
        default:
            break;
        }
    }

    setPosition(pos.x, pos.y);
}

sf::Point sf::WindowsWndImpl::getPosition() const {
    RECT rc;
    ::GetWindowRect(mHwnd, &rc);
    return sf::Point(rc.left, rc.top);
}

sf::Size sf::WindowsWndImpl::getSize() const {
    RECT rc;
    ::GetWindowRect(mHwnd, &rc);
    return sf::Size(rc.right - rc.left, rc.bottom - rc.top);
}

float sf::WindowsWndImpl::getAlpha() const {
    return mAlpha;
}

void sf::WindowsWndImpl::setTitle(const std::string& title) {
    int size = ::MultiByteToWideChar(
        CP_UTF8, 0, title.c_str(), title.size(), nullptr, 0);

    WCHAR* buffer = new WCHAR[size + 1];
    ::MultiByteToWideChar(CP_UTF8, 0, title.c_str(), title.size(), buffer, size);
    buffer[size] = 0;

    ::SetWindowText(mHwnd, buffer);
    delete[] buffer;
}

std::string sf::WindowsWndImpl::getTitle() const {
    WCHAR title[256] = { 0 };
    ::GetWindowText(mHwnd, title, 255);

    int titleLength = wcslen(title);
    int size = ::WideCharToMultiByte(
        CP_UTF8, 0, title, titleLength, nullptr, 0, nullptr, nullptr);

    char* buffer = new char[size + 1];
    ::WideCharToMultiByte(
        CP_UTF8, 0, title, titleLength, buffer, size, nullptr, nullptr);

    buffer[size] = 0;
    std::string result(buffer);
    delete[] buffer;

    return result;
}

void sf::WindowsWndImpl::setIcon(Buffer* content, bool large) {
    if (!content || content->empty()){
        return;
    }

    HICON icon = createIconFromContent(content);
    if (icon) {
        SendMessage(mHwnd, WM_SETICON, large ? ICON_BIG : ICON_SMALL, (LPARAM) icon);
    }
}

void sf::WindowsWndImpl::setCursor(Buffer* content) {
    if (!content || content->empty()) {
        return;
    }

    HCURSOR cursor = (HCURSOR) ::CreateIconFromResourceEx(
        reinterpret_cast<PBYTE>(content->buffer()),
        content->size(), FALSE, 0x00030000, 0, 0,
        LR_DEFAULTSIZE | LR_DEFAULTCOLOR);
    if (cursor) {
        mCursorHandle = cursor;
        ::SetCursor(mCursorHandle);
    }
}

void sf::WindowsWndImpl::setSystemCursor(SystemCursorType type) {
    auto id = covertSystemCursor(type);
    HCURSOR cursor = (HCURSOR) ::LoadImage(
        NULL, id, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    if (cursor) {
        mCursorHandle = cursor;
        ::SetCursor(mCursorHandle);
    }
}

void sf::WindowsWndImpl::bringToTopMost() const {
    ::SetWindowPos(mHwnd, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void sf::WindowsWndImpl::cancelTopMost() const {
    ::SetWindowPos(mHwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void sf::WindowsWndImpl::setMinimizable(bool enable) {
    long style = ::GetWindowLong(mHwnd, GWL_STYLE);
    if (enable) {
        style |= WS_MINIMIZEBOX;
    } else {
        style &= ~WS_MINIMIZEBOX;
    }

    ::SetWindowLong(mHwnd, GWL_STYLE, style);
}

void sf::WindowsWndImpl::setMaximizable(bool enable) {
    long style = ::GetWindowLong(mHwnd, GWL_STYLE);
    if (enable) {
        style |= WS_MAXIMIZEBOX;
    } else {
        style &= ~WS_MAXIMIZEBOX;
    }

    ::SetWindowLong(mHwnd, GWL_STYLE, style);
}

void sf::WindowsWndImpl::minWindow() const {
    if (mHost.getMinimizable()) {
        ::ShowWindow(mHwnd, SW_SHOWMINIMIZED);
    }
}

void sf::WindowsWndImpl::maxOrResotreWindow() const {
    if (isMaximized()) {
        ::ShowWindow(mHwnd, SW_SHOWNORMAL);
    } else if (!isMinimized()) {
        if (mHost.getMaximizable()) {
            ::ShowWindow(mHwnd, SW_MAXIMIZE);
        }
    }
}

bool sf::WindowsWndImpl::isMinimized() const {
    return !!::IsIconic(mHwnd);
}

bool sf::WindowsWndImpl::isMaximized() const {
    return !!::IsZoomed(mHwnd);
}

bool sf::WindowsWndImpl::isShown() const {
    return !!::IsWindowVisible(mHwnd);
}

bool sf::WindowsWndImpl::isActived() const {
    return ::GetActiveWindow() == mHwnd;
}

void sf::WindowsWndImpl::requestFocus() const {
    ::SetFocus(mHwnd);
}

sf::Point sf::WindowsWndImpl::getCursorPosition() const {
    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(mHwnd, &pt);
    return Point(pt.x, pt.y);
}

sf::Point sf::WindowsWndImpl::getCursorPositionInScreen() const {
    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    return Point(pt.x, pt.y);
}

sf::NativeWindow sf::WindowsWndImpl::getNativeWindowInfo() const {
    NativeWindow native;
    native.hwnd = mHwnd;
    return native;
}

void sf::WindowsWndImpl::onAppTerminate() {
    if (mMessageHook != NULL) {
        ::UnhookWindowsHookEx(mMessageHook);
        mMessageHook = NULL;
    }

    ::Gdiplus::GdiplusShutdown(g_gdiplusToken);
    g_gdiplusToken = 0;
}

void sf::WindowsWndImpl::onPaint(HDC hdc) {
    mHost.onPaint();

    SkBitmap* bitmap = mHost.getPaintBitmap();
    if (!bitmap) {
        return;
    }

    const int width = bitmap->width();
    const int height = bitmap->height();
    HDC compatibleDC = ::CreateCompatibleDC(hdc);
    HBITMAP compatibleBitmap = ::CreateCompatibleBitmap(hdc, width, height);
    ::SelectObject(compatibleDC, compatibleBitmap);

    BITMAPINFO info = { 0 };
    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biWidth = width;
    info.bmiHeader.biHeight = -height;  // Flip
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biSizeImage = 0;

    bitmap->lockPixels();
    ::SetDIBitsToDevice(
        compatibleDC,
        0, 0,
        width, height,
        0, 0,
        0, height,
        bitmap->getPixels(),
        &info,
        DIB_RGB_COLORS
        );
    bitmap->unlockPixels();

    Point pos = getPosition();
    POINT ptDest = { pos.x, pos.y };
    POINT ptSrc = { 0, 0 };
    SIZE size = { width, height };

    BLENDFUNCTION blend = { 0 };
    blend.AlphaFormat = AC_SRC_ALPHA;
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = static_cast<BYTE>(mAlpha * 0xff);

    ::UpdateLayeredWindow(
        mHwnd, hdc, &ptDest, &size,
        compatibleDC, &ptSrc, 0, &blend, ULW_ALPHA);

    ::SelectObject(compatibleDC, NULL);
    ::DeleteBitmap(compatibleBitmap);
    ::DeleteDC(compatibleDC);
}

void sf::WindowsWndImpl::trackMouseEvent() {
    TRACKMOUSEEVENT evt;
    evt.cbSize = sizeof(evt);
    evt.hwndTrack = mHwnd;
    evt.dwHoverTime = HOVER_DEFAULT;
    evt.dwFlags = TME_HOVER | TME_LEAVE;
    ::TrackMouseEvent(&evt);
}

LRESULT sf::WindowsWndImpl::handleMessage(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    NativeWindowMessage native(hwnd, msg, wParam, lParam);
    mHost.onHandleNativeMessage(&native);
    if (native.handled) {
        return native.lResult;
    }

    switch (msg)
    {
    case WM_ERASEBKGND:
    case WM_NCPAINT:
    case WM_SYNCPAINT:
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hwnd, &ps);
        onPaint(hdc);
        ::EndPaint(hwnd, &ps);
        break;
    }

    case WM_SETCURSOR: {
        if (LOWORD(lParam) == HTCLIENT) {
            if (::GetCursor() != mCursorHandle) {
                ::SetCursor(mCursorHandle);
            }

            return TRUE;
        }
        break;
    }

    case WM_GETMINMAXINFO: {
        LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(lParam);
        Size size = sf::getWorkZoneScreenSize();
        Rect wndShadow = mHost.getWindowShadow();
        info->ptMaxSize.x = size.width + wndShadow.horzSum();
        info->ptMaxSize.y = size.height
            + ::GetSystemMetrics(SM_CYCAPTION) + wndShadow.vertSum();
        return TRUE;
    }

    case WM_NCLBUTTONDOWN:
    case WM_LBUTTONDOWN: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (msg == WM_NCLBUTTONDOWN) {
            mWaitNCMouseMessage = true;
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onLButtonDown(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCLBUTTONUP:
    case WM_LBUTTONUP: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (msg == WM_NCLBUTTONUP) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onLButtonUp(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCRBUTTONDOWN:
    case WM_RBUTTONDOWN: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (msg == WM_NCRBUTTONDOWN) {
            mWaitNCMouseMessage = true;
            ::ScreenToClient(mHwnd, &pt);
        }
     
        mHost.onRButtonDown(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCRBUTTONUP:
    case WM_RBUTTONUP: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (msg == WM_NCRBUTTONUP) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onRButtonUp(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCMBUTTONDOWN:
    case WM_MBUTTONDOWN: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMBUTTONDOWN) {
            mWaitNCMouseMessage = true;
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onMButtonDown(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCMBUTTONUP:
    case WM_MBUTTONUP: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMBUTTONUP) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onMButtonUp(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCLBUTTONDBLCLK:
    case WM_LBUTTONDBLCLK: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCLBUTTONDBLCLK) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onLButtonDblClick(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCMBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMBUTTONDBLCLK) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onMButtonDblClick(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCRBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCRBUTTONDBLCLK) {
            ::ScreenToClient(mHwnd, &pt);
        }

        mHost.onRButtonDblClick(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_NCMOUSEMOVE:
    case WM_MOUSEMOVE: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMOUSEMOVE) {
            ::ScreenToClient(mHwnd, &pt);
        }

        if (!mMouseHovered) {
            mMouseHovered = true;
            mHost.onMouseHovered();
            trackMouseEvent();
        }

        mHost.onMouseMove(covertKeyDownFlag(wParam), pt.x, pt.y);
        break;
    }

    case WM_MOUSEHWHEEL: {
        mHost.onMouseHorizonWheel();
        break;
    }

    case WM_MOUSEWHEEL: {
        mHost.onMouseWheel();
        break;
    }

    case WM_MOVE: {
        mHost.onMoved(LOWORD(lParam), HIWORD(lParam));
        break;
    }

    case WM_MOVING: {
        mHost.onMoving();
        break;
    }

    case WM_SIZE: {
        mHost.onSized(LOWORD(lParam), HIWORD(lParam));
        if (wParam == SIZE_MAXIMIZED) {
            mHost.onMaximized();
            setCenterInScreen(true);
        } else if (wParam == SIZE_MINIMIZED) {
            mHost.onMinimized();
        } else if (wParam == SIZE_RESTORED) {
            mHost.onRestored();
        }

        break;
    }

    case WM_SIZING: {
        mHost.onSizing();

        RECT* rc = reinterpret_cast<RECT*>(lParam);
        Size minSize = mHost.getMinSize();
        Size maxSize = mHost.getMaxSize();

        bool handled = false;
        if (minSize.width > 0) {
            if (rc->right - rc->left < minSize.width) {
                rc->right = rc->left + minSize.width;
                handled = true;
            }
        }

        if (minSize.height > 0) {
            if (rc->bottom - rc->top < minSize.height) {
                rc->bottom = rc->top + minSize.height;
                handled = true;
            }
        }

        if (maxSize.width > 0) {
            if (rc->right - rc->left > maxSize.width) {
                rc->right = rc->left + maxSize.width;
                handled = true;
            }
        }

        if (maxSize.height > 0) {
            if (rc->bottom - rc->top > maxSize.height) {
                rc->bottom = rc->top + maxSize.height;
                handled = true;
            }
        }

        if (handled) {
            return TRUE;
        }
        break;
    }

    case WM_KEYDOWN: {
        mHost.onKeyDown(wParam);
        break;
    }

    case WM_KEYUP: {
        mHost.onKeyUp(wParam);
        break;
    }

    case WM_CHAR: {
        
        //mImplListener.onChar(SkUTF8_ToUnichar((char*)&wParam));
        break;
    }

    case WM_UNICHAR: {
        mHost.onChar(wParam);
        break;
    }

    case WM_SHOWWINDOW: {
        if (wParam == FALSE) {
            mHost.onHidden();
        } else {
            mHost.onShown();
        }
        break;
    }

    case WM_SETFOCUS: {
        NativeWindow wnd;
        wnd.hwnd = reinterpret_cast<HWND>(wParam);
        mHost.onSetFocus(wnd);
        break;
    }

    case WM_KILLFOCUS: {
        NativeWindow wnd;
        wnd.hwnd = reinterpret_cast<HWND>(wParam);
        mHost.onKillFocus(wnd);
        break;
    }

    case WM_ACTIVATE: {
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE) {
            mHost.onActived();
        } else {
            mHost.onDeactived();
        }
        break;
    }

    case WM_ENABLE: {
        if (wParam == TRUE) {
            mHost.onEnabled();
        } else {
            mHost.onDisabled();
        }
        break;
    }

    case WM_NCMOUSEHOVER:
    case WM_MOUSEHOVER: {
        trackMouseEvent();
        break;
    }

    case WM_NCMOUSELEAVE:
    case WM_MOUSELEAVE: {
        mMouseHovered = false;
        mHost.onMouseLeaved();
        break;
    }

    case WM_QUERYENDSESSION: {
        mHost.onSystemPrepareTerminate();
        break;
    }

    case WM_NCHITTEST: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ::ScreenToClient(mHwnd, &pt);
        return covertHitTestZone(mHost.onHitTest(pt.x, pt.y));
    }

    case WM_CLOSE: {
        bool cancel = false;
        mHost.onClosing(&cancel);
        if (cancel) {
            return 0;
        }

        HWND hParentWnd = ::GetParent(hwnd);
        if (hParentWnd != NULL && mModalWnd) {
            ::EnableWindow(hParentWnd, TRUE);
        }

        ::DestroyWindow(hwnd);
        break;
    }

    case WM_DESTROY: {
        mHost.onClosed();
        break;
    }

    default:
        break;
    }

    return ::DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT sf::WindowsWndImpl::handleMessageProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (msg == WM_CREATE) {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, static_cast<LONG>(
            reinterpret_cast<ULONG_PTR>(lpcs->lpCreateParams)));

        WindowsWndImpl* impl= reinterpret_cast<WindowsWndImpl*>(lpcs->lpCreateParams);
        impl->mHwnd = hwnd;
        impl->invalidate();
    } else {
        WindowsWndImpl* impl = reinterpret_cast<WindowsWndImpl*>(
            static_cast<ULONG_PTR>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)));

        if (impl) {
            return impl->handleMessage(hwnd, msg, wParam, lParam);
        }
    }

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK sf::WindowsWndImpl::handleHookMessageProc(
    int code, WPARAM wParam, LPARAM lParam) {

    if (code == HC_ACTION) {
        MSG* msg = reinterpret_cast<MSG*>(lParam);
        if (msg->message == WM_LBUTTONUP
            || msg->message == WM_RBUTTONUP
            || msg->message == WM_MBUTTONUP) {

            WindowsWndImpl* impl = reinterpret_cast<WindowsWndImpl*>(
                static_cast<ULONG_PTR>(::GetWindowLongPtr(msg->hwnd, GWLP_USERDATA)));
            if (impl && impl->mWaitNCMouseMessage) {
                impl->mWaitNCMouseMessage = false;
                return impl->handleMessage(
                    msg->hwnd, msg->message, msg->wParam, msg->lParam);
            }
        }
    }

    return ::CallNextHookEx(mMessageHook, code, wParam, lParam);
}
