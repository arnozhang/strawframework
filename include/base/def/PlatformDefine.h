/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <string>
#include <cassert>

#include "base/def/FrameworkDef.h"


#ifdef STRAW_BUILD_FOR_WIN32
#include "base/def/PlatformDefine-win32.h"
#elif defined(STRAW_BUILD_FOR_LINUX)
#include "base/def/PlatformDefine-linux.h"
#elif defined(STRAW_BUILD_FOR_MAC)

#include "base/def/PlatformDefine-mac.h"

#endif // STRAW_BUILD_FOR_WIN32


namespace sf {

    typedef class STRAW_TEMPLATE_API PointTemplate<int> Point;
    typedef class STRAW_TEMPLATE_API PointTemplate<float> PointF;

    typedef class STRAW_TEMPLATE_API SizeTemplate<int> Size;
    typedef class STRAW_TEMPLATE_API SizeTemplate<float> SizeF;

    typedef class STRAW_TEMPLATE_API Directionx4Template<int> Directionx4;
    typedef class STRAW_TEMPLATE_API Directionx4Template<float> Directionx4F;

    typedef Directionx4 Rect;
    typedef Directionx4F RectF;


    typedef std::string utf8_string;


#ifdef UNICODE
    typedef wchar_t TCHAR;
    typedef std::wstring string;

    #ifndef _T
        #define _T(t) L##t
    #endif // _T

#else
    typedef char TCHAR;
    typedef std::string string;

    #ifndef _T
        #define _T(t) t
    #endif // _T

#endif // UNICODE


#ifdef _DEBUG
    #ifndef DEBUG
        #define DEBUG _DEBUG
    #endif // !DEBUG
#endif // _DEBUG

}

