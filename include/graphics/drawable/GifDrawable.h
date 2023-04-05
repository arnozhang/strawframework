/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/23
 */

#pragma once

#include "graphics/drawable/Drawable.h"
#include "graphics/drawable/BitmapDrawable.h"
#include "graphics/drawable/AnimationDrawable.h"

#include "thirdparty/skia/images/SkMovie.h"


namespace sf {

    class STRAW_FRAMEWORK_API GifDrawable
        : public ElementInherit<GifDrawable, AnimationDrawable> {

    IMPL_ELEMENT(GifDrawable);

    public:
        GifDrawable(const Context& context, SkMovie* movie = nullptr, bool needRelease = true);
        virtual ~GifDrawable();

        virtual void draw(SkCanvas* canvas) override;
        virtual void setMovie(SkMovie* movie, bool needRelease = true);

    protected:
        virtual Drawable* getCurrentDrawable() override;

    private:
        void releaseMovie();
        void parseGifFrames(int duration);

    private:
        SkMovie* mMovie;
        BitmapDrawable mBitmapDrawable;
        std::vector<int> mGifFrameTimes;
        bool mInitialized;
        bool mNeedRelease;
    };
}
