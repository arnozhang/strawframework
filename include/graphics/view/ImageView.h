/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"
#include "thirdparty/skia/core/SkBitmap.h"


namespace sf {

    enum ScaleType {
        FitXY,
        FitStart,
        FitEnd,
        FitCenter,
        InCenter,
        CenterCrop,
        CenterInside
    };


    class STRAW_FRAMEWORK_API ImageView
        : public ElementInherit<ImageView, ViewGroup> {

        IMPL_ELEMENT(ImageView);

    public:
        ImageView(const Context& context);
        virtual ~ImageView();

        virtual void setBitmapName(const sf::string& name);
        virtual void setBitmap(SkBitmap* bitmap);
        virtual void setScaleType(ScaleType scaleType);

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

        void releaseBitmap();

    private:
        SkBitmap* mBitmap;
        ScaleType mScaleType;
    };
}
