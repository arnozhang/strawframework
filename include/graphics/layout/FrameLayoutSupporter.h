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

    class STRAW_FRAMEWORK_API FrameLayoutParams : public LayoutParams {

    public:
        FrameLayoutParams();
        FrameLayoutParams(const LayoutParams& rhs);
        FrameLayoutParams(const LayoutParams* rhs);
        FrameLayoutParams(const FrameLayoutParams& rhs);
        virtual ~FrameLayoutParams();

        FrameLayoutParams& operator= (const LayoutParams& rhs);
        FrameLayoutParams& operator= (const FrameLayoutParams& rhs);

        virtual LayoutType getLayoutType() const override;
        virtual void setAttributes(Attributes* attrs) override;

        void setLayoutGravity(int _gravity);

    public:
        int gravity;
    };


    class STRAW_FRAMEWORK_API FrameLayoutSupporter : public LayoutSupporter {

    public:
        FrameLayoutSupporter();
        virtual ~FrameLayoutSupporter();

        virtual LayoutType getLayoutType() const override;
        virtual LayoutParams* createDefaultLayoutParams(
            LayoutParams* params = 0) override;

        virtual void layout(ViewGroup* viewGroup) override;
    };

}
