/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"

namespace sf {

    namespace log {

        enum LogType {
            LOG_TYPE_INVALID = 0x00,
            LOG_TYPE_DEVELOP = 0x01,
            LOG_TYPE_FINAL = 0x10,
            LOG_TYPE_FINAL1 = 0x10,
            LOG_TYPE_FINAL2 = 0x30,
            LOG_TYPE_FINAL3 = 0x70
        };

        enum LogWriterType {
            LOG_WRITER_INVALID = 0,
            LOG_WRITER_VIEWER = 1,
            LOG_WRITER_STREAM = 2,
        };

        enum LogFieldFilter {
            LOG_DISABLE_ALL = 0x0000,
            LOG_ENABLE_TYPE = 0x0001,
            LOG_ENABLE_PROCESS_ID = 0x0002,
            LOG_ENABLE_THREAD_ID = 0x0004,
            LOG_ENABLE_LINE_NUMBER = 0x0008,
            LOG_ENABLE_LOG_TIME = 0x0010,
            LOG_ENABLE_LOG_FILTER = 0x0020,
            LOG_ENABLE_MODULE_NAME = 0x0040,
            LOG_ENABLE_FILE_NAME = 0x0080,
            LOG_ENABLE_FUNCTION_NAME = 0x0100,
            LOG_ENABLE_LOG_CONTENT = 0x2000,
            LOG_ENABLE_LOG_TICKCOUNT = 0x4000,
            LOG_ENABLE_ALL = 0x7FFF,
            LOG_ENABLE_SIMPLE_DESC = LOG_ENABLE_LOG_FILTER
                | LOG_ENABLE_LOG_CONTENT | LOG_ENABLE_FILE_NAME
                | LOG_ENABLE_FUNCTION_NAME | LOG_ENABLE_LINE_NUMBER,
        };


        STRAW_FRAMEWORK_API void strawLog(
            const int logLevel,
            const char* const fileName,
            const char* const functionName,
            const int lineNumber,
            const char* const filter,
            const char* const format,
            ...
            );
    }
}

#define LOG_TEXT


#if (defined _DEBUG) || (defined DEBUG)
    #define logDev(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_DEVELOP, LOG_TEXT(__FILE__), LOG_TEXT(__FUNCTION__), __LINE__, filter, format, __VA_ARGS__)
#else
    #define logDev(filter, format, ...)
#endif


#if (defined _DEBUG) || (defined DEBUG) || (!defined STRAW_FINAL_RELEASE_VERSION)
    #define logFinal1(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL1, LOG_TEXT(__FILE__), LOG_TEXT(__FUNCTION__), __LINE__, filter, format, __VA_ARGS__)

    #define logFinal2(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL2, LOG_TEXT(__FILE__), LOG_TEXT(__FUNCTION__), __LINE__, filter, format, __VA_ARGS__)

    #define logFinal3(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL3, LOG_TEXT(__FILE__), LOG_TEXT(__FUNCTION__), __LINE__, filter, format, __VA_ARGS__)
#else
    #define logFinal1(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL1, LOG_TEXT("file"), LOG_TEXT("func"), 0, filter, format, __VA_ARGS__)

    #define logFinal2(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL2, LOG_TEXT("file"), LOG_TEXT("func"), 0, filter, format, __VA_ARGS__)

    #define logFinal3(filter, format, ...)  \
        sf::log::strawLog(sf::log::LOG_TYPE_FINAL3, LOG_TEXT("file"), LOG_TEXT("func"), 0, filter, format, __VA_ARGS__)
#endif


#define logFinal(filter, format, ...) \
    logFinal1(filter, format, __VA_ARGS__)

