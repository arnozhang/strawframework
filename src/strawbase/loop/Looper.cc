#include "base/loop/Looper.h"

#include "base/StrawUtils.h"
#include "base/loop/MessageLooper.h"

#include "ThreadLooper.h"


namespace {

    std::thread::id mainThreadId;

    sf::Looper* mainMsgLooper = nullptr;
    thread_local sf::Looper* threadLooper = nullptr;

} // anonymous namespace ends here.


sf::Looper::TaskItem::TaskItem(
    TaskId _id,
    const Task& _task,
    uint64_t _delay /*= 0*/,
    Priority _priority /*= Priority::Normal*/) {

    taskId = _id;
    task = _task;
    executeTime = sf::getCurrentTime() + _delay;
    priority = _priority;
}

sf::Looper::Looper() {
    mIsTerminated = false;
    mCurrentId = 0;
}

sf::Looper::~Looper() {
}

bool sf::Looper::isTerminated() {
    return mIsTerminated;
}

const std::thread::id& sf::Looper::inMainThread() {
    mainThreadId = std::this_thread::get_id();
    return mainThreadId;
}

const std::thread::id& sf::Looper::getMainThreadId() {
    return mainThreadId;
}

sf::Looper& sf::Looper::mainLooper() {
    if (!mainMsgLooper) {
        mainMsgLooper = new MessageLooper();
    }

    return *mainMsgLooper;
}

sf::Looper& sf::Looper::myLooper() {
    if (std::this_thread::get_id() == Looper::getMainThreadId()) {
        return mainLooper();
    }

    if (!threadLooper) {
        threadLooper = new impl::ThreadLooper();
    }

    return *threadLooper;
}

void sf::Looper::releaseLooper() {
    if (threadLooper) {
        delete threadLooper;
        threadLooper = nullptr;
    }
}

sf::TaskId sf::Looper::nextTaskId() {
    return ++mCurrentId;
}

sf::TaskId sf::Looper::addNormalTask(const TaskItem& item) {
    sf::MutexLock lock(mLock);

    bool add = false;
    auto endIter = mTaskQueue.end();
    for (auto iter = mTaskQueue.begin(); iter != endIter; ++iter) {
        if (iter->executeTime > item.executeTime) {
            mTaskQueue.insert(iter, item);
            add = true;
            break;
        }
    }

    if (!add) {
        mTaskQueue.push_back(item);
    }

    mTaskCondition.notify_all();
    return item.taskId;
}

sf::TaskId sf::Looper::postTask(const Task& task) {
    if (mIsTerminated) {
        return INVALID_ID;
    }

    TaskItem item(nextTaskId(), task);
    return addNormalTask(item);
}

sf::TaskId sf::Looper::postTaskDelay(const Task& task, uint64_t delayMillSeconds) {
    if (mIsTerminated) {
        return INVALID_ID;
    }

    TaskItem item(nextTaskId(), task, delayMillSeconds);
    return addNormalTask(item);
}

sf::TaskId sf::Looper::postMostPriority(const Task& task) {
    if (mIsTerminated) {
        return INVALID_ID;
    }

    sf::MutexLock lock(mLock);
    TaskItem item(nextTaskId(), task, 0, Priority::MostHigh);
    if (mTaskQueue.empty()) {
        mTaskQueue.push_front(item);
        return INVALID_ID;
    }

    bool add = false;
    auto endIter = mTaskQueue.end();
    auto normalIter = endIter;
    for (auto iter = mTaskQueue.begin(); iter != endIter; ++iter) {
        if (iter->priority == Priority::MoreHigh) {
            mTaskQueue.insert(iter, item);
            add = true;
            break;
        }

        if (iter->priority == Priority::Normal && normalIter == endIter) {
            normalIter = iter;
        }
    }

    if (!add) {
        if (normalIter == endIter) {
            mTaskQueue.push_back(item);
        } else {
            mTaskQueue.insert(normalIter, item);
        }
    }

    mTaskCondition.notify_all();
    return item.taskId;
}

sf::TaskId sf::Looper::postMorePriority(const Task& task) {
    if (mIsTerminated) {
        return INVALID_ID;
    }

    sf::MutexLock lock(mLock);
    TaskItem item(nextTaskId(), task, 0, Priority::MoreHigh);
    mTaskQueue.push_front(item);
    mTaskCondition.notify_all();

    return item.taskId;
}

sf::TaskId sf::Looper::postIdleTask(const Task& task) {
    sf::MutexLock lock(mLock);
    TaskItem item(nextTaskId(), task);
    mIdleTask.push_back(item);
    mTaskCondition.notify_all();

    return item.taskId;
}

void sf::Looper::cancelTask(TaskId taskId) {
    sf::MutexLock lock(mLock);
    std::remove_if(mTaskQueue.begin(), mTaskQueue.end(), [taskId](auto& item) {
        return taskId == item.taskId;
    });
}

void sf::Looper::cancelIdleTask(TaskId taskId) {
    sf::MutexLock lock(mLock);
    std::remove_if(mIdleTask.begin(), mIdleTask.end(), [taskId](auto& item) {
        return taskId == item.taskId;
    });
}

void sf::Looper::start() {
    mCurrentId = 0;
    mIsTerminated = false;

    while (true) {
        if (mIsTerminated) {
            break;
        }

        bool did = doWork();
        if (mIsTerminated) {
            break;
        }

        did |= scheduleTask();
        if (mIsTerminated) {
            break;
        }

        if (did) {
            continue;
        }

        did |= doIdle();
        if (mIsTerminated) {
            break;
        }

        if (did) {
            continue;
        }

        waitForWork();
    }
}

void sf::Looper::terminate() {
    mIsTerminated = true;

    sf::MutexLock lock(mLock);
    mTaskQueue.clear();

    std::this_thread::yield();
}

bool sf::Looper::doIdle() {
    if (mIdleTask.empty()) {
        return false;
    }

    for (auto& item : mIdleTask) {
        item.task();
    }

    mIdleTask.clear();
    return true;
}

bool sf::Looper::scheduleTask() {
    if (mTaskQueue.empty()) {
        return false;
    }

    bool did = false;
    mTempScheduleQueue.clear();

    {
        sf::MutexLock lock(mLock);
        for (auto iter = mTaskQueue.begin(); iter != mTaskQueue.end(); ++iter) {
            if (iter->executeTime <= sf::getCurrentTime()) {
                mTempScheduleQueue.push_back(*iter);
                iter = mTaskQueue.erase(iter);
                did = true;

                if (iter == mTaskQueue.end()) {
                    break;
                }
            } else {
                break;
            }
        }
    }

    if (did) {
        for (auto& item : mTempScheduleQueue) {
            item.task();
        }

        mTempScheduleQueue.clear();
    }

    return did;
}

void sf::Looper::waitForWork() {
}

bool sf::Looper::hasTask() const {
    return !mTaskQueue.empty() || !mIdleTask.empty();
}
