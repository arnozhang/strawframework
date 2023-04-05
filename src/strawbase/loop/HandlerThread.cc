#include "base/loop/HandlerThread.h"

#include "base/loop/Looper.h"


sf::HandlerThread::HandlerThread() {
}

sf::HandlerThread::~HandlerThread() {
    terminate();
}

void sf::HandlerThread::start() {
    sf::MutexLock lock(mLock);
    mThread.reset(new std::thread(&HandlerThread::threadProc, this));
    if (!mLooper) {
        mCondition.wait(lock);
    }

    mThread->detach();
}

void sf::HandlerThread::terminate() {
    mLooper->terminate();
}

sf::Looper& sf::HandlerThread::getLooper() {
    return *mLooper;
}

void sf::HandlerThread::threadProc() {
    {
        sf::MutexLock lock(mLock);
        mLooper = &Looper::myLooper();
        mCondition.notify_all();
    }

    mLooper->start();

    Looper::releaseLooper();
    mLooper = nullptr;
}
