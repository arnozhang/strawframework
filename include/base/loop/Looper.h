/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#pragma once

#include <queue>
#include <thread>

#include "base/loop/TaskScheduler.h"


namespace sf {

    class STRAW_FRAMEWORK_API Looper : public TaskScheduler {

    public:
        Looper();
        virtual ~Looper();

        virtual TaskId postTask(const Task& task) override;
        virtual TaskId postTaskDelay(const Task& task, uint64_t delayMillSeconds) override;
        virtual TaskId postMostPriority(const Task& task) override;
        virtual TaskId postMorePriority(const Task& task) override;
        virtual TaskId postIdleTask(const Task& task) override;

        virtual void cancelTask(TaskId taskId) override;
        virtual void cancelIdleTask(TaskId taskId) override;

        virtual void start();
        virtual void terminate();
        virtual bool isTerminated();

        static const std::thread::id& inMainThread();
        static const std::thread::id& getMainThreadId();
        static Looper& mainLooper();
        static Looper& myLooper();
        static void releaseLooper();

    protected:
        virtual bool doWork() = 0;
        virtual bool doIdle();
        virtual bool scheduleTask();
        virtual void waitForWork();

        bool hasTask() const;

    protected:
        enum class Priority {
            Normal,
            MoreHigh,
            MostHigh,
        };

        struct TaskItem {
            TaskItem(
                TaskId _id,
                const Task& _task,
                uint64_t _delay = 0,
                Priority _priority = Priority::Normal);

            TaskId taskId;
            Task task;
            uint64_t executeTime;
            Priority priority;
        };

        bool mIsTerminated;
        TaskId mCurrentId; 
        std::deque<TaskItem> mTaskQueue;
        std::deque<TaskItem> mIdleTask;
        std::vector<TaskItem> mTempScheduleQueue;
        std::mutex mLock;
        std::condition_variable mTaskCondition;

    private:
        TaskId nextTaskId();
        TaskId addNormalTask(const TaskItem& item);
    };
}
