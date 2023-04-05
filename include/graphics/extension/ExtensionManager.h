/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/app/BaseManager.h"
#include "graphics/extension/Extension.h"


namespace sf {

    class STRAW_FRAMEWORK_API ExtensionAgent;


    class STRAW_FRAMEWORK_API ExtensionManager : public BaseManager {
        DECLARE_MANAGER(ExtensionManager);

    public:
        ExtensionManager();
        virtual ~ExtensionManager();

        virtual void onCreate() override;
        virtual void onTerminate() override;

        typedef std::vector<ExtensionAgent*> ExtensionAgentList;
        typedef std::map<std::string, ExtensionAgentList> ExtensionAgentMap;

        void registerExtensionAgent(
            const std::string& extensionName,
            const std::string& agentName);

        ExtensionAgentList getMatchedAgents(
            const std::string& extensionName) const;

        void addExtensionAgentCreator(
            const std::string& agentName,
            ExtensionAgentCreator creator);

    private:
        ExtensionAgentMap mAgentMap;
        std::map<std::string, ExtensionAgent*> mAgentList;
        std::map<std::string, ExtensionAgentCreator> mAgentCreatorMap;
    };
}
