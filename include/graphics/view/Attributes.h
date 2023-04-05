/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>
#include <vector>
#include <memory>

#include "base/data/Data.h"

#include "thirdparty/skia/core/SkBitmap.h"


namespace tinyxml2 {
    class XMLElement;
}


namespace sf {

    class STRAW_FRAMEWORK_API Context;
    class STRAW_FRAMEWORK_API Drawable;
    class STRAW_FRAMEWORK_API StateListColor;
    class STRAW_FRAMEWORK_API ElementNode;


    class STRAW_FRAMEWORK_API Attributes : public Data {

    public:
        Attributes(const Context& context);
        Attributes(const Context* context = nullptr);
        Attributes(Attributes&& rhs);
        ~Attributes();

        const Context* getContext() const;
        void setContext(const Context* context);
        void parseFrom(const tinyxml2::XMLElement* node);
        void parseFromStyle(const tinyxml2::XMLElement* node);
        void setParentStyleName(const std::string& parentStyle);
        void tryMergeParentStyle();

        void mergeParent(Attributes* attrs);
        void mergeStyles(Attributes* attrs);
        bool hasAttribute(const std::string& name) const;
        bool isStateColor(const std::string& name) const;
        void addAttr(const std::string& name, const std::string& value);

        int getIntAttr(const std::string& name);
        int getEnumAttr(const std::string& name);
        int getColorAttr(const std::string& name);
        int getDimensionAttr(const std::string& name);
        bool getBoolAttr(const std::string& name);
        float getFloatAttr(const std::string& name);

        std::string getStringAttr(const std::string& name);
        std::string getRawStringAttr(const std::string& name);
        Directionx4 getDirectionX4Attr(const std::string& name);
        Point getPointAttr(const std::string& name);
        Size getSizeAttr(const std::string& name);
        SkBitmap* getBitmapAttr(const std::string& name);
        Drawable* getDrawableAttr(const std::string& name);
        StateListColor* getStateColorAttr(const std::string& name);
        Attributes* getStyleAttr(const std::string& name);
        ElementNode* getElementNodeAttr(const std::string& name);

        static SkColor parseColor(const Context* context, const std::string& value);
        static int parseDimen(const Context* context, const std::string& value);
        static int parseInteger(const Context* context, const std::string& value);
        static float parseFloat(const Context* context, const std::string& value);

    private:
        std::string mParentStyleName;
        const Context* mContext;
    };
}
