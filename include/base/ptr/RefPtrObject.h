/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <atomic>

#include "base/ptr/RefPtr.h"


template<>
struct STRAW_FRAMEWORK_API std::atomic<int>;

typedef std::atomic<int> atomic_ref_integer;



namespace sf {


    class STRAW_FRAMEWORK_API Ref {

    public:
        Ref() {}
        virtual ~Ref() {}

        virtual int addRef() = 0;
        virtual int release() = 0;
    };


    class STRAW_FRAMEWORK_API RefCountObject : public Ref {

    public:
        RefCountObject(int count = 0) : refCount(count) {}
        virtual ~RefCountObject() {}

        virtual int addRef() override final {
            refCount.fetch_add(1);
            return refCount;
        }

        virtual int release() override final {
            refCount.fetch_sub(1);
            if (refCount == 0) {
                delete this;
                return 0;
            }

            return refCount;
        }

        virtual int count() const {
            return refCount;
        }

        virtual bool valid() const {
            return count() > 0;
        }

    private:
        atomic_ref_integer refCount;
    };


    class STRAW_FRAMEWORK_API WeakSupport : public RefCountObject {

    public:
        WeakSupport() : target(new RefCountObject()) {}
        ~WeakSupport() {
            delete target;
        }

        int addTargetRef() {
            if (target) {
                return target->addRef();
            }

            return 0;
        }

        int releaseTarget() {
            if (!target) {
                return 0;
            }

            int count = target->release();
            if (count == 0) {
                target = nullptr;
            }

            return count;
        }

        virtual int count() const {
            return target ? target->count() : 0;
        }

    private:
        RefCountObject* target;
    };


    class STRAW_FRAMEWORK_API RefPtrObject : public Ref {

    public:
        RefPtrObject() : weak(new WeakSupport()) {}
        virtual ~RefPtrObject() {}

        virtual int addRef() override final {
            return weak->addTargetRef();
        }

        virtual int release() override final {
            return weak->releaseTarget();
        }


    protected:
        template<typename T>
        friend class weak_ptr;

        void attachWeak(WeakSupport** _weak) {
            weak.copyTo(_weak);
        }

    private:
        ref_ptr<WeakSupport> weak;
    };

}
