/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>

#include "graphics/context/ResourcesBundle.h"


namespace sf {

    class STRAW_FRAMEWORK_API RdbResourcesBundle : public ResourcesBundle {

    public:
        RdbResourcesBundle(const std::string& path, bool isPlatform);
        virtual ~RdbResourcesBundle();

        virtual ElementNode* getXmlElementNode(
            const Context& context,
            const std::string& resPath) override;

        virtual ResourceType getResourceType(
            const std::string& resPath) override;

        virtual sf::Buffer* getResBuffer(
            const std::string& resPath,
            ResourceType* resType = nullptr) override;

        virtual bool hasResource(const std::string& resPath) const override;

        virtual std::vector<std::string> listFiles(
            const std::string& resPath) const override;

    private:
        ResourcesBundle* mBundleImpl;
    };
}
