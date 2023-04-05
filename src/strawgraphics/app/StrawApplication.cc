#include "graphics/app/StrawApplication.h"

#include "base/data/Array.h"
#include "base/data/NumericSerializable.h"
#include "base/timer/Timer.h"
#include "base/file/FileUtils.h"
#include "base/loop/Looper.h"
#include "graphics/app/PluginApplication.h"


namespace {

    sf::StrawApplication* application = nullptr;

} // anonymous namespace ends here.


sf::StrawApplication& sf::StrawApplication::getInstance() {
    return *application;
}

const std::thread::id& sf::StrawApplication::getMainThreadId() {
    return Looper::getMainThreadId();;
}

sf::Context& sf::StrawApplication::getPlatformContext() const {
    return *mPlatformContext;
}

void sf::StrawApplication::initialize(const char* cmdLine /*= nullptr*/) {
    Looper::inMainThread();
    sf::timer::startTimerSystem();

    mPlatformContext->initialize();
    BaseApplication::initialize(cmdLine);

    loadPlugins();
}

sf::StrawApplication::StrawApplication() {
    application = this;
    mPlatformContext = nullptr;

    initSerializable();
    createPlatformContext();
}

sf::StrawApplication::~StrawApplication() {
    delete mPlatformContext;
    mPlatformContext = nullptr;
    application = nullptr;
}

void sf::StrawApplication::run() {
    Looper& loop = Looper::mainLooper();
    loop.start();

    clean();
}

void sf::StrawApplication::terminate() {
    sf::timer::terminateTimerSystem();
    Looper::mainLooper().terminate();
}

bool sf::StrawApplication::isTerminated() const {
    return Looper::mainLooper().isTerminated();
}

void sf::StrawApplication::clean() {
    for (auto plugin : mPluginList) {
        delete plugin.second;
    }

    BaseApplication::clean();
}

void sf::StrawApplication::addPlugin(PluginApplication* plugin) {
    const std::string& pluginName = plugin->getPluginNameInternal();
    auto iter = mPluginList.find(pluginName);
    if (iter != mPluginList.end()) {
    } else {
        mPluginList[pluginName] = plugin;
    }
}

sf::PluginApplication* sf::StrawApplication::getPlugin(
    const std::string& pluginName) {

    auto iter = mPluginList.find(pluginName);
    if (iter != mPluginList.end()) {
        return iter->second;
    } else {
        straw_warning("Can not find plugin Application: " + pluginName);
    }

    return nullptr;
}

std::string sf::StrawApplication::getPluginRootDirectory() const {
    return getHomeDirectory()
        + std::string(sf::config::PLUGIN_ROOT_DIRECTORY)
        + std::string(sf::DIRECTORY_SEPARATOR);
}

std::string sf::StrawApplication::getPluginDirectory(
    const std::string& pluginName) const {

    return getPluginRootDirectory()
        + pluginName
        + std::string(sf::DIRECTORY_SEPARATOR);
}

std::string sf::StrawApplication::getPluginBinDirectory(
    const std::string& pluginName) const {

    return getPluginDirectory(pluginName)
        + std::string(sf::BIN_DIRECTORY_NAME)
        + std::string(sf::DIRECTORY_SEPARATOR);
}

std::string sf::StrawApplication::getPluginLibraryFilePath(
    const std::string& pluginName) const {

    return getPluginBinDirectory(pluginName)
        + pluginName
        + std::string(sf::PLUGIN_LIBRARY_EXTENSION);
}

void sf::StrawApplication::initSerializable() {
    auto creator = [](SerializableType type) -> Serializable* {
        if (type == GraphicsSerializableType::t_Attributes) {
            return new Attributes();
        } else if (type == GraphicsSerializableType::t_ElementNode) {
            return new ElementNode();
        }

        return nullptr;
    };

    Serializable::registerSerializableCreator(
        GraphicsSerializableType::t_Attributes, creator);
    Serializable::registerSerializableCreator(
        GraphicsSerializableType::t_ElementNode, creator);
}

void sf::StrawApplication::createPlatformContext() {
    mPlatformContext = new Context(this, true);
}

void sf::StrawApplication::loadPlugins() {
    std::string configFile = getPluginRootDirectory() + sf::config::PLUGIN_CONFIG_FILE;
    ref_ptr<Buffer> buffer = sf::file::getFileContent(configFile);
    if (!buffer || buffer->empty()) {
        return;
    }

    sf::Array plugins;
    plugins.deserialize(buffer->buffer());
    if (plugins.empty()) {
        return;
    }

    for (auto node : plugins) {
        if (node->getType() == SerializableType::t_String) {
            String* plugin = reinterpret_cast<String*>(node);
            std::string pluginPath = getPluginLibraryFilePath(plugin->value());
            if (sf::path::isPathOrFileExists(pluginPath)) {
                loadPlugin(plugin->value(), pluginPath);
            }
        }
    }
}

void sf::StrawApplication::loadPlugin(
    const std::string& pluginName,
    const std::string& pluginLibrary) {

#ifdef STRAW_BUILD_FOR_WIN32
    ::LoadLibraryA(pluginLibrary.c_str());
#else
#endif // STRAW_BUILD_FOR_WIN32
}
