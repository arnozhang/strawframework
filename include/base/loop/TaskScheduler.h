/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#pragma once

#include <mutex>
#include <functional>

#include "base/def/PlatformDefine.h"


namespace sf {

    using Task = std::function<void(void)>;
    using TaskId = int;
    const static TaskId INVALID_ID = 0;


    class STRAW_FRAMEWORK_API TaskScheduler {

    public:
        TaskScheduler() {}
        virtual ~TaskScheduler() {}

        virtual TaskId postTask(const Task& task) = 0;
        virtual TaskId postTaskDelay(const Task& task, uint64_t delayMillSeconds) = 0;
        virtual TaskId postMostPriority(const Task& task) = 0;
        virtual TaskId postMorePriority(const Task& task) = 0;
        virtual TaskId postIdleTask(const Task& task) = 0;

        virtual void cancelTask(TaskId taskId) = 0;
        virtual void cancelIdleTask(TaskId taskId) = 0;
    };

}
