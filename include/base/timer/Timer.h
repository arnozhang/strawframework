/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/23
 */

#pragma once

#include <functional>

#include "base/def/PlatformDefine.h"


namespace sf {

    namespace timer {

        using TimerCallback = std::function<void(void)>;
        using TimerId = void*;


        void STRAW_FRAMEWORK_API setTimerCallback(
            TimerId timerId,
            int elapseMillSeconds,
            TimerCallback callback,
            bool loopTimer = true);


        void STRAW_FRAMEWORK_API killTimerCallback(
            TimerId timerId);

        void STRAW_FRAMEWORK_API startTimerSystem();

        void STRAW_FRAMEWORK_API terminateTimerSystem();

    }


    using AsyncCallProc = timer::TimerCallback;


    inline void asyncCall(AsyncCallProc proc) {
        int dummy = 0;
        timer::setTimerCallback(&dummy, 50, proc, false);
    }
}
