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

    class STRAW_FRAMEWORK_API FakeResourcesBundle : public ResourcesBundle {

    public:
        FakeResourcesBundle(const std::string& path, bool isPlatform);
        virtual ~FakeResourcesBundle();

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

        static ElementNode* parseXmlElementNode(
            const Context& context, Buffer* content, const std::string& resPath);

    private:
        struct ResourceNode {
            ResourceNode() {
                resType = ResourceType::Binary;
            }

            std::string fullPath;
            ResourceType resType;
        };

        ResourceNode* getResFullPath(const std::string& resPath);
        bool resolveResourceType(
            ResourceNode& node,
            const std::string& path,
            const std::string& extension,
            ResourceType resType);

    private:
        std::string mResPath;
        std::map<std::string, ResourceNode> mResFullPathMap;
    };
}
