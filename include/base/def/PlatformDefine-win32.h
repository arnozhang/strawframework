/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#ifdef STRAW_DYNAMIC

    #ifndef STRAW_FRAMEWORK_API
        #define STRAW_FRAMEWORK_API __declspec(dllimport)
        #define STRAW_TEMPLATE_API
    #else
        #undef STRAW_FRAMEWORK_API

        #define STRAW_FRAMEWORK_API __declspec(dllexport)
        #define STRAW_TEMPLATE_API STRAW_FRAMEWORK_API
        #pragma comment(lib, "skia_core.lib")
        #pragma comment(lib, "tinyxml2.lib")
    #endif // STRAW_FRAMEWORK_API

#else

    #undef STRAW_FRAMEWORK_API
    #define STRAW_FRAMEWORK_API
    #define STRAW_TEMPLATE_API

#endif // STRAW_DYNAMIC

#pragma warning(disable: 4251)


#ifdef _DEBUG

#ifndef DEBUG
#define DEBUG _DEBUG
#endif // !DEBUG

#endif // _DEBUG
