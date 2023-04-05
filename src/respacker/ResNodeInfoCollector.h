#pragma once

#include <vector>
#include <string>
#include <memory>

#include "base/StrawUtils.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/context/ElementNode.h"


struct ResFileNodeInfo {

    ResFileNodeInfo()
        : resFileHash(0)
        , resFileSize(0)
        , resType(sf::ResourceType::Binary)
        , elementNode(nullptr) {
    }

    ResFileNodeInfo(const ResFileNodeInfo& rhs) {
        resFileHash = rhs.resFileHash;
        resType = rhs.resType;
        resSubPath = rhs.resSubPath;
        resPathIdentifier = rhs.resPathIdentifier;
        resFileSize = rhs.resFileSize;
        elementNode = rhs.elementNode;
    }

    sf::hash_value resFileHash;
    sf::ResourceType resType;
    std::string resSubPath;
    std::string resPathIdentifier;
    uint32_t resFileSize;
    sf::ElementNode* elementNode;
};


class ResNodeInfoCollector {

public:
    ResNodeInfoCollector();
    ~ResNodeInfoCollector();

    bool startCollectResNodeInfo(
        const std::string& destOutputPath,
        const std::string& resPath,
        const std::string& rdbFileName);


private:
    typedef std::vector<ResFileNodeInfo> ResFileNodeInfoList;

    void enumPathResFiles(const std::string& subPath);
    bool packResNodeInfo(const std::string& rdbFileName);
    sf::ElementNode* parseXmlElementNode(const std::string& resSubPath);
    void reset();

private:
    ResFileNodeInfoList mResFileNodeList;
    std::string mResRootPath;
    std::string mDestOutputPath;
};
