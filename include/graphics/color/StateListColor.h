/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>

#include "graphics/view/Element.h"

#include "thirdparty/skia/core/SkColor.h"


namespace sf {

    class STRAW_FRAMEWORK_API ElementNode;


    class STRAW_FRAMEWORK_API StateListColor
        : public ElementInherit<StateListColor, Element> {

    public:
        StateListColor(const Context& context);
        StateListColor(const Context& context, SkColor color);
        virtual ~StateListColor();

        virtual void decodeByElementNode(ElementNode* node);
        virtual ElementType getElementType() const override;

        void addColor(int state, SkColor color);
        SkColor getColor(int state) const;

    private:
        std::map<int, SkColor> mColorMap;
    };
}
