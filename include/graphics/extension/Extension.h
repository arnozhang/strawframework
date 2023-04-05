/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "graphics/app/DeclareHelper.h"


namespace sf {

    class STRAW_FRAMEWORK_API ViewGroup;
    class STRAW_FRAMEWORK_API BaseApplication;


    class STRAW_FRAMEWORK_API ExtensionHost {

    public:
        ExtensionHost(ViewGroup* group);
        virtual ~ExtensionHost();

        virtual void setExtensionName(const std::string& extensionName);

    protected:
        ViewGroup* mViewGroup;
    };


    class STRAW_FRAMEWORK_API ExtensionAgent {

    public:
        ExtensionAgent() {}
        virtual ~ExtensionAgent() {}

        virtual void getProperties(const std::string& extensionName) = 0;
        virtual void extension(const std::string& extensionName, ViewGroup* parent) = 0;


    protected:
        std::string mExtensionPointName;
    };


    using ExtensionAgentCreator = ExtensionAgent* (*)(BaseApplication*);


    namespace declare {

        class STRAW_FRAMEWORK_API ExtensionDeclarer {
        public:
            ExtensionDeclarer(
                const std::string& moduleName,
                const char* agentName,
                ExtensionAgentCreator creator);
        };
    }
}


#define DECLARE_EXTENSION_AGENT(moduleName, agentName) \
    namespace { \
        sf::declare::ExtensionDeclarer __LINE_UNIQUE_GENERATOR(__declare_extension_agent)( \
            moduleName, #agentName, [] (sf::BaseApplication* app) -> sf::ExtensionAgent* { \
                return new agentName(); \
            }); \
    } \

