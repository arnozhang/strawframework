#include "graphics/app/DeclareHelper.h"

#include <locale>

#include "base/StrawUtils.h"
#include "graphics/app/CustomizeViewManager.h"


sf::declare::ViewDeclarer::ViewDeclarer(
    const char* viewName, ElementCreator creator) {

    std::string name(viewName);
    sf::trimNamespace(name);

    CustomizeViewManager::getInstance().addViewCreator(name, creator);
}

sf::declare::EnumDeclarer::EnumDeclarer(const char* enumName, int enumValue) {
    std::string name(enumName);
    sf::trimNamespace(name);
    if (name.empty()) {
        return;
    }

    name[0] = ::tolower(name[0]);
    CustomizeViewManager::getInstance().addEnum(name, enumValue);
}
