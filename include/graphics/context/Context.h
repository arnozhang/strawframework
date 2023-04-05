/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <functional>

#include "base/def/PlatformDefine.h"
#include "graphics/context/StringBundle.h"
#include "graphics/context/LayoutInflater.h"
#include "graphics/context/ResourcesBundle.h"

#include "thirdparty/skia/images/SkMovie.h"


namespace sf {

    class STRAW_FRAMEWORK_API Theme;
    class STRAW_FRAMEWORK_API BaseApplication;


    class STRAW_FRAMEWORK_API Context {

    public:
        Context(BaseApplication* application, bool isPlatform = false);
        Context(const std::string& rdbPath, bool isPlatform = false, bool isPackedRdb = true);
        virtual ~Context();

        virtual void initialize();
        LayoutInflater& getLayoutInflater() const;
        ResourcesBundle& getResourceBundle() const;
        const Theme& getTheme() const;
        void setTheme(Theme* theme);
        Theme* detachTheme();

        const std::string& getString(const std::string& name) const;
        Drawable* getDrawable(const std::string& name) const;
        SkBitmap* getBitmap(const std::string& name) const;
        SkColor getColor(const std::string& name) const;
        int getDimension(const std::string& name) const;
        int getInteger(const std::string& name) const;
        float getFloat(const std::string& name) const;
        StateListColor* getStateColor(const std::string& name) const;
        Attributes* getStyle(const std::string& name) const;
        ElementNode* getElementNode(const std::string& name) const;
        Buffer* getResourceContent(
            const std::string& name, ResourceType* resType = nullptr) const;

        Drawable* decodeDrawable(ElementNode* node) const;
        StateListColor* decodeStateColor(ElementNode* node) const;
        std::vector<std::string> listFiles(const std::string& directory) const;
        std::vector<std::string> listFiles(
            const std::string& directory,
            std::function<bool(const std::string&)> filter) const;

        bool isStateColor(const std::string& name) const;
        float getPixelDensity() const;
        float getScaledPixelDensity() const;
        bool drawViewBounds() const;

        const Context& decodeResource(
            const std::string& name, std::string& path) const;
        static const Context& decodeResource(
            const Context& context, const std::string& name, std::string& path);

    private:
        Context(bool isPlatform);
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        void loadResBundle(const std::string& resBundlePath, bool isPackedRdb);
        SkBitmap* decodeBitmap(const std::string& name, sf::Buffer* content) const;
        SkMovie* decodeMovie(const std::string& name, sf::Buffer* content) const;

    private:
        std::unique_ptr<Theme> mTheme;
        std::unique_ptr<ResourcesBundle> mResBundle;
        std::unique_ptr<LayoutInflater> mLayoutInflater;

        StringBundle mStringBundle;
        float mPixelDensity;
        bool mIsPlatform;

        std::string mRdbPath;
        bool mIsPackedRdb;
    };
}
