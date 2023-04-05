/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>
#include <Windows.h>

#include "base/StrawUtils.h"
#include "graphics/context/ResourcesBundle.h"


namespace sf {

    namespace impl {

        struct ResNodeInfo {
        public:
            ResNodeInfo()
                : mPathHash(0)
                , mResOffsetBytes(0)
                , mResFileSize(0)
                , mResType(ResourceType::Binary) {
            }

            sf::hash_value mPathHash;
            ResourceType mResType;
            std::string mResPath;
            uint32_t mResOffsetBytes;
            uint32_t mResFileSize;
        };


        class WindowsRdbResourceBundle : public ResourcesBundle {
        public:
            WindowsRdbResourceBundle(const std::string& path, bool isPlatform);
            virtual ~WindowsRdbResourceBundle();

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
            typedef std::multimap<sf::hash_value, ResNodeInfo> ResNodeInfoList;
            typedef ResNodeInfoList::const_iterator res_node_iter;

            bool prepareResources();
            bool loadRdbFile(const std::string& rdbFilePath);
            res_node_iter findResourceNode(const std::string& resPath) const;
            Buffer* getRDBFileBuffer(
                const std::string& resPath, ResourceType* resType = nullptr);

        private:
            void* mRdbFileBuffer;
            void* mRealResFileBuffer;
            HANDLE mFileMappingHandle;
            ResNodeInfoList mResNodeInfoList;
        };
    }
}
