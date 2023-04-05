/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <thread>

#include "graphics/app/BaseApplication.h"


namespace sf {

    class STRAW_FRAMEWORK_API PluginApplication;


    class STRAW_FRAMEWORK_API StrawApplication : public BaseApplication {

    public:
        StrawApplication();
        virtual ~StrawApplication();

        static StrawApplication& getInstance();
        static const std::thread::id& getMainThreadId();
        virtual Context& getPlatformContext() const override;

        virtual void initialize(const char* cmdLine = nullptr) override;
        virtual void clean() override;
        virtual void run();
        virtual void terminate();
        virtual bool isTerminated() const;

        virtual void addPlugin(PluginApplication* plugin);
        virtual PluginApplication* getPlugin(const std::string& pluginName);

        virtual std::string getPluginRootDirectory() const;
        virtual std::string getPluginDirectory(const std::string& pluginName) const;
        virtual std::string getPluginBinDirectory(const std::string& pluginName) const;
        virtual std::string getPluginLibraryFilePath(const std::string& pluginName) const;


    protected:
        virtual void initSerializable();
        virtual void createPlatformContext();
        virtual void loadPlugins();
        virtual void loadPlugin(
            const std::string& pluginName,
            const std::string& pluginLibrary);

    private:
        Context* mPlatformContext;
        std::map<std::string, PluginApplication*> mPluginList;
    };
}
