#include <vector>
#include <iostream>
#include <Windows.h>

#include "ResNodeInfoCollector.h"
#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"


void collectResNode(
    ResNodeInfoCollector& collector,
    const std::string& rootPath,
    const std::string& resPath,
    const std::string& rdbFile) {

    auto result = collector.startCollectResNodeInfo(rootPath, resPath, rdbFile);
    std::cout << "  [ " << (result ? "SUCCESS" : "**FAILED**") << " ]\n" << std::endl;
}

void enumPluginDirectory(
    ResNodeInfoCollector& collector,
    const std::string& pluginRootPath) {

    const std::string search = pluginRootPath + "*.*";

    WIN32_FIND_DATA findData;
    HANDLE hFindNode = ::FindFirstFile(search.c_str(), &findData);
    if (hFindNode == INVALID_HANDLE_VALUE) {
        return;
    }

    std::cout << "\nPrepare Packing plugins resources.rdb.\n" << std::endl;
    std::string nodeName;
    while (::FindNextFile(hFindNode, &findData)) {
        nodeName = findData.cFileName;
        if (nodeName == "..") {
            continue;
        }

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // plugins/plugin_a
            std::string pluginPath = pluginRootPath + nodeName
                + sf::DIRECTORY_SEPARATOR;
            std::string resourcePath = pluginPath
                + sf::config::APPLICATION_RESOURCES
                + sf::DIRECTORY_SEPARATOR;

            std::cout << "  -- Packing " + nodeName + "/resources.rdb...";
            collectResNode(collector, pluginPath,
                resourcePath, sf::config::APPLICATION_RESOURCES);
        }
    }

    std::cout << "\n" << std::endl;
    ::FindClose(hFindNode);
}

int main(int argc, char** argv) {
    std::string resRootPath = sf::path::getCurrentPath();
    sf::path::combineRelativePath("..", resRootPath);

    ResNodeInfoCollector collector;

    // platform_resources.rdb
    std::cout << "Packing platform_resources.rdb...";
    std::string resPath = resRootPath
        + sf::config::PLATFORM_RESOURCES
        + sf::DIRECTORY_SEPARATOR;
    collectResNode(collector, resRootPath, resPath, sf::config::PLATFORM_RESOURCES);

    // resources.rdb
    std::cout << "Packing resources.rdb...";
    resPath = resRootPath
        + sf::config::APPLICATION_RESOURCES
        + sf::DIRECTORY_SEPARATOR;
    collectResNode(collector, resRootPath, resPath, sf::config::APPLICATION_RESOURCES);

    // plugins/
    std::string pluginRootPath = resRootPath
        + sf::config::PLUGIN_ROOT_DIRECTORY
        + sf::DIRECTORY_SEPARATOR;
    enumPluginDirectory(collector, pluginRootPath);

    system("pause");
    return 0;
}
