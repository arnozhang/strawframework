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

    class STRAW_FRAMEWORK_API LinearLayout;


    class STRAW_FRAMEWORK_API LinearLayoutParams : public LayoutParams {

    public:
        LinearLayoutParams();
        LinearLayoutParams(const LayoutParams& rhs);
        LinearLayoutParams(const LayoutParams* rhs);
        LinearLayoutParams(const LinearLayoutParams& rhs);
        virtual ~LinearLayoutParams();

        LinearLayoutParams& operator= (const LayoutParams& rhs);
        LinearLayoutParams& operator= (const LinearLayoutParams& rhs);

        virtual LayoutType getLayoutType() const override;
        virtual void setAttributes(Attributes* attrs) override;

        void setLayoutGravity(int _gravity);
        void setWeight(float _weight);

    public:
        float weight;
        int gravity;
    };


    class STRAW_FRAMEWORK_API LinearLayoutSupporter : public LayoutSupporter {

    public:
        LinearLayoutSupporter();
        virtual ~LinearLayoutSupporter();

        virtual LayoutType getLayoutType() const override;
        virtual LayoutParams* createDefaultLayoutParams(
            LayoutParams* params = 0) override;

        virtual void layout(ViewGroup* viewGroup) override;

        void setOrientation(Orientation orientation);
        Orientation getOrientation() const;

    private:
        void layoutHorizontal(LinearLayout* viewGroup);
        void layoutVertical(LinearLayout* viewGroup);

    private:
        Orientation mOrientation;
    };

}
