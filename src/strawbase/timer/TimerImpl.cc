#include "TimerImpl.h"

#include "base/loop/Looper.h"
#include "base/StrawUtils.h"

#include "../impls/win/WindowsTimerImpl.h"


sf::impl::TimerImpl* sf::impl::TimerImpl::mInstance = nullptr;


sf::impl::TimerImpl::TimerImpl() {
    mInitialized = false;
}

sf::impl::TimerImpl::~TimerImpl() {
}

sf::impl::TimerImpl& sf::impl::TimerImpl::getInstance() {
    if (!mInstance) {
#ifdef STRAW_BUILD_FOR_WIN32
        mInstance = new WindowsTimerImpl();
#elif defined(STRAW_BUILD_FOR_LINUX)
        mInstance = new LinuxTimerImpl();
#elif defined(STRAW_BUILD_FOR_MAC)
        //mInstance = new MacTimerImpl();
#endif // STRAW_BUILD_FOR_WIN32
    }

    return *mInstance;
}

void sf::impl::TimerImpl::initialize() {
    if (!mInitialized) {
        initializeInternal();
        mInitialized = true;
    }
}

void sf::impl::TimerImpl::unInitialize() {
    unInitializeInternal();

    for (auto& item : mTimerList) {
        killTimerInternal(item.second.timerId);
    }

    mTimerList.clear();
}

void sf::impl::TimerImpl::onTimer(timer::TimerId timerId) {
    auto iter = mTimerList.find(timerId);
    if (iter == mTimerList.end()) {
        return;
    }

    TimerItem& item = iter->second;
    auto callback = item.callback;
    if (!item.loopTimer) {
        killTimerInternal(timerId);
        mTimerList.erase(iter);
    }

    Looper::mainLooper().postTask(callback);
}

void sf::impl::TimerImpl::setTimerCallback(
    timer::TimerId timerId,
    int elapseMillSeconds,
    timer::TimerCallback callback,
    bool loopTimer /*= true*/) {

    straw_assert_warn(mTimerList.find(timerId) == mTimerList.end(),
                      sf::format("Already has this Timer: %d!", timerId));

    TimerItem& item = mTimerList[timerId];
    item.timerId = timerId;
    item.elapseMillSeconds = elapseMillSeconds;
    item.callback = callback;
    item.loopTimer = loopTimer;

    setTimerInternal(item);
}

void sf::impl::TimerImpl::killTimerCallback(timer::TimerId timerId) {
    killTimerInternal(timerId);
    mTimerList.erase(timerId);
}
