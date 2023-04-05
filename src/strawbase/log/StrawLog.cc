#include "base/log/StrawLog.h"

#include <Windows.h>

#include "base/StrawUtils.h"


void sf::log::strawLog(
    const int logLevel,
    const char* const fileName,
    const char* const functionName,
    const int lineNumber,
    const char* const filter,
    const char* const format,
    ...) {

    va_list ap;
    va_start(ap, format);
    std::string content = sf::format_ap(format, ap);
    va_end(ap);

    std::string log = sf::format("%d/%s [%s: %d]> %s: %s\n",
        logLevel, filter, fileName, lineNumber, functionName,
        content.c_str());

    ::OutputDebugString(log.c_str());
}
