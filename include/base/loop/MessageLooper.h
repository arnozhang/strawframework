/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/24
 */

#pragma once

#include "base/loop/Looper.h"


namespace sf {

    class STRAW_FRAMEWORK_API MessageLooper : public Looper {

    public:
        MessageLooper();
        virtual ~MessageLooper();

        virtual void terminate() override;

    protected:
        virtual bool doWork() override;

    private:
        bool loopForWindows();
    };
}
