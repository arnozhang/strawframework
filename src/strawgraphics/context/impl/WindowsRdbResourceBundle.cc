#include "WindowsRdbResourceBundle.h"

#include <algorithm>

#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"


namespace {

    const char* const STRAW_RDB_MAPPING = "__straw_rdb_map_%d_%d";


    void getResRealPath(const std::string& resPath, std::string& resRealPath) {
        if (resPath.empty()) {
            resRealPath.clear();
            return;
        }

        resRealPath.assign(resPath.begin(), resPath.end());
        std::transform(
            resRealPath.begin(),
            resRealPath.end(),
            resRealPath.begin(),
            ::tolower);
    }

}


sf::impl::WindowsRdbResourceBundle::WindowsRdbResourceBundle(
    const std::string& path, bool isPlatform) {

    mFileMappingHandle = INVALID_HANDLE_VALUE;
    mRdbFileBuffer = nullptr;
    mRealResFileBuffer = nullptr;

    std::string rdbFilePath = path
        + (isPlatform ? sf::config::PLATFORM_RESOURCES : sf::config::APPLICATION_RESOURCES)
        + sf::config::RDB_FILE_EXTENSION;

    loadRdbFile(rdbFilePath);
}

sf::impl::WindowsRdbResourceBundle::~WindowsRdbResourceBundle() {
    if (mRdbFileBuffer) {
        ::UnmapViewOfFile(mRdbFileBuffer);
    }

    if (mFileMappingHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(mFileMappingHandle);
    }
}

sf::ElementNode* sf::impl::WindowsRdbResourceBundle::getXmlElementNode(
    const Context& context,
    const std::string& resPath) {
    
    return nullptr;
}

sf::ResourceType sf::impl::WindowsRdbResourceBundle::getResourceType(
    const std::string& resPath) {

    auto node = findResourceNode(resPath);
    if (node != mResNodeInfoList.end()) {
        return node->second.mResType;
    }

    return ResourceType::Binary;
}

bool sf::impl::WindowsRdbResourceBundle::loadRdbFile(
    const std::string& rdbFilePath) {

    if (!sf::path::isPathOrFileExists(rdbFilePath)) {
        straw_warning("Rdb file not exsits: " + rdbFilePath);
        return false;
    }

    HANDLE rdbFileHandle = ::CreateFileA(
        rdbFilePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        0,
        0);

    if (rdbFileHandle == INVALID_HANDLE_VALUE) {
        straw_warning("Open rdb file FAILED: " + rdbFilePath);
        return false;
    }

    DWORD highPartSize = 0;
    DWORD lowPartSize = ::GetFileSize(rdbFileHandle, &highPartSize);

    std::string rdbMappingName = sf::format(
        STRAW_RDB_MAPPING, ::GetCurrentThreadId(), reinterpret_cast<int>(this));
    mFileMappingHandle = ::CreateFileMappingA(
        rdbFileHandle, NULL, PAGE_READONLY,
        highPartSize, lowPartSize, rdbMappingName.c_str());
    if (mFileMappingHandle == INVALID_HANDLE_VALUE || !mFileMappingHandle) {
        straw_warning("Create rdb file Mapping FAILED: " + rdbFilePath);
        ::CloseHandle(rdbFileHandle);
        return false;
    }

    mRdbFileBuffer = ::MapViewOfFile(mFileMappingHandle, FILE_MAP_READ, 0, 0, 0);
    ::CloseHandle(rdbFileHandle);

    prepareResources();

    return true;
}

sf::impl::WindowsRdbResourceBundle::res_node_iter
sf::impl::WindowsRdbResourceBundle::findResourceNode(
    const std::string& resPath) const {

    std::string resRealPath;
    getResRealPath(resPath, resRealPath);
    sf::hash_value findHash = sf::BKDRHash(resRealPath);

    const res_node_iter endIter = mResNodeInfoList.end();
    res_node_iter lowIter = mResNodeInfoList.lower_bound(findHash);
    res_node_iter highIter = mResNodeInfoList.upper_bound(findHash);
    res_node_iter findIter = endIter;

    if (lowIter == endIter) {
        return endIter;
    } else {
        while (lowIter != highIter) {
            if (resRealPath == lowIter->second.mResPath) {
                findIter = lowIter;
                break;
            }

            ++lowIter;
        }
    }

    return findIter;
}

sf::Buffer* sf::impl::WindowsRdbResourceBundle::getResBuffer(
    const std::string& resPath, ResourceType* resType /*= nullptr*/) {

    if (!mRealResFileBuffer) {
        return nullptr;
    }

    sf::Buffer* buffer = getRDBFileBuffer(resPath, resType);
    straw_assert_warn(buffer, "Get rdb buffer FAILED: " + resPath);
    return buffer;
}

bool sf::impl::WindowsRdbResourceBundle::hasResource(
    const std::string& resPath) const {

    return findResourceNode(resPath) != mResNodeInfoList.end();
}

bool sf::impl::WindowsRdbResourceBundle::prepareResources() {
    if (!mRdbFileBuffer) {
        return false;
    }

    uint32_t resCount = *((uint32_t*) mRdbFileBuffer);
    char* buffer = (char*) mRdbFileBuffer + sizeof(uint32_t);

    ResNodeInfo node;
    for (int i = 0; i < resCount; ++i) {
        node.mPathHash = *((sf::hash_value*) buffer);
        buffer += sizeof(sf::hash_value);

        node.mResType = static_cast<ResourceType>(*((uint32_t*) buffer));
        buffer += sizeof(uint32_t);

        node.mResOffsetBytes = *((uint32_t*) buffer);
        buffer += sizeof(uint32_t);

        node.mResFileSize = *((uint32_t*) buffer);
        buffer += sizeof(uint32_t);

        uint32_t resPathLength = *((uint32_t*) buffer);
        resPathLength -= 1;
        buffer += sizeof(uint32_t);

        node.mResPath.assign(buffer, resPathLength);
        buffer += (sizeof(char) * resPathLength);
        ++buffer;    // skip '\0'

        mResNodeInfoList.insert(std::make_pair(node.mPathHash, node));
    }

    mRealResFileBuffer = buffer;

    return true;
}

sf::Buffer* sf::impl::WindowsRdbResourceBundle::getRDBFileBuffer(
    const std::string& resPath, ResourceType* resType /*= nullptr*/) {

    res_node_iter iter = findResourceNode(resPath);
    if (iter != mResNodeInfoList.end()) {
        sf::Buffer* buffer = new Buffer;
        buffer->assign(
            (char*) mRealResFileBuffer + iter->second.mResOffsetBytes,
            iter->second.mResFileSize);
        buffer->setReleaseWhenClear(false);

        if (resType) {
            *resType = iter->second.mResType;
        }

        return buffer;
    }

    return nullptr;
}

std::vector<std::string> sf::impl::WindowsRdbResourceBundle::listFiles(
    const std::string& resPath) const {

    std::vector<std::string> list;
    for (auto& node : mResNodeInfoList) {
        if (node.second.mResPath.find(resPath) == 0) {
            list.push_back(node.second.mResPath);
        }
    }

    return list;
}
