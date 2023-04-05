#include "graphics/drawable/NinePatchDrawable.h"

#include <algorithm>

#include "graphics/view/View.h"


sf::NinePatchDrawable::NinePatchDrawable(
    const Context& context,
    SkBitmap* bitmap /*= nullptr*/,
    bool needRelease /*= true*/)
    : ElementInherit(context)
    , mBitmap(nullptr) {

    mNeedRelease = true;
    mHasHorzPadding = false;
    mHasVertPadding = false;

    setBitmap(bitmap, needRelease);
}

sf::NinePatchDrawable::~NinePatchDrawable() {
    releaseBitmap();
}

void sf::NinePatchDrawable::setBitmap(SkBitmap* bitmap, bool needRelease /*= true*/) {
    releaseBitmap();

    mBitmap = bitmap;
    mNeedRelease = needRelease;
    mHasHorzPadding = false;
    mHasVertPadding = false;

    updatePatches();
    updatePaddingForView();

    if (mAttachedView) {
        mAttachedView->invalidateSelf();
    }
}

void sf::NinePatchDrawable::releaseBitmap() {
    if (mNeedRelease) {
        if (mBitmap) {
            delete mBitmap;
        }
    }

    mNeedRelease = true;
    mBitmap = nullptr;
}

void sf::NinePatchDrawable::updatePatches() {
    mHorzRanges.clear();
    mVertRanges.clear();
    mPadding.reset();
    mKeepOrigLength.reset();
    mHasHorzPadding = false;
    mHasVertPadding = false;

    if (!mBitmap) {
        return;
    }

    const int width = mBitmap->width();
    const int height = mBitmap->height();

    if (width <= 2 || height <= 2) {
        return;
    }

    updatePatchZoneInternal(width, height);
    updatePaddingInternal(width, height);
}

void sf::NinePatchDrawable::updatePatchZoneInternal(int width, int height) {
    const int width_1 = width - 1;
    const int height_1 = height - 1;

    int start = 0;
    int transparentPos = 0;
    bool lastIsTransparent = true;

    SkColor color = SK_ColorTRANSPARENT;
    for (int col = 0; col < width; ++col) {
        color = mBitmap->getColor(col, 0);
        if (color == SK_ColorTRANSPARENT) {
            if (!lastIsTransparent) {
                transparentPos = col;
                mHorzRanges.push_back(Range(start, col));
            } else if (col == width_1) {
                mKeepOrigLength.width += col - transparentPos + 1;
            }

            lastIsTransparent = true;
        } else {
            if (lastIsTransparent) {
                start = col;
                mKeepOrigLength.width += col - transparentPos;
            }

            lastIsTransparent = false;
        }
    }

    if (mHorzRanges.empty()) {
        mHorzRanges.push_back(Range(1, width_1));
        mKeepOrigLength.width = 0;
    }

    start = 0;
    transparentPos = 0;
    lastIsTransparent = true;

    for (int row = 0; row < height; ++row) {
        color = mBitmap->getColor(0, row);
        if (color == SK_ColorTRANSPARENT) {
            if (!lastIsTransparent) {
                transparentPos = row;
                mVertRanges.push_back(Range(start, row));
            } else if (row == height_1) {
                mKeepOrigLength.height += row - transparentPos + 1;
            }

            lastIsTransparent = true;
        } else {
            if (lastIsTransparent) {
                start = row;
                mKeepOrigLength.height += row - transparentPos;
            }

            lastIsTransparent = false;
        }
    }

    if (mVertRanges.empty()) {
        mVertRanges.push_back(Range(1, height_1));
        mKeepOrigLength.height = 0;
    }
}

void sf::NinePatchDrawable::updatePaddingInternal(int width, int height) {
    const int width_1 = width - 1;
    const int height_1 = height - 1;

    SkColor color = SK_ColorTRANSPARENT;
    for (int col = 1; col < width_1; ++col) {
        color = mBitmap->getColor(col, height_1);
        if (color != SK_ColorTRANSPARENT) {
            mPadding.left = col - 1;
            mHasHorzPadding = true;
            break;
        }
    }

    if (mHasHorzPadding) {
        for (int col = width - 2; col >= 0; --col) {
            color = mBitmap->getColor(col, height_1);
            if (color != SK_ColorTRANSPARENT) {
                mPadding.right = width - 2 - col;
                break;
            }
        }
    }

    for (int row = 1; row < height_1; ++row) {
        color = mBitmap->getColor(width_1, row);
        if (color != SK_ColorTRANSPARENT) {
            mPadding.top = row - 1;
            mHasVertPadding = true;
            break;
        }
    }

    if (mHasVertPadding) {
        for (int row = height - 2; row >= 0; --row) {
            color = mBitmap->getColor(width_1, row);
            if (color != SK_ColorTRANSPARENT) {
                mPadding.bottom = height - 2 - row;
                break;
            }
        }
    }
}

void sf::NinePatchDrawable::updatePaddingForView() {
    if (!mAttachedView || (!mHasHorzPadding && !mHasVertPadding)) {
        return;
    }

    mAttachedView->lockUpdate();

    if (mHasHorzPadding) {
        mAttachedView->setLeftPadding(mPadding.left);
        mAttachedView->setRightPadding(mPadding.right);
    }

    if (mHasVertPadding) {
        mAttachedView->setTopPadding(mPadding.top);
        mAttachedView->setBottomPadding(mPadding.bottom);
    }

    mAttachedView->unLockUpdate();
    mAttachedView->requestLayout();
}

void sf::NinePatchDrawable::draw(SkCanvas* canvas) {
    if (!mBitmap || mSize.width <= 0 || mSize.height <= 0) {
        return;
    }

    const int width = mBitmap->width();
    const int height = mBitmap->height();
    if (width <= 2 || height <= 2) {
        return;
    }

    SkPaint paint;
    paint.setFilterQuality(kHigh_SkFilterQuality);
    bool widthLarge = mKeepOrigLength.width >= mSize.width;
    bool heightLarge = mKeepOrigLength.height >= mSize.height;
    if (widthLarge && heightLarge) {
        canvas->drawBitmapRect(*mBitmap,
                               SkRect::MakeXYWH(1, 1, width - 2, height - 2),
                               SkRect::MakeWH(mSize.width, mSize.height), &paint);
        return;
    }

    const float horzRatio = (mSize.width - mKeepOrigLength.width + 2)
                            / static_cast<float>(width - mKeepOrigLength.width);
    const float vertRatio = (mSize.height - mKeepOrigLength.height + 2)
                            / static_cast<float>(height - mKeepOrigLength.height);

    DrawPatchRowParams params(canvas, &paint, horzRatio, vertRatio);

    float srcStartX = 1;
    float dstStartX = 0;
    for (auto& col : mHorzRanges) {
        if (col.start > srcStartX) {
            params.update(srcStartX, col.start, dstStartX, false);
            drawPatchedRow(params);
            dstStartX += col.start - srcStartX;
        }

        params.update(col.start, col.end, dstStartX, true);
        drawPatchedRow(params);
        dstStartX += col.length() * horzRatio;
        srcStartX = col.end;
    }

    auto endPos = mHorzRanges.rbegin()->end;
    if (endPos < width) {
        params.update(endPos, width - 1, dstStartX, false);
        drawPatchedRow(params);
    }
}

void sf::NinePatchDrawable::attachView(View* view) {
    View* old = mAttachedView;
    Drawable::attachView(view);
    if (old != view) {
        updatePaddingForView();
    }
}

sf::Size sf::NinePatchDrawable::getRawResourceSize() const {
    if (mBitmap) {
        return Size(std::max(0, mBitmap->width() - 2), std::max(0, mBitmap->height() - 2));
    }

    return super::getRawResourceSize();
}

void sf::NinePatchDrawable::drawPatchedRow(
    const DrawPatchRowParams& params) const {

    SkRect src;
    SkRect dst;

    src.fLeft = params.srcStartX;
    src.fRight = params.srcEndX;
    dst.fLeft = params.dstStartX;
    if (params.scaleHorz) {
        dst.fRight = dst.fLeft + params.horzRatio * src.width();
    } else {
        dst.fRight = dst.fLeft + src.width();
    }

    float top = 0;
    float lastVert = 1;
    for (auto& row : mVertRanges) {
        float height = 0;
        if (row.start > lastVert) {
            // blank zone. (Keep vertical original)
            //
            src.fTop = lastVert;
            src.fBottom = row.start;
            height = src.height();

            dst.fTop = top;
            dst.fBottom = dst.fTop + height;

            params.canvas->drawBitmapRect(*mBitmap, src, dst, params.paint);
            top += height;
        }

        // scale zone.
        //
        src.fTop = row.start;
        src.fBottom = row.end;
        height = src.height();

        dst.fTop = top;
        dst.fBottom = dst.fTop + params.vertRatio * height;
        params.canvas->drawBitmapRect(*mBitmap, src, dst, params.paint);

        lastVert = row.end + 1;
        top += dst.height();
    }

    auto height = mBitmap->height();
    auto endPos = mVertRanges.rbegin()->end;
    if (endPos < height - 1) {
        src.fTop = endPos;
        src.fBottom = height - 1;
        height = src.height();

        dst.fTop = top;
        dst.fBottom = dst.fTop + height;

        params.canvas->drawBitmapRect(*mBitmap, src, dst, params.paint);
    }
}
