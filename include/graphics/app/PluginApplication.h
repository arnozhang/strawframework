/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/app/BaseApplication.h"
#include "graphics/app/StrawApplication.h"


#define DECLARE_PLUGIN(pluginClazz, pluginName) \
public: \
    static const char* getPluginName() { \
        return pluginName; \
    } \
    virtual const char* getPluginNameInternal() const override { \
        return pluginClazz::getPluginName(); \
    } \
    static pluginClazz& getInstance() { \
        return reinterpret_cast<pluginClazz&>( \
            *sf::StrawApplication::getInstance().getPlugin(\
                pluginClazz::getPluginName())); \
    } \


namespace sf {

    class STRAW_FRAMEWORK_API PluginApplication : public BaseApplication {

    public:
        PluginApplication();
        virtual ~PluginApplication();

        virtual void initialize(const char* cmdLine = nullptr) override;

        virtual std::string getAppName() const override;
        virtual const std::string& getHomeDirectory() const override;
        virtual const char* getPluginNameInternal() const;
        virtual const std::string getPluginLibraryFilePath() const;

        virtual void loadExtensionConfig();

    private:
        std::string mPluginHomeDirectory;
    };
}
