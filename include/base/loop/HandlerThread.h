/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#pragma once

#include <thread>

#include "base/loop/TaskScheduler.h"


namespace sf {

    class STRAW_FRAMEWORK_API Looper;


    class STRAW_FRAMEWORK_API HandlerThread {

    public:
        HandlerThread();
        virtual ~HandlerThread();

        virtual void start();
        virtual void terminate();

        Looper& getLooper();

    protected:
        void threadProc();

    protected:
        std::mutex mLock;
        std::condition_variable mCondition;
        std::unique_ptr<std::thread> mThread;
        Looper* mLooper;
    };
}
