/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/data/Array.h"
#include "base/data/NumericSerializable.h"
#include "graphics/view/Attributes.h"

#include "thirdparty/tinyxml2/tinyxml2.h"


namespace sf {

    class ElementNode : public Serializable {

    public:
        ElementNode();
        ElementNode(ElementNode&& rhs);
        ~ElementNode();

        void traversalNode(const Context& context, tinyxml2::XMLElement* node);
        void traversalNode(const Context* context, tinyxml2::XMLElement* node);
        void deserialize(const Context& context, const char* buffer);
        bool valid() const;

        Attributes* detachAttributes();
        Attributes* mergeStyledChildrenAttrs() const;

        virtual Serializable* clone() const;
        virtual size_t byteSize() const;
        virtual void serialize(char* buffer) const;
        virtual size_t deserialize(const char* buffer);
        virtual SerializableType getType() const;

    private:
        void mergeAttributeStyles(const Context& context);
        static void attachContext(ElementNode* node, const Context& context);

    public:
        sf::String mNodeName;
        sf::String mNodeText;
        Attributes mAttributes;
        sf::Array mChildren;
    };
}
