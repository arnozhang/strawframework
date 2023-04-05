#include "graphics/context/RdbResourcesBundle.h"

#include "impl/WindowsRdbResourceBundle.h"
#include "base/data/Buffer.h"
#include "base/StrawUtils.h"
#include "graphics/context/ElementNode.h"
#include "graphics/context/FakeResourcesBundle.h"


sf::RdbResourcesBundle::RdbResourcesBundle(
    const std::string& path, bool isPlatform) {

#ifdef STRAW_BUILD_FOR_WIN32
    mBundleImpl = new sf::impl::WindowsRdbResourceBundle(path, isPlatform);
#endif // STRAW_BUILD_FOR_WIN32
}

sf::RdbResourcesBundle::~RdbResourcesBundle() {
    delete mBundleImpl;
}

sf::ElementNode* sf::RdbResourcesBundle::getXmlElementNode(
    const Context& context,
    const std::string& resPath) {

    ResourceType resType = ResourceType::Binary;
    ref_ptr<Buffer> content = getResBuffer(resPath, &resType);
    if (resType == ResourceType::CompiledXml) {
        if (!content || content->empty()) {
            straw_warning("XML file is invalid or empty: " + resPath);
            return nullptr;
        }

        ElementNode* xml = new ElementNode();
        xml->deserialize(context, content->buffer());
        return xml;
    } else if (resType == ResourceType::Xml) {
        return FakeResourcesBundle::parseXmlElementNode(context, content, resPath);
    } else {
        straw_warning("This file is not XML: " + resPath);
    }

    return nullptr;
}

sf::ResourceType sf::RdbResourcesBundle::getResourceType(
    const std::string& resPath) {
    return mBundleImpl->getResourceType(resPath);
}

sf::Buffer* sf::RdbResourcesBundle::getResBuffer(
    const std::string& resPath, ResourceType* resType /*= nullptr*/) {
    return mBundleImpl->getResBuffer(resPath, resType);
}

bool sf::RdbResourcesBundle::hasResource(const std::string& resPath) const {
    return mBundleImpl->hasResource(resPath);
}

std::vector<std::string> sf::RdbResourcesBundle::listFiles(
    const std::string& resPath) const {
    return mBundleImpl->listFiles(resPath);
}
