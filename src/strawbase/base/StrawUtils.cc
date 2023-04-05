#include "base/StrawUtils.h"

#ifdef STRAW_BUILD_FOR_WIN32
#include <Windows.h>
#undef min
#undef max
#endif

#include "base/loop/Looper.h"


namespace {

    const char* blank_characters = " \t\r\n";
    const wchar_t* w_blank_characters = L" \t\r\n";
    const int str_format_buffer = 1024;

} // anonymous namespace ends here.


std::string sf::wstring2string(const std::wstring& str) {
    const size_t size = str.size();
    const size_t count = size * 2;
    std::string result;

    result.reserve(count + 1);
    wcstombs(const_cast<char*>(result.c_str()), str.c_str(), count);
    result[count] = 0;

    return std::move(result);
}

std::wstring sf::string2wstring(const std::string& str) {
    const size_t size = str.size();
    std::wstring result;
    result.reserve(size + 1);

    mbstowcs(const_cast<wchar_t*>(result.c_str()), str.c_str(), size);
    result[size] = 0;

    return std::move(result);
}

std::string& sf::trim(std::string& val) {
    val.erase(0, val.find_first_not_of(blank_characters));
    val.erase(val.find_last_not_of(blank_characters) + 1);
    return val;
}

std::wstring& sf::trim(std::wstring& val) {
    val.erase(0, val.find_first_not_of(w_blank_characters));
    val.erase(val.find_last_not_of(w_blank_characters) + 1);
    return val;
}

sf::Size sf::getScreenSize() {
    Size size;

    #ifdef STRAW_BUILD_FOR_WIN32
    size.width = ::GetSystemMetrics(SM_CXSCREEN);
    size.height = ::GetSystemMetrics(SM_CYSCREEN);
    #elif defined(STRAW_BUILD_FOR_LINUX)
    #elif defined(STRAW_BUILD_FOR_MAC)
    #endif

    return size;
}

sf::Size sf::getWorkZoneScreenSize() {
    Size size;

    #ifdef STRAW_BUILD_FOR_WIN32
    size.width = ::GetSystemMetrics(SM_CXFULLSCREEN);
    size.height = ::GetSystemMetrics(SM_CYFULLSCREEN);
    #elif defined(STRAW_BUILD_FOR_LINUX)
    #elif defined(STRAW_BUILD_FOR_MAC)
    #endif

    return size;
}

sf::hash_value sf::BKDRHash(const std::string& str) {
    if (str.empty()) {
        return 0;
    }

    int seed = 131;
    hash_value hash = 0;

    const char* buffer = str.c_str();
    while (*buffer) {
        hash = hash * seed + (*buffer);
        ++buffer;
    }

    return (hash & 0x7FFFFFFF);
}

void sf::trimNamespace(std::string& name) {
    std::string::size_type pos = name.rfind(":");
    if (pos != -1) {
        name.erase(0, pos + 1);
    }
}

void sf::makeLower(std::wstring& wstr) {
    std::transform(wstr.begin(), wstr.end(), wstr.begin(), ::towlower);
}

void sf::makeLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void sf::format(std::wstring& wstr, const wchar_t* format, ...) {
    va_list ap;
    va_start(ap, format);

    wstr = sf::format(format, ap);

    va_end(ap);
}

std::string sf::format_ap(const char* format, va_list ap) {
    char buffer[str_format_buffer + 1] = {0};
    vsnprintf(
        buffer,
        str_format_buffer,
        format,
        ap);

    return std::string(buffer);
}

std::wstring sf::format_ap(const wchar_t* format, va_list ap) {
    return L"";
}

void sf::format(std::string& str, const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    str = sf::format(format, ap);

    va_end(ap);
}

std::string sf::format(const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    char buffer[str_format_buffer + 1] = {0};
    vsnprintf(
        buffer,
        str_format_buffer,
        format,
        ap);

    va_end(ap);

    return std::string(buffer);
}

std::wstring sf::format(const wchar_t* format, ...) {
    va_list ap;
    va_start(ap, format);

    wchar_t buffer[str_format_buffer + 1] = {0};
    vswprintf(
        buffer,
        str_format_buffer,
        format,
        ap);

    va_end(ap);

    return std::wstring(buffer);
}

uint64_t sf::getCurrentTime() {
    return (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool sf::isMainThread() {
    return std::this_thread::get_id() == Looper::getMainThreadId();
}
