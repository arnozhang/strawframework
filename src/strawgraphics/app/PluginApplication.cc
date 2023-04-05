#include "graphics/app/PluginApplication.h"

#include "base/StrawUtils.h"
#include "base/data/Buffer.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/extension/ExtensionManager.h"


namespace {

    const char* const EXTENSION_POINT_FILE = "@extension-point";
    const char* const tag_EXTENSION_POINT = "extension-point";
    const char* const tag_EXTENSION_POINT_NAME = "name";
    const char* const tag_EXTENSION_POINT_AGENT = "agent";

} // anonymous namespace ends here.



sf::PluginApplication::PluginApplication() {
}

sf::PluginApplication::~PluginApplication() {
}

void sf::PluginApplication::initialize(const char* cmdLine /* = nullptr */) {
    BaseApplication::initialize(cmdLine);

    loadExtensionConfig();
}

std::string sf::PluginApplication::getAppName() const {
    return getPluginNameInternal();
}

const std::string& sf::PluginApplication::getHomeDirectory() const {
    if (mPluginHomeDirectory.empty()) {
        PluginApplication& app = const_cast<PluginApplication&>(*this);
        app.mPluginHomeDirectory = BaseApplication::getHomeDirectory()
            + sf::config::PLUGIN_ROOT_DIRECTORY
            + sf::DIRECTORY_SEPARATOR
            + getPluginNameInternal()
            + sf::DIRECTORY_SEPARATOR;
    }

    return mPluginHomeDirectory;
}

const char* sf::PluginApplication::getPluginNameInternal() const {
    return sf::config::EMPTY_STRING.c_str();
}

const std::string sf::PluginApplication::getPluginLibraryFilePath() const {
    return getBinDirectory()
        + getPluginNameInternal()
        + sf::PLUGIN_LIBRARY_EXTENSION;
}

void sf::PluginApplication::loadExtensionConfig() {
    std::unique_ptr<ElementNode> node(mContext->getElementNode(EXTENSION_POINT_FILE));
    if (!node) {
        return;
    }

    for (Serializable* sub : node->mChildren) {
        ElementNode* child = reinterpret_cast<ElementNode*>(sub);
        if (child->mNodeName.value() != tag_EXTENSION_POINT) {
            straw_warning("Extension-Point item must use \"extension-point\" as name!");
        } else {
            auto name = child->mAttributes.getRawStringAttr(tag_EXTENSION_POINT_NAME);
            auto agent = child->mAttributes.getRawStringAttr(tag_EXTENSION_POINT_AGENT);
            if (!name.empty() && !agent.empty()) {
                ExtensionManager::getInstance().registerExtensionAgent(name, agent);
            }
        }
    }
}
