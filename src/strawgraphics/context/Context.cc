#include "graphics/context/Context.h"

#include "base/data/Buffer.h"
#include "base/StrawUtils.h"
#include "base/file/FileUtils.h"
#include "graphics/app/StrawApplication.h"
#include "graphics/app/CustomizeViewManager.h"
#include "graphics/view/Attributes.h"
#include "graphics/theme/Theme.h"
#include "graphics/color/StateListColor.h"
#include "graphics/drawable/GifDrawable.h"
#include "graphics/drawable/BitmapDrawable.h"
#include "graphics/drawable/NinePatchDrawable.h"
#include "graphics/context/RdbResourcesBundle.h"
#include "graphics/context/FakeResourcesBundle.h"
#include "graphics/context/ElementNode.h"

#include "thirdparty/skia/core/SkStream.h"
#include "thirdparty/skia/core/SkImageDecoder.h"


namespace {

    std::string empty_string_ref = "";

    const char tag_FLAG_RESOURCE = '@';
    const char tag_FLAG_RESOURCE_SRC = ':';

    const char* const tag_PLATFORM_CONTEXT = "platform";
    const char* const tag_APPLICATION_CONTEXT = "app";

    const int len_PLATFORM_CONTEXT = strlen(tag_PLATFORM_CONTEXT);
    const int len_APPLICATION_CONTEXT = strlen(tag_APPLICATION_CONTEXT);

    std::string& eraseResourceBackslash(std::string& path) {
        int pos = path.rfind('/');
        if (pos != -1) {
            path = path.substr(pos + 1);
        }

        return path;
    }

} // anonymous namespace ends here.


sf::Context::Context(bool isPlatform)
    : mStringBundle(*this) {

    mIsPlatform = isPlatform;
    mPixelDensity = 1.0f;
    mLayoutInflater = std::make_unique<LayoutInflater>(*this);
    mTheme = std::make_unique<Theme>(*this);
}

sf::Context::Context(BaseApplication* app, bool isPlatform /*= false*/)
    : Context(app->getHomeDirectory(), isPlatform) {

#ifdef DEBUG
    mIsPackedRdb = false;
#else
    mIsPackedRdb  = true;
#endif // DEBUG
}

sf::Context::Context(
    const std::string& rdbPath,
    bool isPlatform /*= false*/,
    bool isPackedRdb /*= true*/)
    : Context(isPlatform) {

    mRdbPath = rdbPath;
    mIsPackedRdb = isPackedRdb;
}

sf::Context::~Context() {
}

void sf::Context::initialize() {
    loadResBundle(mRdbPath, mIsPackedRdb);
    mTheme->loadTheme();
}

const sf::Theme& sf::Context::getTheme() const {
    return *mTheme;
}

void sf::Context::setTheme(Theme* theme) {
    mTheme.reset(theme);
}

sf::Theme* sf::Context::detachTheme() {
    Theme* attached = mTheme.get();
    mTheme.reset(nullptr);
    return attached;
}

sf::LayoutInflater& sf::Context::getLayoutInflater() const {
    return *mLayoutInflater;
}

sf::ResourcesBundle& sf::Context::getResourceBundle() const {
    return *mResBundle;
}

float sf::Context::getPixelDensity() const {
    return mPixelDensity;
}

float sf::Context::getScaledPixelDensity() const {
    return mPixelDensity;
}

bool sf::Context::drawViewBounds() const {
    return false;
}

std::vector<std::string> sf::Context::listFiles(
    const std::string& directory,
    std::function<bool(const std::string&)> filter) const {

    std::vector<std::string> list = mResBundle->listFiles(directory);
    if (list.empty()) {
        return list;
    }

    for (std::vector<std::string>::iterator iter = list.begin();
        iter != list.end();) {

        if (filter(*iter)) {
            ++iter;
        } else {
            iter = list.erase(iter);
        }
    }

    return list;
}

bool sf::Context::isStateColor(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return false;
    }

    return context.mResBundle->hasResource(path);
}

std::vector<std::string> sf::Context::listFiles(
    const std::string& directory) const {

    return mResBundle->listFiles(directory);
}

void sf::Context::loadResBundle(
    const std::string& resBundlePath, bool isPackedRdb) {

    if (isPackedRdb) {
        mResBundle.reset(new RdbResourcesBundle(resBundlePath, mIsPlatform));
    } else {
        mResBundle.reset(new FakeResourcesBundle(resBundlePath, mIsPlatform));
    }
}

const std::string& sf::Context::getString(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return sf::config::EMPTY_STRING;
    }

    if (path == sf::layout::val_null) {
        return sf::config::EMPTY_STRING;
    }

    eraseResourceBackslash(path);
    Context* ptr = const_cast<Context*>(&context);
    return ptr->mStringBundle.getString(path);
}

sf::Drawable* sf::Context::getDrawable(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return nullptr;
    }

    if (path == sf::layout::val_null) {
        return nullptr;
    }

    ResourceType resType = context.mResBundle->getResourceType(path);
    if (resType == ResourceType::Xml || resType == ResourceType::CompiledXml) {
        std::unique_ptr<ElementNode> node(
            context.mResBundle->getXmlElementNode(context, path));
        if (node) {
            return decodeDrawable(node.get());
        }
    } else {
        ref_ptr<Buffer> content = context.mResBundle->getResBuffer(path);
        if (!content || content->empty()) {
            straw_warning("Drawable Image file not exists: " + name);
            return nullptr;
        }

        if (resType == ResourceType::Image
            || resType == ResourceType::Cursor
            || resType == ResourceType::NinePatchImage) {
            SkBitmap* bitmap = decodeBitmap(name, content);
            if (bitmap) {
                if (resType == ResourceType::NinePatchImage) {
                    return new NinePatchDrawable(*this, bitmap);
                } else {
                    return new BitmapDrawable(*this, bitmap);
                }
            }
        } else if (resType == ResourceType::Gif) {
            SkMovie* movie = decodeMovie(name, content);
            if (movie) {
                return new GifDrawable(*this, movie);
            }
        }
    }

    return nullptr;
}

SkBitmap* sf::Context::getBitmap(const std::string& name) const {
    ref_ptr<Buffer> content = getResourceContent(name);
    if (!content || content->empty()) {
        return nullptr;
    }

    return decodeBitmap(name, content);
}

SkBitmap* sf::Context::decodeBitmap(
    const std::string& name, sf::Buffer* content) const {

    SkBitmap* bitmap = new SkBitmap();
    if (!SkImageDecoder::DecodeMemory(content->buffer(), content->size(), bitmap)) {
        delete bitmap;
        straw_warning("Decode Image FAILED: " + name);
        return nullptr;
    }

    return bitmap;
}

SkMovie* sf::Context::decodeMovie(
    const std::string& name, sf::Buffer* content) const {

    SkMemoryStream stream(content->buffer(), content->size(), false);
    SkMovie* movie = SkMovie::DecodeStream(&stream);
    if (!movie) {
        straw_warning("Decode Gif FAILED: " + name);
    }

    return movie;
}

SkColor sf::Context::getColor(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return SK_ColorBLACK;
    }

    if (path == sf::layout::val_null) {
        return SK_ColorTRANSPARENT;
    }

    eraseResourceBackslash(path);
    return context.mTheme->getColor(path);
}

int sf::Context::getDimension(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return 0;
    }

    if (path == sf::layout::val_null) {
        return 0;
    }

    eraseResourceBackslash(path);
    return context.mTheme->getDimension(path);
}

int sf::Context::getInteger(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return 0;
    }

    if (path == sf::layout::val_null) {
        return 0;
    }

    eraseResourceBackslash(path);
    return context.mTheme->getInteger(path);
}

float sf::Context::getFloat(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return 0;
    }

    if (path == sf::layout::val_null) {
        return 0;
    }

    eraseResourceBackslash(path);
    return context.mTheme->getFloat(path);
}

sf::Attributes* sf::Context::getStyle(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return nullptr;
    }

    eraseResourceBackslash(path);
    return context.mTheme->getStyle(path);
}

sf::StateListColor* sf::Context::getStateColor(const std::string& name) const {
    std::unique_ptr<ElementNode> node(getElementNode(name));
    if (!node) {
        straw_warning("Parse state-color XML Failed: " + name);
        return nullptr;
    }

    return decodeStateColor(node.get());
}

sf::ElementNode* sf::Context::getElementNode(const std::string& name) const {
    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return nullptr;
    }

    return context.mResBundle->getXmlElementNode(context, path);
}

const sf::Context& sf::Context::decodeResource(
    const std::string& name, std::string& path) const {

    return decodeResource(*this, name, path);
}

const sf::Context& sf::Context::decodeResource(
    const Context& context, const std::string& name, std::string& path) {

    const Context* resultContext = &context;

    // @
    if (name[0] == tag_FLAG_RESOURCE) {
        int pos = name.find(tag_PLATFORM_CONTEXT);
        if (pos == 1) {
            // @platform:drawable/xxx
            resultContext = &StrawApplication::getInstance().getPlatformContext();
            path = name.substr(pos + len_PLATFORM_CONTEXT + 1);
        } else {
            pos = name.find(tag_APPLICATION_CONTEXT);
            if (pos == 1) {
                // @app:drawable/xxx
                resultContext = &StrawApplication::getInstance().getAppContext();
                path = name.substr(pos + len_APPLICATION_CONTEXT + 1);
            }
        }

        if (path.empty()) {
            // @drawable/xxx
            path = name.substr(1);
        }
    }

    return *resultContext;
}

sf::Buffer* sf::Context::getResourceContent(
    const std::string& name, ResourceType* resType /*= nullptr*/) const {

    std::string path;
    const Context& context = decodeResource(name, path);
    if (path.empty()) {
        return nullptr;
    }

    return context.mResBundle->getResBuffer(path, resType);
}

sf::Drawable* sf::Context::decodeDrawable(ElementNode* node) const {
    if (!node) {
        return nullptr;
    }

    sf::ElementCreator creator = CustomizeViewManager::getInstance()
        .getViewCreator(node->mNodeName);
    if (!creator) {
        straw_warning("Drawable \"" + node->mNodeName.value() + "\" Creator not defined!");
        return nullptr;
    }

    Element* element = creator(*this);
    Drawable* drawable = sf::element_cast<Drawable>(element, ElementType::Element_Drawable);
    if (!drawable) {
        delete element;
        return nullptr;
    }

    drawable->setAttributes(&node->mAttributes);
    drawable->decodeByElementNode(node);

    return drawable;
}

sf::StateListColor* sf::Context::decodeStateColor(ElementNode* node) const {
    if (!node) {
        return nullptr;
    }

    sf::ElementCreator creator = CustomizeViewManager::getInstance()
        .getViewCreator(node->mNodeName);
    if (!creator) {
        straw_warning("Color \"" + node->mNodeName.value() + "\" Creator not defined!");
        return nullptr;
    }

    Element* element = creator(*this);
    StateListColor* color = sf::element_cast<StateListColor>(element, ElementType::Element_Color);
    if (!color) {
        delete element;
        return nullptr;
    }

    color->setAttributes(&node->mAttributes);
    color->decodeByElementNode(node);

    return color;
}
