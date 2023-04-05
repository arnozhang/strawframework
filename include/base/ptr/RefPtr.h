/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/StrawUtils.h"


namespace sf {


    template<typename T>
    class STRAW_TEMPLATE_API ref_ptr {

    public:
        ~ref_ptr() {
            reset();
        }

        ref_ptr(T* _ptr = nullptr) : ptr(_ptr) {
            if (ptr) {
                ptr->addRef();
            }
        }

        explicit ref_ptr(const ref_ptr& count) : ptr(count.ptr) {
            if (ptr) {
                ptr->addRef();
            }
        }

        template<typename U>
        explicit ref_ptr(const ref_ptr<U>& count) : ptr(count.get()) {
            if (ptr) {
                ptr->addRef();
            }
        }

        ref_ptr& operator= (T* _ptr) {
            if (_ptr) {
                _ptr->addRef();
            }

            if (ptr) {
                ptr->release();
            }

            ptr = _ptr;
            return *this;
        }

        ref_ptr& operator= (const ref_ptr& count) {
            return *this = count.ptr;
        }

        template<typename U>
        ref_ptr& operator= (const ref_ptr<U>& count) {
            return *this = count.get();
        }

        operator bool() const {
            return ptr != nullptr;
        }

        operator T*() const {
            return ptr;
        }

    public:
        template<typename U>
        void copyTo(U** __ptr) {
            if (ptr) {
                ptr->addRef();
            }

            *__ptr = ptr;
        }

        void copyTo(T** __ptr) {
            if (ptr) {
                ptr->addRef();
            }

            *__ptr = ptr;
        }

        void reset() {
            *this = nullptr;
        }

        void swap(T** _ptr) {
            T* tmp = *_ptr;
            *_ptr = ptr;
            ptr = tmp;
        }

        void swap(ref_ptr& count) {
            swap(&count);
        }

        T* returnGet() const {
            if (ptr) {
                ptr->addRef();
            }

            return get();
        }

        T* get() const {
            return ptr;
        }

        T* operator-> () const {
            straw_assert(ptr && "Oops! ptr must **NOT** be nullptr!");
            return ptr;
        }

        T** operator& () {
            straw_assert(!ptr && "Oops! ptr must be nullptr!");
            return &ptr;
        }

        T& operator* () const {
            return *ptr;
        }

    protected:
        T* ptr;
    };
}
