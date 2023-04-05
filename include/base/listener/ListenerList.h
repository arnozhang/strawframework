/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/28
 */

#pragma once

#include <vector>


namespace sf {

    template<typename T>
    class ListenerList {
    public:
        ListenerList() {
        }

        virtual ~ListenerList() {
            clear();
        }

        void addListener(T& listener) {
            mListeners.push_back(&listener);
        }

        void addListener(T* listener) {
            mListeners.push_back(listener);
        }

        void removeListener(T& listener) {
            removeListener(&listener);
        }

        void removeListener(T* listener) {
            for (auto iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
                if (*iter == listener) {
                    mListeners.erase(iter);
                    break;
                }
            }
        }

        auto begin() {
            return mListeners.begin();
        }

        auto begin() const {
            return mListeners.begin();
        }

        auto end() {
            return mListeners.end();
        }

        auto end() const {
            return mListeners.end();
        }

        bool empty() const {
            return mListeners.empty();
        }

        void clear() {
            for (auto listener : mListeners) {
                if (listener->isManaged()) {
                    delete listener;
                }
            }

            mListeners.clear();
        }

    protected:
        std::vector<T*> mListeners;
    };
}
