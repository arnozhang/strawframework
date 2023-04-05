/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "graphics/GraphicsDeclare.h"


namespace sf {

    class STRAW_FRAMEWORK_API Buffer;
    class STRAW_FRAMEWORK_API Context;
    class STRAW_FRAMEWORK_API ElementNode;


    class STRAW_FRAMEWORK_API ResourcesBundle {

    public:
        ResourcesBundle() {}
        virtual ~ResourcesBundle() {}

        virtual ElementNode* getXmlElementNode(
            const Context& context,
            const std::string& resPath) = 0;

        virtual ResourceType getResourceType(
            const std::string& resPath)= 0;

        virtual sf::Buffer* getResBuffer(
            const std::string& resPath,
            ResourceType* resType = nullptr) = 0;

        virtual bool hasResource(const std::string& resPath) const = 0;

        virtual std::vector<std::string> listFiles(
            const std::string& resPath) const = 0;
    };

}
