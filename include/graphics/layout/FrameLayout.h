/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/FrameLayoutSupporter.h"


namespace sf {

    class STRAW_FRAMEWORK_API FrameLayout
        : public ElementInherit<FrameLayout, ViewGroup> {

        IMPL_ELEMENT(FrameLayout);

    public:
        FrameLayout(const Context& context);
        virtual ~FrameLayout();

        virtual void setLayoutType(LayoutType type) override;
        virtual LayoutSupporter* createDefaultLayoutSupporter() const override;

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
        FrameLayoutSupporter* getLayouter() const;
    };

}