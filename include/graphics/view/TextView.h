/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"
#include "graphics/color/StateListColor.h"


namespace sf {

    enum EllipsizeType {
        NoEllipseize,
        Start,
        Middle,
        End
    };


    enum TextStyle {
        Normal = 0x00,
        Italic = 0x01,
        Bold = 0x02,
        Underline = 0x04,
        Strike = 0x08
    };


    class STRAW_FRAMEWORK_API TextView
        : public ElementInherit<TextView, ViewGroup> {

        IMPL_ELEMENT(TextView);

    public:
        TextView(const Context& context);
        virtual ~TextView();

        virtual void setText(const std::string& text);
        virtual void setTextColor(SkColor color);
        virtual void setTextStateColor(StateListColor* color);
        virtual void setTextSize(float size);
        virtual void setMaxLines(int maxLines);
        virtual void setSingleLine(bool single);
        virtual void setLineSpace(float space);
        virtual void setEllipsize(EllipsizeType ellipsize);
        virtual void setTextStyle(TextStyle textStyle);
        virtual void setGravity(Gravity textGravity);
        virtual void setTypeface(const std::string& typeface);

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

    private:
        std::string mText;
        ref_ptr<StateListColor> mTextColor;
        float mTextSize;
        int mMaxLines;
        float mLineSpace;
        std::string mTypeface;
        EllipsizeType mEllipsize;
        TextStyle mTextStyle;
        Gravity mTextGravity;
        SkPaint mPaint;
        int mLineHeight;
    };
}
