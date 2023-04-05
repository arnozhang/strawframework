#include <iostream>
#include <fstream>
#include <vector>

#include "base/ptr/RefPtr.h"
#include "base/data/Buffer.h"
#include "base/data/Array.h"
#include "base/file/FileUtils.h"
#include "base/StrawUtils.h"
#include "base/data/NumericSerializable.h"
#include "graphics/GraphicsDeclare.h"


int main(int argc, char** argv) {
    std::string resRootPath = sf::path::getCurrentPath();
    sf::path::combineRelativePath("..", resRootPath);

    std::string pluginPath = resRootPath
                             + sf::config::PLUGIN_ROOT_DIRECTORY
                             + sf::DIRECTORY_SEPARATOR
                             + sf::config::PLUGIN_CONFIG_FILE;

    std::string srcPth = pluginPath;
    size_t pos = srcPth.rfind('.');
    if (pos != -1) {
        srcPth.erase(pos);
    }

    srcPth += ".txt";
    std::string content = sf::file::getFileStringContent(srcPth);
    if (content.empty()) {
        return 0;
    }

    sf::Array plugins;
    while (true) {
        pos = content.find("\r\n");
        if (pos == -1) {
            break;
        }

        std::string name = content.substr(0, pos);
        sf::trim(name);
        if (!name.empty()) {
            plugins.push_back(new sf::String(name));
        }

        content.erase(0, pos + 2);
    }

    sf::ref_ptr<sf::Buffer> buffer = plugins.Serializable::serialize();

    std::ofstream config;
    config.open(pluginPath.c_str(), std::ios::out | std::ios::binary);

    if (buffer && !buffer->empty()) {
        config.write(buffer->buffer(), buffer->size());
    }

    config.close();

    system("pause");
    return 0;
}
