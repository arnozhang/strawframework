#include "base/timer/Timer.h"

#include "TimerImpl.h"


void sf::timer::setTimerCallback(
    TimerId timerId,
    int elapseMillSeconds,
    TimerCallback callback,
    bool loopTimer /*= true*/) {

    sf::impl::TimerImpl::getInstance().setTimerCallback(
        timerId, elapseMillSeconds, callback, loopTimer);
}

void sf::timer::killTimerCallback(TimerId timerId) {
    sf::impl::TimerImpl::getInstance().killTimerCallback(timerId);
}

void sf::timer::startTimerSystem() {
    sf::impl::TimerImpl::getInstance().initialize();
}

void sf::timer::terminateTimerSystem() {
    sf::impl::TimerImpl::getInstance().unInitialize();
}
