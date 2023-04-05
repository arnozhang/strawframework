/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "graphics/app/StrawApplication.h"


namespace sf {

    class STRAW_FRAMEWORK_API BaseManager {

    public:
        virtual ~BaseManager() {}

        virtual void onCreate() = 0;
        virtual void onTerminate() = 0;
    };
}


#define DECLARE_MANAGER(manager) \
    public: \
        static const TCHAR* getManagerName() { \
            return _T(#manager); \
        } \
        static manager* create() { \
            return new manager(); \
        } \
        static manager& getInstance() { \
            const TCHAR* name = manager::getManagerName(); \
            StrawApplication& app = StrawApplication::getInstance(); \
            BaseManager* mgr = app.getManager(name); \
            if (!mgr) { \
                mgr = create(); \
                mgr->onCreate(); \
                app.addManager(name, mgr); \
            } \
            return *(reinterpret_cast<manager*>(mgr)); \
        } \
    private: \
        manager(const manager& rhs) = delete; \
        manager& operator= (const manager& rhs) = delete; \


