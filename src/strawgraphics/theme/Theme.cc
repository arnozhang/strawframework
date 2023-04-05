#include "graphics/theme/Theme.h"

#include "base/data/Buffer.h"
#include "graphics/context/Context.h"
#include "graphics/app/StrawApplication.h"


namespace {

    const char* const tag_STYLES = "style";
    const char* const tag_COLORS = "color";
    const char* const tag_DIMENSION = "dimen";
    const char* const tag_INTEGERS = "integer";
    const char* const tag_FLOATS = "float";

} // anonymous namespace ends here.


sf::Theme::Theme(Context& context)
    : mContext(context) {
}

sf::Theme::~Theme() {
}

void sf::Theme::loadTheme() {
    loadValues();
}

void sf::Theme::loadValues() {
    std::vector<std::string> list = mContext.listFiles("values");
    for (auto& file : list) {
        std::string name = "@" + file;
        std::unique_ptr<ElementNode> node(mContext.getElementNode(name));
        if (!node) {
            continue;
        }

        if (node->mNodeName.value() != sf::config::RESOURCE_ROOT_TAG) {
            straw_warning("values(styles/strings/colors...) "
                              "XML resource must started with root resources: " + name);
            continue;
        }

        for (Serializable* sub : node->mChildren) {
            ElementNode* child = reinterpret_cast<ElementNode*>(sub);
            std::string& tag = child->mNodeName;

            if (tag == tag_STYLES) {
                // <style name="xxx">...</style>
                //
                parseStyle(child);
            } else if (tag == tag_COLORS) {
                // <color name="xxx">...</color>
                //
                parseColor(child);
            } else if (tag == tag_DIMENSION) {
                // <dimen name="xxx">...</dimen>
                //
                parseDimension(child);
            } else if (tag == tag_INTEGERS) {
                // <integer name="xxx">...</integer>
                //
                parseIntegers(child);
            } else if (tag == tag_DIMENSION) {
                // <float name="xxx">...</float>
                //
                parseFloats(child);
            } else {
                straw_warning("Parse values XML find invalid tag: \""
                              + tag + "\". (" + name + ")");
            }
        }
    }
}

void sf::Theme::parseStyle(ElementNode* element) {
    Attributes* attrs = element->mergeStyledChildrenAttrs();

    std::string styleName = element->mAttributes.getRawStringAttr(
        sf::config::RESOURCE_NAME_ATTR);
    auto target = element->mAttributes.getRawStringAttr(sf::config::RESOURCE_TARGET_ATTR);
    if (styleName.empty()) {
        styleName = target;
    }

    if (!target.empty()) {
        mThemeStyles[target] = styleName;
    }

    auto parent = element->mAttributes.getRawStringAttr(sf::config::RESOURCE_PARENT_ATTR);
    if (!parent.empty()) {
        attrs->setParentStyleName(parent);
    }

    mStyles[styleName].reset(attrs);
}

void sf::Theme::parseColor(ElementNode* element) {
    auto colorName = element->mAttributes.getRawStringAttr(
        sf::config::RESOURCE_NAME_ATTR);
    auto& text = element->mNodeText;
    if (!colorName.empty() && !text.empty()) {
        mColors[colorName] = Attributes::parseColor(&mContext, text);
    }
}

void sf::Theme::parseDimension(ElementNode* element) {
    auto dimenName = element->mAttributes.getRawStringAttr(
        sf::config::RESOURCE_NAME_ATTR);
    auto& text = element->mNodeText;
    if (!dimenName.empty() && !text.empty()) {
        mDimens[dimenName] = Attributes::parseDimen(&mContext, text);
    }
}

void sf::Theme::parseIntegers(ElementNode* element) {
    auto integerName = element->mAttributes.getRawStringAttr(
        sf::config::RESOURCE_NAME_ATTR);
    auto& text = element->mNodeText;
    if (!integerName.empty() && !text.empty()) {
        mIntegers[integerName] = Attributes::parseInteger(&mContext, text);
    }
}

void sf::Theme::parseFloats(ElementNode* element) {
    auto integerName = element->mAttributes.getRawStringAttr(
        sf::config::RESOURCE_NAME_ATTR);
    auto& text = element->mNodeText;
    if (!integerName.empty() && !text.empty()) {
        mFloats[integerName] = Attributes::parseFloat(&mContext, text);
    }
}

sf::Attributes* sf::Theme::getThemeByTarget(const std::string& target) const {
    auto iter = mThemeStyles.find(target);
    bool valid = iter != mThemeStyles.end();

    auto& app = StrawApplication::getInstance();
    auto& platformContext = app.getPlatformContext();
    auto& appContext = app.getAppContext();

    Attributes* attrs = iter != mThemeStyles.end()
                        ? getStyleInternal(mContext, iter->second) : nullptr;

    if (&mContext == &platformContext) {
        return attrs;
    } else if (&mContext == &appContext) {
        Attributes* platformTheme = platformContext.getTheme().getThemeByTarget(target);
        if (attrs) {
            attrs->mergeStyles(platformTheme);
        } else {
            attrs = platformTheme;
        }
    } else {
        // plugin
        Attributes* appTheme = appContext.getTheme().getThemeByTarget(target);
        if (attrs) {
            attrs->mergeStyles(appTheme);
        } else {
            attrs = appTheme;
        }
    }

    return attrs;
}

sf::Attributes* sf::Theme::getStyle(const std::string& name) const {
    return getStyleInternal(mContext, name);
}

SkColor sf::Theme::getColor(const std::string& name) const {
    auto iter = mColors.find(name);
    return iter != mColors.end() ? iter->second : SK_ColorBLACK;
}

int sf::Theme::getDimension(const std::string& name) const {
    auto iter = mDimens.find(name);
    return iter != mDimens.end() ? iter->second : 0;
}

int sf::Theme::getInteger(const std::string& name) const {
    auto iter = mIntegers.find(name);
    return iter != mIntegers.end() ? iter->second : 0;
}

float sf::Theme::getFloat(const std::string& name) const {
    auto iter = mFloats.find(name);
    return iter != mFloats.end() ? iter->second : 0;
}

sf::Attributes* sf::Theme::getStyleInternal(
    const Context& context, const std::string& name) const {

    const Theme& theme = context.getTheme();
    auto iter = theme.mStyles.find(name);
    Attributes* attrs = iter != theme.mStyles.end() ? iter->second.get() : nullptr;
    if (attrs) {
        attrs->tryMergeParentStyle();
    }

    return attrs;
}
