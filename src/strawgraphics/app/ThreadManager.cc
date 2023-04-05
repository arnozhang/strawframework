#include "graphics/app/ThreadManager.h"


sf::ThreadManager::ThreadManager() {
}

sf::ThreadManager::~ThreadManager() {
}

void sf::ThreadManager::onCreate() {
}

void sf::ThreadManager::onTerminate() {
    if (mLogicHandler) {
        mLogicHandler->terminate();
        mLogicHandler.reset(nullptr);
    }
}

sf::Looper& sf::ThreadManager::mainLooper() {
    return Looper::mainLooper();
}

sf::Looper& sf::ThreadManager::logicLooper() {
    if (!mLogicHandler) {
        mLogicHandler.reset(new HandlerThread());
        mLogicHandler->start();
    }

    return mLogicHandler->getLooper();
}
