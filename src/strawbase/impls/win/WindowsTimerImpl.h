/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/23
 */

#pragma once

#include <Windows.h>

#include "../../timer/TimerImpl.h"


namespace sf {

    namespace impl {

        class WindowsTimerImpl : public TimerImpl {

        public:
            WindowsTimerImpl();
            virtual ~WindowsTimerImpl();

        protected:
            virtual void initializeInternal() override;
            virtual void unInitializeInternal() override;
            virtual void setTimerInternal(const TimerItem& item) override;
            virtual void killTimerInternal(timer::TimerId timerId) override;

        private:
            void initializeTimer();
            static HRESULT CALLBACK OnTimerWndProc(
                HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        private:
            HWND mTimerHwnd;
        };
    }
}
