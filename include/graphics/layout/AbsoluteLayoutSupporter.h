/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/layout/LayoutSupporter.h"
#include "graphics/view/View.h"


namespace sf {

    class STRAW_FRAMEWORK_API AbsoluteLayoutParams : public LayoutParams {

    public:
        AbsoluteLayoutParams();
        AbsoluteLayoutParams(const LayoutParams& rhs);
        AbsoluteLayoutParams(const LayoutParams* rhs);
        AbsoluteLayoutParams(const AbsoluteLayoutParams& rhs);
        virtual ~AbsoluteLayoutParams();

        AbsoluteLayoutParams& operator= (const LayoutParams& rhs);
        AbsoluteLayoutParams& operator= (const AbsoluteLayoutParams& rhs);

        virtual LayoutType getLayoutType() const override;
        virtual void setAttributes(Attributes* attrs) override;

        void setPosition(const Point& pt);

    public:
        Point position;
    };


    class STRAW_FRAMEWORK_API AbsoluteLayoutSupporter : public LayoutSupporter {

    public:
        AbsoluteLayoutSupporter();
        virtual ~AbsoluteLayoutSupporter();

        virtual LayoutType getLayoutType() const override;
        virtual LayoutParams* createDefaultLayoutParams(
            LayoutParams* params = 0) override;

        virtual void layout(ViewGroup* viewGroup);
    };

}
