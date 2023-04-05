#include "WindowsTimerImpl.h"

#include <thread>


sf::impl::WindowsTimerImpl::WindowsTimerImpl() {
    mTimerHwnd = NULL;
}

sf::impl::WindowsTimerImpl::~WindowsTimerImpl() {
}

void sf::impl::WindowsTimerImpl::initializeInternal() {
    std::thread initThread([this]() {
        initializeTimer();
    });

    initThread.detach();
}

void sf::impl::WindowsTimerImpl::unInitializeInternal() {
    ::DestroyWindow(mTimerHwnd);
}

void sf::impl::WindowsTimerImpl::setTimerInternal(const TimerItem& item) {
    if (mTimerHwnd) {
        ::SetTimer(
            mTimerHwnd,
            reinterpret_cast<UINT_PTR>(item.timerId),
            item.elapseMillSeconds,
            nullptr);
    }
}

void sf::impl::WindowsTimerImpl::killTimerInternal(timer::TimerId timerId) {
    if (mTimerHwnd) {
        ::KillTimer(mTimerHwnd, reinterpret_cast<UINT_PTR>(timerId));
    }
}

void sf::impl::WindowsTimerImpl::initializeTimer() {
    const WCHAR* const TIMER_CLS_NAME = L"StrawFramework_timer_class";
    const WCHAR* const TIMER_WND_NAME = L"StrawFramework_timer_window";

    WNDCLASSEXW wndcls = { sizeof(wndcls) };
    wndcls.hInstance = ::GetModuleHandle(NULL);
    wndcls.lpszClassName = TIMER_CLS_NAME;
    wndcls.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
    wndcls.lpfnWndProc = &WindowsTimerImpl::OnTimerWndProc;
    wndcls.style = CS_HREDRAW | CS_VREDRAW;

    ::RegisterClassExW(&wndcls);

    mTimerHwnd = ::CreateWindowExW(
        0,
        TIMER_CLS_NAME,
        TIMER_WND_NAME,
        WS_OVERLAPPED,
        0, 0, 0, 0,
        HWND_MESSAGE,
        NULL,
        wndcls.hInstance,
        NULL);

    if (!mTimerHwnd) {
        return;
    }

    ::ShowWindow(mTimerHwnd, SW_HIDE);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}

HRESULT CALLBACK sf::impl::WindowsTimerImpl::OnTimerWndProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_TIMER:{
        void* timerId = reinterpret_cast<timer::TimerId>(wParam);
        TimerImpl::getInstance().onTimer(timerId);
    }
    break;
    case WM_DESTROY: {
        ::PostQuitMessage(0);
    }
    default:
        break;
    }

    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
