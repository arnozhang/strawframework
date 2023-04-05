/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/ViewGroup.h"


namespace sf {

    class STRAW_FRAMEWORK_API RootViewListener {

    public:
        virtual ~RootViewListener() {}

        virtual void scheduleLayout() = 0;
        virtual void scheduleInvalidate(
            Rect* dirty = nullptr, bool immediately = false) = 0;
    };


    class STRAW_FRAMEWORK_API RootView : public ViewGroup {

    public:
        RootView(Window* window);
        virtual ~RootView();

        virtual void addView(View* view) override;
        virtual void addView(View* view, int index) override;
        virtual void addView(View* view, LayoutParams* params) override;
        virtual void addView(View* view, int index, LayoutParams* params) override;
        virtual void dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;
        virtual void dispatchLayout() override;

        virtual void requestLayout() override;
        virtual void invalidate(Rect* dirty = nullptr, bool immediately = false) override;

        virtual Window* getWindow() const override;

    private:
        sf::ref_ptr<Window> mWindow;
    };
}
