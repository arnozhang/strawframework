/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/04/02
 */

#pragma once

#include "graphics/view/View.h"


namespace sf {


    class STRAW_FRAMEWORK_API CheckBoxListener
        : public ListenerBinder<CheckBoxListener> {

    public:
        virtual void onCheckChanged(bool checked) {
            INVOKE(onCheckChanged, checked);
        }
    };


    class STRAW_FRAMEWORK_API CheckBox
        : public ElementInherit<CheckBox, View> {

        IMPL_ELEMENT(CheckBox);

    public:
        CheckBox(const Context& context);
        virtual ~CheckBox();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void onDraw(SkCanvas* canvas) override;
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;

        virtual void setChecked(bool checked);
        virtual void setText(const std::string& text);

        virtual const std::string& getText() const;
        virtual bool isChecked() const;

        virtual void addCheckBoxListener(CheckBoxListener& listener);

    protected:
        std::string mText;
        bool mIsChecked;
        ListenerList<CheckBoxListener> mCheckBoxListeners;
    };

}
