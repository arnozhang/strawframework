/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>
#include <functional>

#include "base/def/PlatformDefine.h"
#include "graphics/view/Attributes.h"


namespace sf {

    class STRAW_FRAMEWORK_API Context;


    class STRAW_FRAMEWORK_API Theme {

    public:
        Theme(Context& context);
        virtual ~Theme();

        void loadTheme();
        Attributes* getThemeByTarget(const std::string& target) const;
        Attributes* getStyle(const std::string& name) const;
        SkColor getColor(const std::string& name) const;
        int getDimension(const std::string& name) const;
        int getInteger(const std::string& name) const;
        float getFloat(const std::string& name) const;

    private:
        void loadValues();
        void parseStyle(ElementNode* element);
        void parseColor(ElementNode* element);
        void parseDimension(ElementNode* element);
        void parseIntegers(ElementNode* element);
        void parseFloats(ElementNode* element);

        Attributes* getStyleInternal(
            const Context& context, const std::string& name) const;

    private:
        Context& mContext;
        std::map<std::string, std::string> mThemeStyles;
        std::map<std::string, std::unique_ptr<Attributes>> mStyles;
        std::map<std::string, SkColor> mColors;
        std::map<std::string, int> mDimens;
        std::map<std::string, int> mIntegers;
        std::map<std::string, float> mFloats;

        Theme(Theme&) = delete;
        Theme& operator= (Theme&) = delete;
    };

}
