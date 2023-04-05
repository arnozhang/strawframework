/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/23
 */

#pragma once

#include <memory>
#include <vector>

#include "base/loop/Looper.h"
#include "graphics/drawable/Drawable.h"


namespace sf {

    class STRAW_FRAMEWORK_API AnimationDrawable
        : public ElementInherit<AnimationDrawable, Drawable> {

        IMPL_ELEMENT(AnimationDrawable);

    public:
        AnimationDrawable(const Context& context);
        virtual ~AnimationDrawable();

        virtual void setAttributes(Attributes* attrs) override;
        virtual void draw(SkCanvas* canvas) override;
        virtual void decodeByElementNode(ElementNode* node) override;
        virtual void setSize(Size size) override;
        virtual void setVisible(bool visible) override;
        virtual Size getRawResourceSize() const override;

        virtual void addFrame(Drawable* drawable, int duration);
        virtual void setRepeat(bool repeat);

    protected:
        virtual Drawable* getCurrentDrawable();
        void scheduleNextFrame();

    private:
        virtual void onVisibilityChanged() override;

    protected:
        struct DrawableFrame {
            DrawableFrame() {
                duration = 0;
            }

            DrawableFrame(Drawable* _drawable, int _duration) {
                drawable.reset(_drawable);
                duration = _duration;
            }

            DrawableFrame(DrawableFrame& rhs) {
                drawable = std::move(rhs.drawable);
                duration = rhs.duration;
            }

            std::unique_ptr<Drawable> drawable;
            int duration;
        };

        TaskId mScheduleId;
        int mCurrFrameIndex;
        bool mRepeat;
        std::vector<DrawableFrame> mDrawableFrames;
    };
}
