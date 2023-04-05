/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/ptr/RefPtrObject.h"


namespace sf {

    class STRAW_FRAMEWORK_API Buffer : public RefCountObject {

    public:
        Buffer();
        Buffer(char* buffer, size_t size);
        virtual ~Buffer();

        void assign(char* buffer, size_t size);
        char* buffer() const;
        size_t size() const;
        bool empty() const;
        void clear();
        void setReleaseWhenClear(bool release);

    private:
        char* mBuffer;
        size_t mSize;
        bool mReleaseWhenClear;
    };
}
