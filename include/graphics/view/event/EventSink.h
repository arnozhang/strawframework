/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <functional>

#include "base/def/PlatformDefine.h"
#include "base/ptr/weak_ptr.h"


namespace sf {


    template <typename P1, typename... P>
    class __Counter {
    public:
        static const int count = 1 + __Counter<P...>::count;
    };

    template <typename P1>
    class __Counter<P1> {
    public:
        static const int count = 1;
    };

    template <>
    class __Counter<void> {
    public:
        static const int count = 0;
    };


    template <typename Fn>
    class __FunctionExtract {
    public:
        using Function = std::function<void(void)>;
    };

    template <typename T, typename R, typename... P>
    class __FunctionExtract<R(T::*)(P...)> {
    public:
        using Function =std::function<R(P...)>;
    };

    template <typename T, typename Fn, int holder>
    class __SinkBinder {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 0> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 1> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 2> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 3> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 4> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 5> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 6> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 7> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7);
        }
    };

    template <typename T, typename Fn>
    class __SinkBinder<T, Fn, 8> {
    public:
        static typename __FunctionExtract<Fn>::Function bind(T* target, Fn fn) {
            return std::bind(fn, target, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7, std::placeholders::_8);
        }
    };


    template <typename Fn>
    class EventSink {
    };

    template <typename R, typename... P>
    class EventSink<R(P...)> {
    public:
        using Sink = std::function<R(P...)>;

        EventSink() {
            mBindRef = false;
        }

        EventSink(const EventSink& rhs) {
            *this = rhs;
        }

        EventSink& operator=(const EventSink& rhs) {
            mBindRef = rhs.mBindRef;
            mRefWeak = rhs.mRefWeak;
            mBindFn = rhs.mBindFn;
            return *this;
        }

        EventSink(R(* fn)(P...)) {
            bind(fn);
        }

        EventSink(const Sink& fn) {
            bind(fn);
        }

        R operator()(P... params) const {
            if (*this) {
                if (mBindRef) {
                    if (mRefWeak.get()) {
                        return mBindFn(params...);
                    }
                } else {
                    return mBindFn(params...);
                }
            }

            return R();
        }

        template <typename T>
        void bind(const sf::ref_ptr<T>& target, R(T::*memberFn)(P...)) {
            bind(target.get(), memberFn);
        }

        template <typename T>
        void bind(T* target, R(T::*memberFn)(P...)) {
            using Fn = R(T::*)(P...);

            mBindRef = true;
            mRefWeak = static_cast<RefPtrObject*>(target);
            mBindFn = __SinkBinder<T, Fn, __Counter<P...>::count>::bind(target, memberFn);
        }

        void bind(R(* fn)(P...)) {
            mBindFn = fn;
            mBindRef = false;
            mRefWeak.reset();
        }

        void bind(const Sink& fn) {
            mBindFn = fn;
            mBindRef = false;
            mRefWeak.reset();
        }

        operator bool() const {
            return static_cast<bool>(mBindFn);
        }


    private:
        bool mBindRef;
        sf::weak_ptr<RefPtrObject> mRefWeak;
        Sink mBindFn;
    };

}
