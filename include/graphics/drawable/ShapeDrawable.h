/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/drawable/Drawable.h"

#include "thirdparty/skia/core/SkPath.h"


namespace sf {


    enum ShapeType {
        Rectangle,
        Oval
    };

    enum GradientType {
        NoGradient,
        Linear,
        Radial,
        Sweep,
    };


    class STRAW_FRAMEWORK_API ShapeDrawable
        : public ElementInherit<ShapeDrawable, Drawable> {

        IMPL_ELEMENT(ShapeDrawable);

    public:
        ShapeDrawable(const Context& context);
        virtual ~ShapeDrawable();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void draw(SkCanvas* canvas) override;
        virtual void decodeByElementNode(ElementNode* node) override;

        void setShapeType(ShapeType type);
        void setSolidColor(SkColor solidColor);

        void setGradientType(GradientType type);
        void setGradientStartColor(SkColor startColor);
        void setGradientCenterColor(SkColor centerColor);
        void setGradientEndColor(SkColor endColor);
        void setGradientRadius(int radius);
        void setGradientAngleDegree(int degree);
        void setGradientCenterX(float centerX);
        void setGradientCenterY(float centerY);

        void setStrokeColor(SkColor strokeColor);
        void setStrokeWidth(int width);

        void setCornerRadius(int radius);
        void setTopLeftRadius(int radius);
        void setTopRightRadius(int radius);
        void setBottomLeftRadius(int radius);
        void setBottomRightRadius(int radius);

    private:
        void handleCornersAttr(Attributes& attrs);
        void handleGradientAttr(Attributes& attrs);
        SkShader* createGradientShader() const;

        void drawRectangle(SkCanvas* canvas);
        void drawOval(SkCanvas* canvas);
        void drawSolidRectangle(SkCanvas* canvas, SkPath& path, SkPaint& paint);
        void drawGradientRectangle(SkCanvas* canvas, SkPath& path, SkPaint& paint);

        struct CornerRadius {
            CornerRadius() {
                topLeft = 0;
                topRight = 0;
                bottomLeft = 0;
                bottomRight = 0;
            }

            int topLeft;
            int topRight;
            int bottomLeft;
            int bottomRight;
        };

    private:
        ShapeType mShapeType;
        SkColor mSolidColor;
        CornerRadius mCorner;
        GradientType mGradientType;
        SkColor mStrokeColor;
        SkColor mGradientStartColor;
        SkColor mGradientEndColor;
        SkColor mGradientCenterColor;
        int mGradientAngleDegree;
        int mGradientRadius;
        int mStrokeWidth;
        float mGradientCenterX;
        float mGradientCenterY;
    };

}
