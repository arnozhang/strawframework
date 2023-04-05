/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "graphics/drawable/Drawable.h"


namespace sf {

    class STRAW_FRAMEWORK_API NinePatchDrawable
        : public ElementInherit<NinePatchDrawable, Drawable> {

    IMPL_ELEMENT(NinePatchDrawable);

    public:
        NinePatchDrawable(
            const Context& context,
            SkBitmap* bitmap = nullptr,
            bool needRelease = true);
        virtual ~NinePatchDrawable();

        virtual void draw(SkCanvas* canvas) override;
        virtual void attachView(View* view) override;
        virtual Size getRawResourceSize() const override;
        virtual void setBitmap(SkBitmap* bitmap, bool needRelease = true);

    private:
        struct DrawPatchRowParams {
            DrawPatchRowParams(
                SkCanvas* _canvas, SkPaint* _paint,
                float _horzRatio, float _vertRatio) {

                canvas = _canvas;
                paint = _paint;
                horzRatio = _horzRatio;
                vertRatio = _vertRatio;
                update(0, 0, 0, false);
            }

            void update(float _srcStartX, float _srcEndX,
                        float _dstStartX, bool _scaleHorz) {

                srcStartX = _srcStartX;
                srcEndX = _srcEndX;
                dstStartX = _dstStartX;
                scaleHorz = _scaleHorz;
            }

            SkCanvas* canvas;
            SkPaint* paint;
            float horzRatio;
            float vertRatio;
            float srcStartX;
            float srcEndX;
            float dstStartX;
            bool scaleHorz;
        };

        void releaseBitmap();
        void updatePatches();
        void updatePatchZoneInternal(int width, int height);
        void updatePaddingInternal(int width, int height);
        void updatePaddingForView();
        void drawPatchedRow(const DrawPatchRowParams& params) const;

    private:
        struct Range {
            Range() : Range(0, 0) {
            }

            Range(float _start, float _end) {
                start = _start;
                end = _end;
            }

            float length() const {
                return end - start;
            }

            float start;
            float end;
        };

        SkBitmap* mBitmap;
        Directionx4 mPadding;
        SizeF mKeepOrigLength;
        bool mHasHorzPadding;
        bool mHasVertPadding;
        bool mNeedRelease;
        std::vector<Range> mHorzRanges;
        std::vector<Range> mVertRanges;
    };

}
