/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/LayoutParams.h"
#include "graphics/layout/AbsoluteLayoutSupporter.h"


namespace sf {


    class STRAW_FRAMEWORK_API AbsoluteLayout
        : public ElementInherit<AbsoluteLayout, ViewGroup> {

        IMPL_ELEMENT(AbsoluteLayout);

    public:
        AbsoluteLayout(const Context& context);
        virtual ~AbsoluteLayout();

        virtual void setLayoutType(LayoutType type) override;
        virtual LayoutSupporter* createDefaultLayoutSupporter() const override;

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
        AbsoluteLayoutSupporter* getLayouter() const;
    };
}
