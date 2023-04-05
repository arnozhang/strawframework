#include "ResNodeInfoCollector.h"

#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <winioctl.h>

#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"


ResNodeInfoCollector::ResNodeInfoCollector() {
}

ResNodeInfoCollector::~ResNodeInfoCollector() {
}

void ResNodeInfoCollector::reset() {
    for (auto& node : mResFileNodeList) {
        delete node.elementNode;
    }

    mResFileNodeList.clear();
}

bool ResNodeInfoCollector::startCollectResNodeInfo(
    const std::string& destOutputPath,
    const std::string& resPath,
    const std::string& rdbFileName) {

    reset();
    mResRootPath = resPath;
    mDestOutputPath = destOutputPath;

    if (mResRootPath.empty()) {
        return false;
    }

    if (*mResRootPath.rbegin() != '\\') {
        mResRootPath += "\\";
    }

    enumPathResFiles("");

    return packResNodeInfo(rdbFileName);
}

void ResNodeInfoCollector::enumPathResFiles(const std::string& subPath) {
    std::string enumPath = mResRootPath + subPath;
    enumPath += "*.*";

    WIN32_FIND_DATA findData;
    HANDLE hFindNode = ::FindFirstFile(enumPath.c_str(), &findData);
    if (hFindNode == INVALID_HANDLE_VALUE) {
        return ;
    }

    std::string nodeName;
    ResFileNodeInfo node;
    const size_t ninePatchPngExtLength = strlen(sf::extension::NINE_PATCH_PNG);

    while (::FindNextFile(hFindNode, &findData)) {
        nodeName = findData.cFileName;
        if (nodeName == "..") {
            continue;
        }

        std::string newSubPath = subPath + nodeName;
        node.elementNode = nullptr;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            newSubPath += "/";
            enumPathResFiles(newSubPath);
        } else if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
            node.resPathIdentifier = newSubPath;
            sf::makeLower(node.resPathIdentifier);

            size_t pos = node.resPathIdentifier.rfind(sf::extension::NINE_PATCH_PNG);
            if (pos == node.resPathIdentifier.size() - ninePatchPngExtLength) {
                node.resType = sf::ResourceType::NinePatchImage;
                node.resPathIdentifier.erase(pos);
            } else {
                pos = node.resPathIdentifier.rfind('.');
                if (pos != -1) {
                    std::string extension = node.resPathIdentifier.substr(pos);
                    if (sf::isImageExtension(extension)) {
                        node.resType = sf::ResourceType::Image;
                    } else if (extension == sf::extension::XML) {
                        node.resType = sf::ResourceType::Xml;
                    } else if (extension == sf::extension::GIF) {
                        node.resType = sf::ResourceType::Gif;
                    } else if (extension == sf::extension::CURSOR) {
                        node.resType = sf::ResourceType::Cursor;
                    } else {
                        node.resType = sf::ResourceType::Binary;
                    }

                    node.resPathIdentifier.erase(pos);
                }
            }

            node.resSubPath = newSubPath;
            node.resFileHash = sf::BKDRHash(node.resPathIdentifier);
            if (node.resType == sf::ResourceType::Xml) {
                node.elementNode = parseXmlElementNode(node.resSubPath);
            }

            if (node.elementNode) {
                node.resType = sf::ResourceType::CompiledXml;
                node.resFileSize = node.elementNode->byteSize();
            } else {
                node.resFileSize = findData.nFileSizeLow;
            }

            mResFileNodeList.push_back(node);
        }
    }

    ::FindClose(hFindNode);
}

sf::ElementNode* ResNodeInfoCollector::parseXmlElementNode(
    const std::string& resSubPath) {

    std::string resFilePath = mResRootPath + resSubPath;
    sf::ref_ptr<sf::Buffer> content = sf::file::getFileContent(resFilePath);
    if (!content || content->empty()) {
        return nullptr;
    }

    tinyxml2::XMLDocument document;
    document.Parse(content->buffer(), content->size());
    auto root = document.RootElement();
    if (!root) {
        return nullptr;
    }

    sf::ElementNode* node = new sf::ElementNode();
    node->traversalNode(nullptr, root);
    return node;
}

bool ResNodeInfoCollector::packResNodeInfo(const std::string& rdbFileName) {
    std::string rdbFilePath = mDestOutputPath + rdbFileName + sf::config::RDB_FILE_EXTENSION;
    std::ofstream file;
    file.open(rdbFilePath, std::ios::out | std::ios::binary);
    if (!file) {
        return false;
    }

    // Write res file count.
    uint32_t resFileCount = mResFileNodeList.size();
    file.write(reinterpret_cast<char*>(&resFileCount), sizeof(uint32_t));

    // Write RDB head.
    const char SUB_PATH_END = 0;
    uint32_t currDataOffset = 0;
    for (auto& node : mResFileNodeList) {
        // Write hash value.
        file.write(reinterpret_cast<char*>(&node.resFileHash), sizeof(sf::hash_value));

        // Write res type.
        uint32_t resType = node.resType;
        file.write(reinterpret_cast<char*>(&resType), sizeof(uint32_t));

        // Write data offset by head-end-position.
        file.write(reinterpret_cast<char*>(&currDataOffset), sizeof(uint32_t));
        currDataOffset += node.resFileSize;

        // Write file size.
        file.write(reinterpret_cast<char*>(&node.resFileSize), sizeof(uint32_t));

        // Write file sub-path length.
        uint32_t dwFileSubPathLength = node.resPathIdentifier.size();
        uint32_t dwFileSubPathNeedSpace = dwFileSubPathLength + 1;
        file.write(reinterpret_cast<char*>(&dwFileSubPathNeedSpace), sizeof(uint32_t));

        // Write file sub-path and string end sign.
        file.write(node.resPathIdentifier.c_str(), sizeof(char) * dwFileSubPathLength);
        file.write(&SUB_PATH_END, sizeof(char));
    }

    // Write RDB file nodes data.
    bool writeResNodesDataFinish = true;
    for (auto& node : mResFileNodeList) {
        sf::ref_ptr<sf::Buffer> content = nullptr;
        if (node.elementNode) {
            content = node.elementNode->Serializable::serialize();
        } else {
            std::string resFilePath = mResRootPath + node.resSubPath;
            content = sf::file::getFileContent(resFilePath);
        }

        if (!content || content->size() != node.resFileSize) {
            writeResNodesDataFinish = false;
            break;
        }

        file.write(content->buffer(), node.resFileSize);
    }

    file.close();
    if (!writeResNodesDataFinish) {
        ::DeleteFile(rdbFilePath.c_str());
        return false;
    }

    return true;
}
