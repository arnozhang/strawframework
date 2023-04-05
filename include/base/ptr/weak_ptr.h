/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/ptr/RefPtrObject.h"
#include "base/ptr/RefPtr.h"


namespace sf {

    template <typename T>
    class STRAW_TEMPLATE_API weak_ptr {

    public:
        weak_ptr(T* _ptr = 0) {
            *this = _ptr;
        }

        template <typename U>
        weak_ptr(const ref_ptr<U>& count) : weak_ptr(count.get()) {
        }

        virtual ~weak_ptr() {
        }

        weak_ptr& operator=(const weak_ptr& rhs) {
            return *this = rhs.get();
        }

        weak_ptr& operator=(T* _ptr) {
            weak = nullptr;
            ptr = _ptr;

            if (ptr) {
                ptr->attachWeak(&weak);
            }

            return *this;
        }


    public:
        T* get() const {
            if (weak && weak->valid()) {
                return ptr;
            }

            return nullptr;
        }

        weak_ptr& reset() {
            weak = nullptr;
            ptr = nullptr;
            return *this;
        }


    private:
        T* ptr;
        ref_ptr<WeakSupport> weak;
    };

}
