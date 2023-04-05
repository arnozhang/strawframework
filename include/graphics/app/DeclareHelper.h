/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once


#include "base/def/PlatformDefine.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;
    class STRAW_FRAMEWORK_API Element;
    class STRAW_FRAMEWORK_API Context;


    using ElementCreator = Element* (*)(const Context&);


    namespace declare {

        class ViewDeclarer {
        public:
            ViewDeclarer(const char* viewName, ElementCreator creator);
        };


        class EnumDeclarer {
        public:
            EnumDeclarer(const char* enumName, int enumValue);
        };
    }


    namespace tag {

        const char* const MOUSE_HOVER = "hovered";
        const char* const PRESSED = "pressed";
        const char* const ENABLED = "enabled";
        const char* const SELECTED = "selected";
        const char* const DRAWABLE = "drawable";
        const char* const COLOR = "color";
        const char* const ITEM = "item";

    } // anonymous namespace ends here.
}


#define __UNIQUE_GENERATOR_(prefix, unique) prefix##unique
#define __UNIQUE_GENERATOR(prefix, unique) __UNIQUE_GENERATOR_(prefix, unique)
#define __LINE_UNIQUE_GENERATOR(prefix) __UNIQUE_GENERATOR(prefix, __LINE__)


#define __DECLARE_ELEMENT(viewName, viewType) \
    namespace { \
        sf::declare::ViewDeclarer __LINE_UNIQUE_GENERATOR(__declare_view)( \
            viewName, \
            reinterpret_cast<sf::ElementCreator>(&sf::ElementCreatorTraits<viewType>::newObject)); \
    } \


#define DECLARE_ELEMENT_NAME(viewName, viewType) \
    __DECLARE_ELEMENT(#viewName, viewType)


#define DECLARE_ELEMENT(viewType) \
    __DECLARE_ELEMENT(viewType::getElemenetName(), viewType)


#define DECLARE_ENUM(enumName) \
    namespace { \
        sf::declare::EnumDeclarer __LINE_UNIQUE_GENERATOR(__declare_enum)( \
            #enumName, enumName); \
    } \

