#include "graphics/view/Attributes.h"

#include <cstdlib>

#include "base/StrawUtils.h"
#include "base/data/NumericSerializable.h"
#include "graphics/app/CustomizeViewManager.h"
#include "graphics/context/Context.h"
#include "graphics/app/StrawApplication.h"
#include "graphics/color/StateListColor.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/layout/LayoutSupporter.h"

#include "thirdparty/tinyxml2/tinyxml2.h"
#include "thirdparty/skia/core/SkImageDecoder.h"


namespace {

#define ATTR_VERIFY(def) \
    auto iter = mMap.find(name); \
    if (iter == mMap.end()) { \
        return def; \
    } \
    sf::String* ptr = reinterpret_cast<sf::String*>(iter->second); \
    if (ptr->empty()) { \
        return def;\
    } \
    const std::string& val = ptr->value(); \


    const char* const tag_MATCH_PARENT = "match_parent";
    const char* const tag_WRAP_CONTENT = "wrap_content";
    const char* const tag_PIXEL = "px";
    const char* const tag_DP = "dp";
    const char* const tag_SP = "sp";

    const char* const tag_LAYOUT = "layout/";
    const char* const tag_COLOR = "color/";

    const char* const tag_TRUE = "true";
    const char* const tag_FALSE = "false";

    const char separator_character = ',';
    const char enum_character = '|';
    const char color_character = '#';
    const char resource_character = '@';
    const char* numeric_characters = "01234567890.";


    int convertDimensionNoTrim(const sf::Context* context, const std::string& val) {
        if (val == tag_MATCH_PARENT) {
            return sf::LayoutParams::MATCH_PARENT;
        } else if (val == tag_WRAP_CONTENT) {
            return sf::LayoutParams::WRAP_CONTENT;
        }

        if (val[0] == resource_character) {
            return context->getDimension(val);
        }

        float density = 1.0f;
        float value = 0.0f;

        int pos = val.find_first_not_of(numeric_characters);
        if (pos > 0) {
            auto unit = val.substr(pos);
            sf::trim(unit);
            if (unit == tag_DP) {
                density = context->getPixelDensity();
            } else if (unit == tag_PIXEL) {
                density = 1.0f;
            } else if (unit == tag_SP) {
                density = context->getScaledPixelDensity();
            }

            value = static_cast<float>(atof(val.substr(0, pos).c_str()));
        } else {
            value = static_cast<float>(atof(val.c_str()));
        }

        return static_cast<int>(value * density);
    }

    int convertDimension(const sf::Context* context, std::string& val) {
        sf::trim(val);
        return convertDimensionNoTrim(context, val);
    }

} // anonymous namespace ends here.


sf::Attributes::Attributes(const Context& context)
    : Attributes(&context) {
}

sf::Attributes::Attributes(const Context* context /*= nullptr*/)
    : mContext(context) {
}

sf::Attributes::Attributes(Attributes&& rhs)
    : mContext(rhs.mContext) {

    mParentStyleName = rhs.mParentStyleName;
    mMap = std::move(rhs.mMap);
}

sf::Attributes::~Attributes() {
}

const sf::Context* sf::Attributes::getContext() const {
    return mContext;
}

void sf::Attributes::setContext(const Context* context) {
    mContext = context;
}

void sf::Attributes::parseFrom(const tinyxml2::XMLElement* node) {
    const tinyxml2::XMLAttribute* attr = node->FirstAttribute();
    while (attr) {
        std::string value = attr->Value();
        sf::trim(value);

        mMap[attr->Name()] = new String(value);
        attr = attr->Next();
    }
}

void sf::Attributes::parseFromStyle(const tinyxml2::XMLElement* node) {
    auto child = node->FirstChildElement();
    while (child) {
        std::string tag(child->Name());
        if (tag != sf::config::RESOURCE_ITEM_TAG) {
            straw_warning("Parse style find invalid tag: \""
                + tag + "\". (tag must be \"item\")");

            child = child->NextSiblingElement();
            continue;
        }

        auto name = child->Attribute(sf::config::RESOURCE_NAME_ATTR);
        if (!name) {
            straw_warning("Style item must declare name!");
        } else {
            mMap[name] = new String(child->GetText());
        }

        child = child->NextSiblingElement();
    }
}

void sf::Attributes::setParentStyleName(const std::string& parentStyle) {
    mParentStyleName = parentStyle;
}

void sf::Attributes::tryMergeParentStyle() {
    if (mParentStyleName.empty()) {
        return;
    }

    mergeParent(mContext->getStyle(mParentStyleName));
    mParentStyleName.clear();
}

void sf::Attributes::mergeParent(Attributes* attrs) {
    mergeStyles(attrs);
}

void sf::Attributes::mergeStyles(Attributes* attrs) {
    if (!attrs) {
        return;
    }

    for (auto& iter : attrs->mMap) {
        if (mMap.find(iter.first) == mMap.end()) {
            mMap[iter.first] = iter.second->clone();
        }
    }
}

bool sf::Attributes::hasAttribute(const std::string& name) const {
    return mMap.find(name) != mMap.end();
}

bool sf::Attributes::isStateColor(const std::string& name) const {
    ATTR_VERIFY(false);

    // @color/xxx
    //
    return val[0] == '@' && mContext->isStateColor(val);
}

void sf::Attributes::addAttr(const std::string& name, const std::string& value) {
    put(name, new String(value));
}

std::string sf::Attributes::getStringAttr(const std::string& name) {
    ATTR_VERIFY("");

    if (val[0] == '@') {
        // @string/xxx
        return mContext->getString(val);
    }

    return val;
}

std::string sf::Attributes::getRawStringAttr(const std::string& name) {
    ATTR_VERIFY("");
    return val;
}

int sf::Attributes::getIntAttr(const std::string& name) {
    ATTR_VERIFY(0);

    return parseInteger(mContext, val);
}

int sf::Attributes::getEnumAttr(const std::string& name) {
    ATTR_VERIFY(0);

    CustomizeViewManager& manager = CustomizeViewManager::getInstance();

    int enumValue = 0;
    int pos = -1;
    std::string sub;

    while (true) {
        int prev = pos + 1;
        pos = val.find(enum_character, prev);
        if (pos < 0) {
            sub = val.substr(prev);
        } else {
            sub = val.substr(prev, pos - prev);
        }

        enumValue |= manager.getEnum(sf::trim(sub));
        if (pos < 0) {
            break;
        }
    }

    return enumValue;
}

float sf::Attributes::getFloatAttr(const std::string& name) {
    ATTR_VERIFY(0.0f);

    return parseFloat(mContext, val);
}

int sf::Attributes::getDimensionAttr(const std::string& name) {
    ATTR_VERIFY(0);

    return convertDimensionNoTrim(mContext, val);
}

int sf::Attributes::getColorAttr(const std::string& name) {
    ATTR_VERIFY(SK_ColorBLACK);

    return parseColor(mContext, val);
}

bool sf::Attributes::getBoolAttr(const std::string& name) {
    ATTR_VERIFY(false);

    if (val == tag_TRUE) {
        return true;
    } else if (val == tag_FALSE) {
        return false;
    }

    return false;
}

sf::Directionx4 sf::Attributes::getDirectionX4Attr(const std::string& name) {
    ATTR_VERIFY(sf::Directionx4());

    sf::Directionx4 direction;
    int pos = val.find(separator_character);
    if (pos < 0) {
        int value = convertDimensionNoTrim(mContext, val);
        return direction.set(value, value, value, value);
    }

    direction.left = convertDimension(mContext, val.substr(0, pos));

    int next = val.find(separator_character, pos + 1);
    if (next > 0) {
        direction.top = convertDimension(mContext, val.substr(pos + 1, next - pos));
        pos = next;
    }

    next = val.find(separator_character, pos + 1);
    if (next > 0) {
        direction.right = convertDimension(mContext, val.substr(pos + 1, next - pos));
        pos = next;
    }

    direction.bottom = convertDimension(mContext, val.substr(pos + 1));

    return direction;
}

sf::Point sf::Attributes::getPointAttr(const std::string& name) {
    ATTR_VERIFY(sf::Point());

    sf::Point point;
    int pos = val.find(separator_character);
    if (pos < 0) {
        int value = convertDimensionNoTrim(mContext, val);
        return point.set(value, value);
    }

    point.x = convertDimension(mContext, val.substr(0, pos));
    point.y = convertDimension(mContext, val.substr(pos + 1));

    return point;
}

sf::Size sf::Attributes::getSizeAttr(const std::string& name) {
    ATTR_VERIFY(sf::Size());

    sf::Size size;
    int pos = val.find(separator_character);
    if (pos < 0) {
        int value = convertDimensionNoTrim(mContext, val);
        return size.set(value, value);
    }

    size.width = convertDimension(mContext, val.substr(0, pos));
    size.height = convertDimension(mContext, val.substr(pos + 1));

    return size;
}

SkBitmap* sf::Attributes::getBitmapAttr(const std::string& name) {
    ATTR_VERIFY(nullptr);

    return mContext->getBitmap(val);
}

sf::Drawable* sf::Attributes::getDrawableAttr(const std::string& name) {
    ATTR_VERIFY(nullptr);

    if (val.find(color_character) == 0 || val.find(tag_COLOR) == 0) {
        return new ColorDrawable(*mContext, getColorAttr(name));
    }

    // @drawable/xxx
    return mContext->getDrawable(val);
}

sf::StateListColor* sf::Attributes::getStateColorAttr(const std::string& name) {
    ATTR_VERIFY(nullptr);

    if (!isStateColor(name)) {
        StateListColor* color = new StateListColor(*mContext);
        color->addColor(ViewState::StateNormal, parseColor(mContext, val));
        return color;
    }

    return mContext->getStateColor(val);
}

sf::Attributes* sf::Attributes::getStyleAttr(const std::string& name) {
    ATTR_VERIFY(nullptr);

    // @style/xxx
    return mContext->getStyle(val);
}

sf::ElementNode* sf::Attributes::getElementNodeAttr(const std::string& name) {
    ATTR_VERIFY(nullptr);

    return mContext->getElementNode(val);
}

SkColor sf::Attributes::parseColor(
    const Context* context, const std::string& value) {

    if (value[0] == color_character) {
        auto color = value.substr(1);
        if (color.size() <= 4) {
            // #ffff -> #ffffff
            //
            std::string full = "";
            for (auto ch : color) {
                full += ch;
                full += ch;
            }

            color = std::move(full);
        }

        int value = strtol(color.c_str(), nullptr, 16);
        if (color.length() <= 6) {
            value = SkColorSetA(value, 0xff);
        }

        return value;
    } else if (value[0] == resource_character) {
        return context->getColor(value);
    }

    return SK_ColorBLACK;
}

int sf::Attributes::parseDimen(
    const Context* context, const std::string& value) {

    return convertDimensionNoTrim(context, value);
}

int sf::Attributes::parseInteger(
    const Context* context, const std::string& value) {

    if (value[0] == resource_character) {
        return context->getInteger(value);
    }

    return atoi(value.c_str());
}

float sf::Attributes::parseFloat(
    const Context* context, const std::string& value) {

    if (value[0] == resource_character) {
        return context->getFloat(value);
    }

    return static_cast<float>(atof(value.c_str()));
}
