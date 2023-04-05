/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#pragma once

#include <memory>

#include "base/loop/Looper.h"
#include "base/loop/HandlerThread.h"
#include "graphics/app/BaseManager.h"


namespace sf {

    class STRAW_FRAMEWORK_API ThreadManager : public BaseManager {

        DECLARE_MANAGER(ThreadManager);

    public:
        ThreadManager();
        virtual ~ThreadManager();

        virtual void onCreate() override;
        virtual void onTerminate() override;

        Looper& mainLooper();
        Looper& logicLooper();

    private:
        std::unique_ptr<HandlerThread> mLogicHandler;
    };
}
