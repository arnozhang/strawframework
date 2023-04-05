/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/04/02
 */

#pragma once

#include "graphics/view/View.h"


namespace sf {

    class STRAW_FRAMEWORK_API ScrollView
        : public ElementInherit<ScrollView, View> {

        IMPL_ELEMENT(ScrollView);

    public:
        ScrollView(const Context& context);
        virtual ~ScrollView();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
    };

}
