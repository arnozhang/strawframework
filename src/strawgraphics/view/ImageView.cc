#include "graphics/view/ImageView.h"

#include "graphics/view/AttributesHelper.h"


namespace {

    void calcFitCenter(
        SkRect& dst,
        int width, int height,
        int viewWidth, int viewHeight,
        bool bitmapWidthLonger, float ratio) {

        if (bitmapWidthLonger) {
            float h = viewWidth / ratio;
            dst.setXYWH(0, (viewHeight - h) / 2, viewWidth, h);
        }
        else {
            float w = viewHeight * ratio;
            dst.setXYWH((viewWidth - w) / 2, 0, w, viewHeight);
        }
    }

    void calcFitXY(
        SkIRect& src,
        int width, int height,
        int viewWidth, int viewHeight) {

        if (viewWidth < width || viewHeight < height) {
            src.setXYWH((width - viewWidth) / 2, (height - viewHeight) / 2,
                viewWidth, viewHeight);
        }
    }

} // anonymous namespace ends here.


sf::ImageView::ImageView(const Context& context)
    : ElementInherit(context) {

    mBitmap = nullptr;
    mScaleType = ScaleType::FitCenter;
}

sf::ImageView::~ImageView() {
    releaseBitmap();
}

void sf::ImageView::setAttributesInternal(Attributes* attrs) {
    ViewGroup::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_BITMAP(bitmap, setBitmap)
        ATTR_ENUM(scaleType, setScaleType, ScaleType)
    END_ATTRS()
}

void sf::ImageView::setBitmapName(const sf::string& name) {
    releaseBitmap();
}

void sf::ImageView::setBitmap(SkBitmap* bitmap) {
    releaseBitmap();

    mBitmap = bitmap;
    invalidateSelf();
}

void sf::ImageView::setScaleType(ScaleType scaleType) {
    mScaleType = scaleType;
    invalidateSelf();
}

void sf::ImageView::onDraw(SkCanvas* canvas) {
    if (!mBitmap) {
        return;
    }

    int width = mBitmap->width();
    int height = mBitmap->height();
    int viewWidth = mMeasuredSize.width;
    int viewHeight = mMeasuredSize.height;

    if (width == 0 || height == 0 || viewWidth == 0 || viewHeight == 0) {
        return;
    }

    SkIRect src = SkIRect::MakeWH(width, height);
    SkRect dst = SkRect::MakeWH(viewWidth, viewHeight);

    float ratio = (float) width / (float) height;
    float viewRatio = (float) viewWidth / (float) viewHeight;
    bool bitmapWidthLonger = ratio > viewRatio;

    switch (mScaleType)
    {
    case sf::FitXY:
        calcFitXY(src, width, height, viewWidth, viewHeight);
        break;
    case sf::FitStart:
        if (bitmapWidthLonger) {
            dst.setXYWH(0, 0, viewWidth, viewWidth / ratio);
        } else {
            dst.setXYWH(0, 0, viewHeight * ratio, viewHeight);
        }
        break;
    case sf::FitEnd:
        if (bitmapWidthLonger) {
            float h = viewWidth / ratio;
            dst.setXYWH(0, viewHeight - h, viewWidth, h);
        } else {
            float w = viewHeight * ratio;
            dst.setXYWH(viewWidth - w, 0, w, viewHeight);
        }
        break;
    case sf::InCenter:
        if (width > viewWidth || height > viewHeight) {
            calcFitXY(src, width, height, viewWidth, viewHeight);
        } else {
            calcFitCenter(dst, width, height,
                viewWidth, viewHeight, bitmapWidthLonger, ratio);
        }
        break;
    case sf::CenterCrop:
        if (width > height) {
            float w = height * viewRatio;
            src.setXYWH((width - w) / 2, 0, w, height);
        } else {
            float h = width / viewRatio;
            src.setXYWH(0, (height - h) / 2, width, h);
        }
        break;
    case sf::CenterInside:
        if (viewWidth >= width && viewHeight >= height) {
            dst.setXYWH((viewWidth - width) / 2, (viewHeight - height) / 2, width, height);
        } else {
            calcFitCenter(dst, width, height,
                viewWidth, viewHeight, bitmapWidthLonger, ratio);
        }
        break;
    case sf::FitCenter:
        calcFitCenter(dst, width, height,
            viewWidth, viewHeight, bitmapWidthLonger, ratio);
        break;
    default:
        break;
    }

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setFilterQuality(kHigh_SkFilterQuality);
    canvas->drawBitmapRect(*mBitmap, src, dst, &paint);
}

void sf::ImageView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    int bitmapWidth = 0;
    int bitmapHeight = 0;
    if (mBitmap) {
        bitmapWidth = mBitmap->width();
        bitmapHeight = mBitmap->height();
    }

    int width = resolveAppropriateSize(widthSpec, mMinSize.width, bitmapWidth);
    int height = resolveAppropriateSize(heightSpec, mMinSize.height, bitmapHeight);
    setMeasuredSize(width, height);
}

void sf::ImageView::releaseBitmap() {
    if (mBitmap) {
        delete mBitmap;
        mBitmap = nullptr;
    }
}
