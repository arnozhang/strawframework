/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/04/02
 */

#pragma once

#include "graphics/view/TextView.h"


namespace sf {

    class STRAW_FRAMEWORK_API EditText
        : public ElementInherit<EditText, TextView> {

    IMPL_ELEMENT(EditText);

    public:
        EditText(const Context& context);
        virtual ~EditText();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
    };

}
