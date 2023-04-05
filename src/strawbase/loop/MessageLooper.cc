#include "base/loop/MessageLooper.h"

#include <Windows.h>


sf::MessageLooper::MessageLooper() {
}

sf::MessageLooper::~MessageLooper() {
}

bool sf::MessageLooper::doWork() {
#ifdef STRAW_BUILD_FOR_WIN32
    return loopForWindows();
#elif defined(STRAW_BUILD_FOR_LINUX)
#endif // STRAW_BUILD_FOR_WIN32

    return false;
}

void sf::MessageLooper::terminate() {
#ifdef STRAW_BUILD_FOR_WIN32
    ::PostQuitMessage(0);
#endif // STRAW_BUILD_FOR_WIN32

    Looper::terminate();
}

bool sf::MessageLooper::loopForWindows() {
    MSG msg;
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }

        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        return true;
    }

    return false;
}
