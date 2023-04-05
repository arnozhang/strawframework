/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/ptr/RefPtr.h"
#include "graphics/context/Context.h"


namespace sf {

    enum TargetPlatform {
        Unknown,
        Windows,
        Linux,
        Mac
    };

    class STRAW_FRAMEWORK_API BaseManager;


    class STRAW_FRAMEWORK_API BaseApplication {

    public:
        BaseApplication();
        virtual ~BaseApplication();

        virtual void initialize(const char* cmdLine = nullptr);
        virtual void clean();
        virtual Context& getContext() const;
        virtual Context& getAppContext() const;
        virtual Context& getPlatformContext() const;

        virtual std::string getAppName() const;
        virtual const std::string& getHomeDirectory() const;
        virtual const std::string getBinDirectory() const;
        virtual std::string getLanguageCode() const;
        TargetPlatform getTargetPlatform() const;

        BaseManager* getManager(const sf::string& name) const;
        void addManager(const sf::string& name, BaseManager* manager);

    protected:
        virtual Context* createContext();

    protected:
        typedef std::map<sf::string, BaseManager*> ManagerList;

        Context* mContext;
        ManagerList mManagers;
        std::string mHomeDirectory;
    };
}
