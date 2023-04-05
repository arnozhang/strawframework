/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/Element.h"
#include "thirdparty/skia/core/SkCanvas.h"


namespace sf {


    class STRAW_FRAMEWORK_API View;
    class STRAW_FRAMEWORK_API ElementNode;


    class STRAW_FRAMEWORK_API Drawable
        : public ElementInherit<Drawable, Element, true> {

    IMPL_ELEMENT(Drawable);

    public:
        Drawable(const Context& context);
        virtual ~Drawable();

    public:
        virtual void draw(SkCanvas* canvas) = 0;
        virtual void decodeByElementNode(ElementNode* node);
        virtual ElementType getElementType() const override;

        virtual Size getSize() const;
        virtual void setSize(Size size);
        virtual Size getRawResourceSize() const;

        virtual void attachView(View* view);
        virtual View* detachView();
        virtual void setVisible(bool visible);

    protected:
        virtual void onVisibilityChanged();

    protected:
        Size mSize;
        View* mAttachedView;
        bool mVisible;
    };
}
