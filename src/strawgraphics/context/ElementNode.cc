#include "graphics/context/ElementNode.h"

#include "base/StrawUtils.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/theme/Theme.h"
#include "graphics/context/Context.h"


sf::ElementNode::ElementNode() {
}

sf::ElementNode::ElementNode(ElementNode&& rhs)
    : mNodeName(std::move(rhs.mNodeName))
    , mChildren(std::move(rhs.mChildren))
    , mAttributes(std::move(rhs.mAttributes)) {
}

sf::ElementNode::~ElementNode() {
}

void sf::ElementNode::traversalNode(
    const Context* context, tinyxml2::XMLElement* node) {

    mNodeName = node->Name();
    mAttributes.setContext(context);
    mAttributes.parseFrom(node);
    mNodeText = node->GetText();

    if (context) {
        mergeAttributeStyles(*context);
    }

    auto child = node->FirstChildElement();
    while (child) {
        ElementNode* childNode = new ElementNode();
        childNode->traversalNode(context, child);
        mChildren.push_back(childNode);

        child = child->NextSiblingElement();
    }
}

void sf::ElementNode::traversalNode(
    const Context& context, tinyxml2::XMLElement* node) {
    traversalNode(&context, node);
}

void sf::ElementNode::deserialize(const Context& context, const char* buffer) {
    deserialize(buffer);
    attachContext(this, context);
}

void sf::ElementNode::mergeAttributeStyles(const Context& context) {
    if (mAttributes.hasAttribute(sf::layout::attr_style)) {
        Attributes* styles = mAttributes.getStyleAttr(sf::layout::attr_style);
        if (!styles) {
            straw_warning("Cannot find style for \"" + mNodeName.value() + "\"!");
        } else {
            mAttributes.mergeStyles(styles);
        }
    }

    Attributes* themeAttrs = context.getTheme().getThemeByTarget(mNodeName);
    mAttributes.mergeStyles(themeAttrs);
}

void sf::ElementNode::attachContext(ElementNode* node, const Context& context) {
    node->mAttributes.setContext(&context);
    node->mergeAttributeStyles(context);

    for (Serializable* sub : node->mChildren) {
        ElementNode* child = reinterpret_cast<ElementNode*>(sub);
        attachContext(child, context);
    }
}

bool sf::ElementNode::valid() const {
    return !mNodeName.value().empty();
}

sf::Attributes* sf::ElementNode::detachAttributes() {
    return new Attributes(std::move(mAttributes));
}

sf::Attributes* sf::ElementNode::mergeStyledChildrenAttrs() const {
    Attributes* attrs = new Attributes(mAttributes.getContext());
    for (Serializable* sub : mChildren) {
        ElementNode* child = reinterpret_cast<ElementNode*>(sub);
        attrs->addAttr(
            child->mAttributes.getRawStringAttr(sf::config::RESOURCE_NAME_ATTR),
            child->mNodeText);
    }

    return attrs;
}

sf::Serializable* sf::ElementNode::clone() const {
    ElementNode* node = new ElementNode();
    return node;
}

size_t sf::ElementNode::byteSize() const {
    return getBaseSize()
        + mNodeName.byteSize()
        + mAttributes.byteSize()
        + mNodeText.byteSize()
        + mChildren.byteSize();
}

size_t sf::ElementNode::deserialize(const char* buffer) {
    mNodeName.clear();
    mAttributes.clear();
    mChildren.clear();

    if (!buffer) {
        return 0;
    }

    SerializableType type = deserializeType(buffer);
    uint32_t size = deserializeSize(buffer);

    if (type != getType()) {
        return size;
    }

    buffer += mNodeName.deserialize(buffer) +getBaseSize();
    buffer += mAttributes.deserialize(buffer) + getBaseSize();
    buffer += mNodeText.deserialize(buffer) + getBaseSize();
    buffer += mChildren.deserialize(buffer) + getBaseSize();

    return size;
}

sf::SerializableType sf::ElementNode::getType() const {
    return static_cast<SerializableType>(
        GraphicsSerializableType::t_ElementNode);
}

void sf::ElementNode::serialize(char* buffer) const {
    size_t offset = serializeBuffer(
        buffer, getType(), nullptr, byteSize() - getBaseSize());
    buffer += offset;

    mNodeName.serialize(buffer);
    buffer += mNodeName.byteSize();

    mAttributes.serialize(buffer);
    buffer += mAttributes.byteSize();

    mNodeText.serialize(buffer);
    buffer += mNodeText.byteSize();

    mChildren.serialize(buffer);
    buffer += mChildren.byteSize();
}
