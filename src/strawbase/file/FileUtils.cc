#include "base/file/FileUtils.h"

#include <fstream>
#include <Windows.h>
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace sf {
    namespace impl {
        std::string g_currentPath;
    }
}


sf::Buffer* sf::file::getFileContent(const std::string& file) {
    std::ifstream stream;
    stream.open(file.c_str(), std::ios::in | std::ios::binary);
    if (!stream) {
        stream.close();
        return nullptr;
    }

    std::istream::pos_type start = stream.tellg();
    stream.seekg(0, std::ios_base::end);
    std::istream::pos_type end = stream.tellg();
    stream.seekg(start);

    auto length = end - start;
    if (length <= 0) {
        stream.close();
        return new Buffer();
    }

    char* buffer = new char[static_cast<unsigned int>(length)];
    while (!stream.eof()) {
        stream.read(buffer, length);
    }

    stream.close();
    return new Buffer(buffer, static_cast<int>(length));
}

std::string sf::file::getFileStringContent(const std::string& file) {
    ref_ptr<Buffer> buffer = sf::file::getFileContent(file);
    if (!buffer || buffer->empty()) {
        return "";
    }

    return std::string(buffer->buffer(), buffer->size());
}


void sf::path::setCurrentPath(const std::string& currPath) {
    sf::impl::g_currentPath = currPath;
}

const std::string& sf::path::getCurrentPath() {
    if (sf::impl::g_currentPath.empty()) {
        char xszBuffer[MAX_PATH + 1] = {0};
        ::GetModuleFileName(NULL, xszBuffer, MAX_PATH);

        sf::impl::g_currentPath = xszBuffer;
        std::string::size_type pos = sf::impl::g_currentPath.rfind('\\');
        if (pos != std::string::npos) {
            sf::impl::g_currentPath.erase(pos + 1);
        }

        sf::path::combineRelativePath(sf::impl::g_currentPath, sf::impl::g_currentPath);
    }

    return sf::impl::g_currentPath;
}

bool sf::path::isPathAbsolutely(const std::string& directory) {
    return std::string::npos != directory.find(":\\");
}

bool sf::path::combineRelativeFilePath(
    const std::string& relativeFilePath,
    std::string& resultFilePath) {

    const std::string& strCurrentPath(getCurrentPath());
    char xszBuffer[MAX_PATH + 1] = {0};
    memcpy(
        xszBuffer,
        strCurrentPath.c_str(),
        strCurrentPath.size() * sizeof(char));

    resultFilePath.clear();
    if (::PathAppend(xszBuffer, relativeFilePath.c_str())) {
        resultFilePath = xszBuffer;
        return true;
    }

    return false;
}

bool sf::path::combineRelativePath(
    const std::string& relativePath,
    std::string& resultPath) {

    if (combineRelativeFilePath(relativePath, resultPath)) {
        if (*resultPath.rbegin() != '\\') {
            resultPath += "\\";
        }

        return true;
    }

    return false;
}

bool sf::path::isPathOrFileExists(const std::string& directory) {
    return ::PathFileExists(directory.c_str());
}

std::vector<std::string> sf::path::listFiles(
    const std::string directory, bool containSubPath /*= false*/) {

    std::vector<std::string> list;
    const std::string search = directory + "*.*";

    WIN32_FIND_DATA findData;
    HANDLE hFindNode = ::FindFirstFile(search.c_str(), &findData);
    if (hFindNode == INVALID_HANDLE_VALUE) {
        return list;
    }

    std::string name;
    while (::FindNextFile(hFindNode, &findData)) {
        name = findData.cFileName;
        if (name == "..") {
            continue;
        }

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE
            || (containSubPath && findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            list.push_back(name);
        }
    }

    ::FindClose(hFindNode);
    return list;
}
