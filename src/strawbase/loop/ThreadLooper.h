/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#include "base/loop/Looper.h"


namespace sf {

    namespace impl {

        class ThreadLooper : public Looper {

        public:
            ThreadLooper() {
            }

            virtual ~ThreadLooper() {
            }

        protected:
            virtual bool doWork() override {
                return false;
            }

            virtual void waitForWork() override {
                if (hasTask()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                } else {
                    sf::MutexLock lock(mLock);
                    mTaskCondition.wait(lock);
                }
            }
        };
    }
}
