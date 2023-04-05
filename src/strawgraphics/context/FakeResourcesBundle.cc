#include "graphics/context/FakeResourcesBundle.h"

#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/context/ElementNode.h"


sf::FakeResourcesBundle::FakeResourcesBundle(
    const std::string& path, bool isPlatform) {

    mResPath = path
        + (isPlatform ? sf::config::PLATFORM_RESOURCES : sf::config::APPLICATION_RESOURCES)
        + sf::DIRECTORY_SEPARATOR;
}

sf::FakeResourcesBundle::~FakeResourcesBundle() {
}

sf::ElementNode* sf::FakeResourcesBundle::getXmlElementNode(
    const Context& context,
    const std::string& resPath) {

    ResourceType resType = ResourceType::Binary;
    ref_ptr<Buffer> content = getResBuffer(resPath, &resType);
    if (resType != ResourceType::Xml) {
        straw_warning("This file is not XML: " + resPath);
        return nullptr;
    }

    return parseXmlElementNode(context, content, resPath);
}

sf::ResourceType sf::FakeResourcesBundle::getResourceType(
    const std::string& resPath) {

    auto node = getResFullPath(resPath);
    return node ? node->resType : ResourceType::Binary;
}

sf::Buffer* sf::FakeResourcesBundle::getResBuffer(
    const std::string& resPath, ResourceType* resType /*= nullptr*/) {

    ResourceNode* resultNode = getResFullPath(resPath);
    if (!resultNode) {
        return nullptr;
    }

    if (resType) {
        *resType = resultNode->resType;
    }

    return sf::file::getFileContent(resultNode->fullPath);
}

bool sf::FakeResourcesBundle::hasResource(const std::string& resPath) const {
    ResourceNode* resultNode = nullptr;

    FakeResourcesBundle* ptr = const_cast<FakeResourcesBundle*>(this);
    return ptr->getResFullPath(resPath) != nullptr;
}

sf::FakeResourcesBundle::ResourceNode*
sf::FakeResourcesBundle::getResFullPath(const std::string& resPath) {

    ResourceNode* resultNode = nullptr;
    auto iter = mResFullPathMap.find(resPath);
    if (iter != mResFullPathMap.end()) {
        resultNode = &iter->second;
    } else {
        ResourceNode& node = mResFullPathMap[resPath];
        resultNode = &node;

        std::string path;
        if (sf::path::isPathOrFileExists(path)) {
            node.fullPath = path;
        } else {
            path = mResPath + resPath;

            while (true) {
                if (resolveResourceType(node, path,
                    sf::extension::XML, ResourceType::Xml)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::PNG, ResourceType::Image)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::NINE_PATCH_PNG, ResourceType::NinePatchImage)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::JPG, ResourceType::Image)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::BMP, ResourceType::Image)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::JPEG, ResourceType::Image)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::ICO, ResourceType::Image)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::GIF, ResourceType::Gif)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::CURSOR, ResourceType::Cursor)) {
                    break;
                }

                if (resolveResourceType(node, path,
                    sf::extension::WEBP, ResourceType::Image)) {
                    break;
                }

                break;
            }
        }
    }

    if (!resultNode || resultNode->fullPath.empty()) {
        return nullptr;
    }

    return resultNode;
}

bool sf::FakeResourcesBundle::resolveResourceType(
    ResourceNode& node,
    const std::string& path,
    const std::string& extension,
    ResourceType resType) {

    std::string fullPath = path + extension;
    if (sf::path::isPathOrFileExists(fullPath)) {
        node.fullPath = fullPath;
        node.resType = resType;
        return true;
    }

    return false;
}

std::vector<std::string> sf::FakeResourcesBundle::listFiles(
    const std::string& resPath) const {

    std::vector<std::string> list;
    list = sf::path::listFiles(mResPath + resPath + sf::DIRECTORY_SEPARATOR);
    for (int i = 0; i < list.size(); ++i) {
        int pos = list[i].rfind('.');
        if (pos != -1) {
            list[i].erase(pos);
        }

        list[i] = resPath + '/' + list[i];
    }

    return list;
}

sf::ElementNode* sf::FakeResourcesBundle::parseXmlElementNode(
    const Context& context, Buffer* content, const std::string& resPath ) {

    if (!content || content->empty()) {
        straw_warning("XML file is invalid or empty: " + resPath);
        return nullptr;
    }

    tinyxml2::XMLDocument document;
    document.Parse(content->buffer(), content->size());

    auto root = document.RootElement();
    if (!root) {
        straw_warning("Can not parse XML file: " + resPath);
        return nullptr;
    }

    ElementNode* xml = new ElementNode();
    xml->traversalNode(context, root);
    return xml;
}
