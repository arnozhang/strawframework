/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/drawable/Drawable.h"


namespace sf {

    class STRAW_FRAMEWORK_API BitmapDrawable
        : public ElementInherit<BitmapDrawable, Drawable>{

        IMPL_ELEMENT(BitmapDrawable);

    public:
        BitmapDrawable(
            const Context& context,
            const SkBitmap* bitmap = nullptr,
            bool needRelease = true);
        virtual ~BitmapDrawable();

        virtual void draw(SkCanvas* canvas) override;
        virtual Size getRawResourceSize() const override;
        virtual void setBitmap(const SkBitmap* bitmap, bool needRelease = true);

    private:
        void releaseBitmap();

    private:
        const SkBitmap* mBitmap;
        bool mReleaseBitmap;
    };
}
