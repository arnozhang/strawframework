/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/drawable/Drawable.h"


namespace sf {

    class STRAW_FRAMEWORK_API ColorDrawable
        : public ElementInherit<ColorDrawable, Drawable> {

        IMPL_ELEMENT(ColorDrawable);

    public:
        ColorDrawable(const Context& context);
        ColorDrawable(const Context& context, SkColor color);
        virtual ~ColorDrawable();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void draw(SkCanvas* canvas) override;
        virtual void setColor(SkColor color);

    private:
        SkColor mColor;
    };
}
