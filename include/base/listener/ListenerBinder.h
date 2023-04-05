/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/28
 */

#pragma once

#include <map>


namespace sf {

    using MemFunctionKey = uint64_t;


    template <typename T, typename R, typename... Arg>
    union MemberFnCast {
        R (T::* mem_fn_ptr)(Arg...);
        MemFunctionKey void_ptr;
    };


    template <typename T>
    class ListenerBinder {
    public:
        typedef T this_class;

        static this_class& create() {
            return (new this_class())->setManaged();
        }

        template <typename R, typename... Arg, typename Fn>
        static this_class& create(R(this_class::* memFn)(Arg...), Fn& fn) {
            return create().bind(memFn, fn);
        }

        virtual ~ListenerBinder() {
            for (auto pair : mFunctionMap) {
                delete pair.second;
            }
        }

        virtual bool isManaged() const {
            return mIsManaged;
        }

        this_class& setManaged() {
            mIsManaged = true;
            return toThis();
        }

        this_class& setUnManaged() {
            mIsManaged = false;
            return toThis();
        }

        template <typename R, typename... Arg, typename Fn>
        this_class& bind(R(this_class::* memFn)(Arg...), Fn& fn) {
            typedef std::function<R(Arg...)> Function;
            Function* fn_ptr = new Function(fn);
            mFunctionMap[mem_ptr_cast(memFn)] = (void*) (fn_ptr);

            return toThis();
        }

        template <typename R, typename... Arg>
        this_class& unBind(R(this_class::* memFn)(Arg...)) {
            auto iter = mFunctionMap.find(mem_ptr_cast(memFn));
            if (iter != mFunctionMap.end()) {
                delete iter->second;
                mFunctionMap.erase(iter);
            }

            return toThis();
        }


    protected:
        ListenerBinder() {
            mIsManaged = false;
        }

        this_class& toThis() {
            return *reinterpret_cast<this_class*>(this);
        }

        template <typename R, typename... Arg>
        R invoke(R(this_class::* memFn)(Arg...), Arg... args) {
            MemFunctionKey key = mem_ptr_cast(memFn);
            auto iter = mFunctionMap.find(key);
            if (iter == mFunctionMap.end()) {
                return R();
            }

            void* fn_ptr = iter->second;
            if (!fn_ptr) {
                return R();
            }

            using Function = std::function<R(Arg...)>;
            Function* fn = reinterpret_cast<Function*>(fn_ptr);
            return (*fn)(args...);
        }

    private:
        template <typename R, typename... Arg>
        MemFunctionKey mem_ptr_cast(R(this_class::* memFn)(Arg...)) {
            MemberFnCast<this_class, R, Arg...> cast;
            static_assert(sizeof(cast.mem_fn_ptr) <= sizeof(MemFunctionKey),
                          "Listener Binder function cast may FAILED!");

            cast.mem_fn_ptr = memFn;
            return cast.void_ptr;
        }

    private:
        ListenerBinder(const ListenerBinder& rhs) = delete;
        ListenerBinder& operator=(const ListenerBinder& rhs) = delete;

        std::map<MemFunctionKey, void*> mFunctionMap;
        bool mIsManaged;
    };

}


#define INVOKE(mem_fn_name, ...) \
    return invoke(&this_class::mem_fn_name, __VA_ARGS__)

