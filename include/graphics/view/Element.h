/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/ptr/RefPtrObject.h"
#include "graphics/GraphicsDeclare.h"


#define IMPL_ELEMENT(elementName) \
public: \
    static const char* getElemenetName() { \
        return #elementName; \
    } \
    virtual const char* getElementNameInternal() const override { \
        return elementName::getElemenetName(); \
    } \


#define ELEMENT_CREATOR(elementName) \
public: \
    static elementName* newObject(const sf::Context& context) { \
        return new elementName(context); \
    } \


namespace sf {

    enum ElementType {
        Element_None = 0x000,
        Element_Element = 0x001,
        Element_View = 0x002,
        Element_ViewGroup = 0x004 | Element_View,
        Element_Window = 0x008,
        Element_Drawable = 0x010,
        Element_Color = 0x020,
        Element_Animation = 0x040,
        Element_Menu = 0x080,
        Element_MenuItem = 0x100,
    };


    class STRAW_FRAMEWORK_API Context;
    class STRAW_FRAMEWORK_API Attributes;


    template <typename This, bool isAbstract = false>
    class ElementCreatorTraits {
    ELEMENT_CREATOR(This);
    };

    template <typename This>
    class ElementCreatorTraits<This, true> {
    };

    template <typename This, typename Parent, bool isAbstract = false>
    class ElementInherit
        : public ElementCreatorTraits<This, isAbstract>
          , public InheritTraits<This, Parent> {

    public:
        template <typename... T>
        ElementInherit(T& ... args) : InheritTraits(args...) {
        }

        virtual const char* getElementNameInternal() const = 0;

    private:
        ElementInherit(const ElementInherit&) = delete;
        ElementInherit& operator=(const ElementInherit&) = delete;
    };


    class STRAW_FRAMEWORK_API Element
        : public RefPtrObject
          , public ElementInherit<Element, Object> {

    IMPL_ELEMENT(Element);

    public:
        Element(const Context& context);
        virtual ~Element();

        virtual void setAttributes(Attributes* attrs);
        virtual Attributes* getAttributes() const;
        virtual ElementType getElementType() const;

        const Context& getContext() const;


    protected:
        const Context& mContext;

    private:
        Element(const Element&) = delete;
        Element& operator=(const Element&) = delete;
    };


    template <typename T>
    T* element_cast(Element* element, ElementType type) {
        if (!element) {
            return nullptr;
        }

        if (!sf::checkFlag(element->getElementType(), type)) {
            return nullptr;
        }

        return reinterpret_cast<T*>(element);
    }
}
