/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <string>
#include <cstdarg>
#include <cassert>

#include "base/def/PlatformDefine.h"


namespace sf {

    typedef unsigned long hash_value;


    STRAW_FRAMEWORK_API std::string wstring2string(const std::wstring& str);
    STRAW_FRAMEWORK_API std::wstring string2wstring(const std::string& str);

    STRAW_FRAMEWORK_API std::string& trim(std::string& val);
    STRAW_FRAMEWORK_API std::wstring& trim(std::wstring& val);

    STRAW_FRAMEWORK_API void makeLower(std::wstring& wstr);
    STRAW_FRAMEWORK_API void makeLower(std::string& str);

    STRAW_FRAMEWORK_API void format(
        std::wstring& wstr, const wchar_t* format, ...);

    STRAW_FRAMEWORK_API void format(
        std::string& str, const char* format, ...);

    STRAW_FRAMEWORK_API std::wstring format(const wchar_t* format, ...);
    STRAW_FRAMEWORK_API std::wstring format_ap(const wchar_t* format, va_list ap);

    STRAW_FRAMEWORK_API std::string format(const char* format, ...);
    STRAW_FRAMEWORK_API std::string format_ap(const char* format, va_list ap);

    STRAW_FRAMEWORK_API Size getScreenSize();
    STRAW_FRAMEWORK_API Size getWorkZoneScreenSize();

    STRAW_FRAMEWORK_API hash_value BKDRHash(const std::string& str);

    STRAW_FRAMEWORK_API void trimNamespace(std::string& name);

    STRAW_FRAMEWORK_API uint64_t getCurrentTime();

    STRAW_FRAMEWORK_API bool isMainThread();

#ifdef UNICODE
#define toUtf8(str) sf::wstring2string(str)
#else
#define toUtf8(str) str
#endif // UNICODE

}



#ifdef STRAW_BUILD_FOR_WIN32
    #ifdef DEBUG
        #define straw_assert_warn(expression, warning) \
            (void)((!!(expression)) \
                || (_wassert(sf::string2wstring(warning).c_str(), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))
    #else
        #define straw_assert_warn(expression, warning) ((void) 0)
    #endif // DEBUG
#else
    #define straw_assert_warn(expression, warning) assert(expression)
#endif //STRAW_BUILD_FOR_WIN32

#ifdef DEBUG
    #define straw_assert(expression) straw_assert_warn(expression, #expression)
    #define straw_warning(warning) straw_assert_warn(false, warning)
#else
    #define straw_assert(expression) ((void) 0)
    #define straw_warning(warning) straw_assert(warning)
#endif


#ifdef DEBUG
    #define straw_check_thread(warning) \
        if (!sf::isMainThread()) { \
            straw_warning(warning); \
        }
#else
    #define straw_check_thread(warning) ((void) 0)
#endif

