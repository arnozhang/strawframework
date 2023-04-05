/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>

#include "graphics/view/Element.h"
#include "graphics/app/DeclareHelper.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;


    class CustomizeViewManager {

    public:
        virtual ~CustomizeViewManager();

        static CustomizeViewManager& getInstance();
        void addViewCreator(const std::string& name, ElementCreator creator);
        ElementCreator getViewCreator(const std::string& name) const;

        void addEnum(const std::string& name, int enumValue);
        int getEnum(const std::string& name) const;


    private:
        CustomizeViewManager();
        std::map<std::string, ElementCreator> mViewCreatorMap;
        std::map<std::string, int> mEnumValueMap;
    };

}
