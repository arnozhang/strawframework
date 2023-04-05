/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/04/02
 */

#pragma once

#include "graphics/view/View.h"


namespace sf {

    class STRAW_FRAMEWORK_API TreeView
        : public ElementInherit<TreeView, View> {

        IMPL_ELEMENT(TreeView);

    public:
        TreeView(const Context& context);
        virtual ~TreeView();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
    };

}
