#include "graphics/app/BaseApplication.h"

#ifdef STRAW_BUILD_FOR_WIN32
#include <wtypes.h>
#include <winnls.h>
#endif

#include "base/StrawUtils.h"
#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/app/BaseManager.h"
#include "graphics/window/Window.h"


sf::BaseApplication::BaseApplication() {
    mHomeDirectory = sf::path::getCurrentPath();
    sf::path::combineRelativePath("..", mHomeDirectory);
}

sf::BaseApplication::~BaseApplication() {
    delete mContext;
}

void sf::BaseApplication::initialize(const char* cmdLine /*= nullptr*/) {
    mContext = createContext();
    mContext->initialize();
}

sf::Context* sf::BaseApplication::createContext() {
    return new Context(this);
}

void sf::BaseApplication::clean() {
    for (auto iter = mManagers.begin(); iter != mManagers.end(); ++iter) {
        iter->second->onTerminate();
        delete iter->second;
    }

    mManagers.clear();

    Window::onAppTerminate();
}

sf::Context& sf::BaseApplication::getContext() const {
    return *mContext;
}

sf::Context& sf::BaseApplication::getAppContext() const {
    return StrawApplication::getInstance().getContext();
}

sf::Context& sf::BaseApplication::getPlatformContext() const {
    return StrawApplication::getInstance().getPlatformContext();
}

std::string sf::BaseApplication::getAppName() const {
    return sf::config::APP_NAME;
}

const std::string& sf::BaseApplication::getHomeDirectory() const {
    return mHomeDirectory;
}

const std::string sf::BaseApplication::getBinDirectory() const {
    return getHomeDirectory() + sf::BIN_DIRECTORY_NAME + sf::DIRECTORY_SEPARATOR;
}

std::string sf::BaseApplication::getLanguageCode() const {
#ifdef STRAW_BUILD_FOR_WIN32
    LANGID lid = ::GetUserDefaultUILanguage();
    const LCID locale_id = MAKELCID(lid, SORT_DEFAULT);
    char code[3] = { 0 };
    ::GetLocaleInfoA(locale_id, LOCALE_SISO639LANGNAME, code, sizeof(code));
    code[2] = 0;
    return code;
#endif

    return "";
}

sf::TargetPlatform sf::BaseApplication::getTargetPlatform() const {
#ifdef STRAW_BUILD_FOR_WIN32
    return sf::TargetPlatform::Windows;
#elif defined STRAW_BUILD_FOR_LINUX
    return sf::TargetPlatform::Linux;
#elif defined STRAW_BUILD_FOR_MAC
    return sf::TargetPlatform::Mac;
#endif

    return sf::TargetPlatform::Unknown;
}

sf::BaseManager* sf::BaseApplication::getManager(const sf::string& name) const {
    auto iter = mManagers.find(name);
    if (iter != mManagers.end()) {
        return iter->second;
    }

    return nullptr;
}

void sf::BaseApplication::addManager(
    const sf::string& name, BaseManager* manager) {

    mManagers[name] = manager;
}
