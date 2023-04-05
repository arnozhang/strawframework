#include "graphics/app/CustomizeViewManager.h"

#include "base/StrawUtils.h"


namespace {

    sf::CustomizeViewManager* customizeViewManager = nullptr;


} // anonymous namespace ends here.


sf::CustomizeViewManager::CustomizeViewManager() {
}

sf::CustomizeViewManager::~CustomizeViewManager() {
}

sf::CustomizeViewManager& sf::CustomizeViewManager::getInstance() {
    if (!customizeViewManager) {
        customizeViewManager = new CustomizeViewManager();
    }

    return *customizeViewManager;
}

void sf::CustomizeViewManager::addViewCreator(
    const std::string& name, ElementCreator creator) {

    mViewCreatorMap[name] = creator;
}

sf::ElementCreator sf::CustomizeViewManager::getViewCreator(
    const std::string& name) const {

    auto iter = mViewCreatorMap.find(name);
    if (iter != mViewCreatorMap.end()) {
        return iter->second;
    }

    return nullptr;
}

void sf::CustomizeViewManager::addEnum(const std::string& name, int enumValue) {
    auto iter = mEnumValueMap.find(name);
    if (iter != mEnumValueMap.end()) {
        std::string warning = "\"" + name + "\" Enum already exist! can't add it to map!";
        straw_warning(warning.c_str());
        return;
    }

    mEnumValueMap[name] = enumValue;
}

int sf::CustomizeViewManager::getEnum(const std::string& name) const {
    auto iter = mEnumValueMap.find(name);
    if (iter != mEnumValueMap.end()) {
        return iter->second;
    }

    std::string warning = "Enum \"" + name + "\" not exist! Please Check your XML.";
    straw_warning(warning.c_str());

    return 0;
}
