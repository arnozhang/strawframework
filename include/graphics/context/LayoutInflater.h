/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "graphics/context/ElementNode.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;
    class STRAW_FRAMEWORK_API ViewGroup;
    class STRAW_FRAMEWORK_API Window;
    class STRAW_FRAMEWORK_API Context;
    class STRAW_FRAMEWORK_API Menu;


    class STRAW_FRAMEWORK_API LayoutInflater {

    public:
        LayoutInflater(const Context& context);
        ~LayoutInflater();

        View* inflate(const std::string& res);
        View* inflate(const std::string& res, ViewGroup* parent);
        View* inflate(const std::string& res, ViewGroup* parent, bool attachToParent);

        Window* inflateWindow(const std::string& res, Window* parent = nullptr);
        Menu* inflateMenu(const std::string& res, Window* window);

    private:
        View* inflate(ElementNode* node, ViewGroup* parent, bool attachToParent);
        Menu* inflateMenu(ElementNode* node, Window* window, Menu* parent);

    private:
        const Context& mContext;
    };
}
