/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"
#include "graphics/layout/LayoutParams.h"
#include "graphics/layout/LinearLayoutSupporter.h"


namespace sf {


    class STRAW_FRAMEWORK_API LinearLayout
        : public ElementInherit<LinearLayout, ViewGroup> {

        IMPL_ELEMENT(LinearLayout);

    public:
        LinearLayout(const Context& context);
        virtual ~LinearLayout();

        virtual void setLayoutType(LayoutType type) override;
        virtual LayoutSupporter* createDefaultLayoutSupporter() const override;

        void setWeightSum(float weightSum);
        void setGravity(Gravity gravity);
        void setOrientation(Orientation orientation);
        Orientation getOrientation();
        Gravity getGravity() const;

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;
        void measureHorizontal(MeasureSpec widthSpec, MeasureSpec heightSpec);
        void measureVertical(MeasureSpec widthSpec, MeasureSpec heightSpec);

    private:
        LinearLayoutSupporter* getLayouter() const;
        float mWeightSum;
        Gravity mGravity;
    };
}
