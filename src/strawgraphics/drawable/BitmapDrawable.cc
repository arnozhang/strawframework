#include "graphics/drawable/BitmapDrawable.h"

#include "graphics/view/View.h"


sf::BitmapDrawable::BitmapDrawable(
    const Context& context,
    const SkBitmap* bitmap /*= nullptr*/,
    bool needRelease /*= true*/)
    : ElementInherit(context)
    , mBitmap(bitmap)
    , mReleaseBitmap(needRelease) {
}

sf::BitmapDrawable::~BitmapDrawable() {
    releaseBitmap();
}

void sf::BitmapDrawable::setBitmap(
    const SkBitmap* bitmap, bool needRelease /*= true*/) {

    releaseBitmap();
    mBitmap = bitmap;
    mReleaseBitmap = needRelease;

    if (mAttachedView) {
        mAttachedView->invalidateSelf();
    }
}

void sf::BitmapDrawable::releaseBitmap() {
    if (mReleaseBitmap && mBitmap) {
        delete mBitmap;
        mBitmap = nullptr;
    }
}

void sf::BitmapDrawable::draw(SkCanvas* canvas) {
    if (mBitmap) {
        SkPaint paint;
        paint.setFilterQuality(kHigh_SkFilterQuality);
        canvas->drawBitmapRect(*mBitmap,
                               SkRect::MakeWH(mSize.width, mSize.height), &paint);
    }
}

sf::Size sf::BitmapDrawable::getRawResourceSize() const {
    if (mBitmap) {
        return Size(mBitmap->width(), mBitmap->height());
    }

    return super::getRawResourceSize();
}
