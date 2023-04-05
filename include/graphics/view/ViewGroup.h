/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "graphics/view/View.h"
#include "graphics/layout/LayoutSupporter.h"
#include "graphics/extension/Extension.h"


namespace sf {


    class STRAW_FRAMEWORK_API ViewGroup
        : public ElementInherit<ViewGroup, View>
          , public ExtensionHost {

    IMPL_ELEMENT(ViewGroup);

    public:
        ViewGroup(const Context& context);
        virtual ~ViewGroup();

        virtual bool canAddChild() const override;
        virtual ElementType getElementType() const override;
        virtual LayoutSupporter* createDefaultLayoutSupporter() const;
        virtual LayoutParams* createDefaultLayoutParams(
            LayoutParams* params = nullptr);

        virtual void setLayoutType(LayoutType type);

        virtual View* findViewById(const std::string& id) const override;
        virtual View* getChildAt(int index) const;
        virtual int getChildCount() const;

        virtual void addView(View* view);
        virtual void addView(View* view, int index);
        virtual void addView(View* view, LayoutParams* params);
        virtual void addView(View* view, int index, LayoutParams* params);

        virtual void removeView(View* view);
        virtual void removeViewAt(int index);
        virtual void removeAllViews();

        virtual void onDraw(SkCanvas* canvas) override;
        virtual void dispatchLayout() override;
        virtual void dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) override;
        virtual void invalidateSelf(bool immediately = false) override;
        virtual void invalidateAll() override;
        virtual void invalidateChild(View* child, Rect* dirty, bool immediately);

    protected:
        virtual void setAttributesInternal(Attributes* attrs) override;

        void ensureLayouter();
        void measureChildren(MeasureSpec widthSpec, MeasureSpec heightSpec);
        void measureChild(View* view, MeasureSpec widthSpec, MeasureSpec heightSpec);
        MeasureSpec getChildMeasureSpec(MeasureSpec parent, int padding, int childDimension);

    protected:
        sf::ref_ptr<LayoutSupporter> mLayouter;
        std::vector<sf::ref_ptr<View>> mChildren;
    };
}
