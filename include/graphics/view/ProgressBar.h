/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/04/02
 */

#pragma once

#include "graphics/view/View.h"


namespace sf {

    class STRAW_FRAMEWORK_API ProgressBar
        : public ElementInherit<ProgressBar, View> {

        IMPL_ELEMENT(ProgressBar);

    public:
        ProgressBar(const Context& context);
        virtual ~ProgressBar();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
    };

}
