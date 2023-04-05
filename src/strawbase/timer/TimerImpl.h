/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/23
 */

#pragma once

#include <map>

#include "base/timer/Timer.h"


namespace sf {

    namespace impl {

        struct TimerItem {
            timer::TimerId timerId;
            timer::TimerCallback callback;
            bool loopTimer;
            int elapseMillSeconds;
        };


        class TimerImpl {

        public:
            TimerImpl();
            virtual ~TimerImpl();

            static TimerImpl& getInstance();
            virtual void initialize();
            virtual void unInitialize();
            virtual void onTimer(timer::TimerId timerId);

            virtual void setTimerCallback(
                timer::TimerId timerId,
                int elapseMillSeconds,
                timer::TimerCallback callback,
                bool loopTimer = true);


            virtual void killTimerCallback(
                timer::TimerId timerId);

        protected:
            using TimerList = std::map<timer::TimerId, TimerItem>;

            virtual void initializeInternal() = 0;
            virtual void unInitializeInternal() = 0;
            virtual void setTimerInternal(const TimerItem& item) = 0;
            virtual void killTimerInternal(timer::TimerId timerId) = 0;

        private:
            bool mInitialized;
            TimerList mTimerList;
            static TimerImpl* mInstance;
        };
    }
}
