#include "graphics/extension/ExtensionManager.h"

#include "graphics/extension/Extension.h"


sf::ExtensionManager::ExtensionManager() {
}

sf::ExtensionManager::~ExtensionManager() {
}

void sf::ExtensionManager::onCreate() {
}

void sf::ExtensionManager::onTerminate() {
    for (auto agent : mAgentList) {
        delete agent.second;
    }

    mAgentList.clear();
    mAgentMap.clear();
    mAgentCreatorMap.clear();
}

void sf::ExtensionManager::registerExtensionAgent(
    const std::string& extensionName, const std::string& agentName) {

    ExtensionAgent* agent = nullptr;
    auto agentIter = mAgentList.find(agentName);
    if (agentIter != mAgentList.end()) {
        agent = agentIter->second;
    } else {
        auto iter = mAgentCreatorMap.find(agentName);
        if (iter == mAgentCreatorMap.end()) {
            return;
        }

        agent = iter->second(nullptr);
        mAgentList[agentName] = agent;
    }

    mAgentMap[extensionName].push_back(agent);
}

sf::ExtensionManager::ExtensionAgentList sf::ExtensionManager::getMatchedAgents(
    const std::string& extensionName) const {

    ExtensionAgentList list;
    auto iter = mAgentMap.find(extensionName);
    if (iter != mAgentMap.end()) {
        list = iter->second;
    }

    return list;
}

void sf::ExtensionManager::addExtensionAgentCreator(
    const std::string& agentName, ExtensionAgentCreator creator) {

    auto iter = mAgentCreatorMap.find(agentName);
    if (iter != mAgentCreatorMap.end()) {
        straw_warning("Extension-Agent Creator already exists: " + agentName);
        return;
    }

    mAgentCreatorMap[agentName] = creator;
}

