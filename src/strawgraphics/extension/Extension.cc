#include "graphics/extension/Extension.h"

#include "base/StrawUtils.h"
#include "graphics/extension/ExtensionManager.h"


sf::declare::ExtensionDeclarer::ExtensionDeclarer(
    const std::string& moduleName,
    const char* agentName,
    ExtensionAgentCreator creator) {

    std::string name(agentName);
    sf::trimNamespace(name);
    if (name.empty()) {
        return;
    }

    if (!moduleName.empty()) {
        name = moduleName + "." + name;
    }

    ExtensionManager::getInstance().addExtensionAgentCreator(name, creator);
}

sf::ExtensionHost::ExtensionHost(ViewGroup* group) {
    mViewGroup = group;
}

sf::ExtensionHost::~ExtensionHost() {
}

void sf::ExtensionHost::setExtensionName(const std::string& extensionName) {
    auto agents = ExtensionManager::getInstance().getMatchedAgents(extensionName);
    for (auto agent : agents) {
        agent->extension(extensionName, mViewGroup);
    }
}
