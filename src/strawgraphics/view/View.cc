#include "graphics/view/View.h"

#include "graphics/view/Attributes.h"
#include "graphics/view/ViewGroup.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/context/Context.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/layout/LinearLayoutSupporter.h"

#include "thirdparty/skia/core/SkShader.h"
#include "thirdparty/skia/effects/SkGradientShader.h"


sf::View::View(const Context& context)
    : ElementInherit(context)
    , MessageDispatcher(this) {

    mEnable = true;
    mSelected = false;
    mClickable = false;
    mPressed = false;
    mMouseHovered = false;
    mParent = nullptr;
    mUpdateLockCnt = 0;
    mWhAspect = 0;
    mFadeEdgeColor = SK_ColorTRANSPARENT;
    mViewStateFlag = sf::ViewStateFlags::ForceLayout | sf::ViewStateFlags::ForceInvalidate;
    mAspectFixOrientation = sf::Orientation::Horizontal;
    mVisibility = sf::Visibility::Visible;
    mLayoutParams = new LayoutParams();
}

sf::View::~View() {
}

void sf::View::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    ViewUpdateLock lock(this);
    setAttributesInternal(attrs);
}

void sf::View::setAttributesInternal(Attributes* attrs) {
    BEGIN_ATTRS(attrs)
        ATTR_STRING(id, setId)
        ATTR_FLOAT(alpha, setAlpha)
        ATTR_DRAWABLE(background, setBackground)
        ATTR_DRAWABLE(foreground, setForeground)

        ATTR_BOOL(enable, setEnable)
        ATTR_BOOL(clickable, setClickable)
        ATTR_BOOL(selected, setSelected)

        ATTR_DIMENSION(width, setWidth)
        ATTR_DIMENSION(height, setHeight)
        ATTR_SIZE(size, setSize)

        ATTR_INT(minWidth, setMinWidth)
        ATTR_INT(minHeight, setMinHeight)
        ATTR_FLOAT(whAspect, setWHAspect)
        ATTR_ENUM(aspectFixOrientation, setAspectFixOrientation, sf::Orientation)

        ATTR_ENUM(visibility, setVisibility, sf::Visibility)

        ATTR_DIRECTION_X4(margin, setMargin)
        ATTR_DIMENSION(marginLeft, setLeftMargin)
        ATTR_DIMENSION(marginTop, setTopMargin)
        ATTR_DIMENSION(marginRight, setRightMargin)
        ATTR_DIMENSION(marginBottom, setBottomMargin)

        ATTR_DIRECTION_X4(padding, setPadding)
        ATTR_DIMENSION(paddingLeft, setLeftPadding)
        ATTR_DIMENSION(paddingTop, setTopPadding)
        ATTR_DIMENSION(paddingRight, setRightPadding)
        ATTR_DIMENSION(paddingBottom, setBottomPadding)

        ATTR_DIRECTION_X4(fadeEdge, setFadeEdge)
        ATTR_DIMENSION(leftFadeEdge, setLeftFadeEdge)
        ATTR_DIMENSION(topFadeEdge, setTopFadeEdge)
        ATTR_DIMENSION(rightFadeEdge, setRightFadeEdge)
        ATTR_DIMENSION(bottomFadeEdge, setBottomFadeEdge)
        ATTR_COLOR(fadeEdgeColor, setFadeEdgeColor)

        ATTR_STRING(toolTip, setToolTip)
    END_ATTRS()
}

sf::ElementType sf::View::getElementType() const {
    return sf::ElementType::Element_View;
}

void sf::View::setId(const std::string& id) {
    mId = id;
}

const std::string& sf::View::getId() const {
    return mId;
}

sf::View* sf::View::findViewById(const std::string& id) const {
    if (mId == id) {
        return const_cast<View*>(this);
    }

    return nullptr;
}

void sf::View::setMargin(int left, int top, int right, int bottom) {
    mLayoutParams->margin.set(left, top, right, bottom);
    setLayoutParams(mLayoutParams);
}

void sf::View::setMargin(int margin) {
    setMargin(margin, margin, margin, margin);
}

void sf::View::setMargin(const Directionx4& margin) {
    mLayoutParams->margin = margin;
    setLayoutParams(mLayoutParams);
}

void sf::View::setLeftMargin(int left) {
    mLayoutParams->margin.left = left;
    setLayoutParams(mLayoutParams);
}

void sf::View::setTopMargin(int top) {
    mLayoutParams->margin.top = top;
    setLayoutParams(mLayoutParams);
}

void sf::View::setRightMargin(int right) {
    mLayoutParams->margin.right = right;
    setLayoutParams(mLayoutParams);
}

void sf::View::setBottomMargin(int bottom) {
    mLayoutParams->margin.bottom = bottom;
    setLayoutParams(mLayoutParams);
}

void sf::View::setPadding(int left, int top, int right, int bottom) {
    mPadding.set(left, top, right, bottom);
    updatePadding();
}

void sf::View::setPadding(int padding) {
    setPadding(padding, padding, padding, padding);
}

void sf::View::setPadding(const Directionx4& padding) {
    mPadding = padding;
    updatePadding();
}

void sf::View::setLeftPadding(int left) {
    mPadding.left = left;
    updatePadding();
}

void sf::View::setTopPadding(int top) {
    mPadding.top = top;
    updatePadding();
}

void sf::View::setRightPadding(int right) {
    mPadding.right = right;
    updatePadding();
}

void sf::View::setBottomPadding(int bottom) {
    mPadding.bottom = bottom;
    updatePadding();
}

void sf::View::setFadeEdge(int fadeEdge) {
    mFadeEdge.set(fadeEdge, fadeEdge, fadeEdge, fadeEdge);
    invalidateSelf();
}

void sf::View::setLeftFadeEdge(int left) {
    mFadeEdge.left = left;
    invalidateSelf();
}

void sf::View::setTopFadeEdge(int top) {
    mFadeEdge.top = top;
    invalidateSelf();
}

void sf::View::setRightFadeEdge(int right) {
    mFadeEdge.right = right;
    invalidateSelf();
}

void sf::View::setBottomFadeEdge(int bottom) {
    mFadeEdge.bottom = bottom;
    invalidateSelf();
}

void sf::View::setFadeEdgeColor(SkColor color) {
    mFadeEdgeColor = color;
}

void sf::View::setFadeEdge(const Directionx4& fadeEdge) {
    mFadeEdge = fadeEdge;
    invalidateSelf();
}

void sf::View::setFadeEdge(int left, int top, int right, int bottom) {
    mFadeEdge.set(left, top, right, bottom);
    invalidateSelf();
}

void sf::View::setEnable(bool enabled) {
    if (mEnable == enabled) {
        return;
    }

    mEnable = enabled;
    invalidateSelf();
}

void sf::View::setClickable(bool clickable) {
    mClickable = clickable;
    invalidateSelf();
}

void sf::View::setSelected(bool selected) {
    if (mSelected == selected) {
        return;
    }

    mSelected = selected;
    invalidateSelf();
}

void sf::View::setPressed(bool pressed) {
    if (mPressed == pressed) {
        return;
    }

    mPressed = pressed;
    invalidateSelf(true);
}

void sf::View::setMouseHovered(bool hovered) {
    if (mMouseHovered == hovered) {
        return;
    }

    mMouseHovered = hovered;
    invalidateSelf();
}

void sf::View::setAlpha(float alpha) {
    if (mAlpha == alpha) {
        return;
    }

    mAlpha = alpha;
    invalidateSelf();
}

void sf::View::setBackground(Drawable* drawable) {
    if (mBackground) {
        mBackground->detachView();
    }

    mBackground = drawable;
    if (mBackground) {
        mBackground->attachView(this);
    }

    invalidateSelf();
}

void sf::View::setBackgroundColor(SkColor color) {
    setBackground(new sf::ColorDrawable(mContext, color));
}

void sf::View::setForeground(Drawable* drawable) {
    if (mForeground) {
        mForeground->detachView();
    }

    mForeground = drawable;
    if (mForeground) {
        mForeground->attachView(this);
    }

    invalidateSelf();
}

void sf::View::setForegroundColor(SkColor color) {
    setForeground(new sf::ColorDrawable(mContext, color));
}

void sf::View::setVisibility(Visibility _visibility) {
    if (mVisibility == _visibility) {
        return;
    }

    mVisibility = _visibility;
    if (mVisibility == Visibility::Gone) {
        if (mParent) {
            mParent->requestLayout();
        }
    } else if (mVisibility == Visibility::Invisible) {
        if (mParent) {
            mParent->invalidateSelf();
        }
    } else {
        invalidateSelf();
    }
}

void sf::View::setVisible() {
    setVisibility(Visibility::Visible);
}

void sf::View::setInvisible() {
    setVisibility(Visibility::Invisible);
}

void sf::View::setGone() {
    setVisibility(Visibility::Gone);
}

void sf::View::dispatchDraw(SkCanvas* canvas) {
    if (!isVisible()
        || mMeasuredSize.width == 0
        || mMeasuredSize.height == 0
        || !sf::checkFlag(mViewStateFlag, sf::ViewStateFlags::ForceInvalidate)) {
        return;
    }

    SkAutoCanvasRestore lock(canvas, true);
    canvas->translate(mPosition.x, mPosition.y);
    canvas->clipRect(SkRect::MakeWH(mMeasuredSize.width, mMeasuredSize.height));

    drawBackground(canvas);
    drawFadeEdge(canvas);
    onDraw(canvas);
    drawForeground(canvas);

#ifdef DEBUG
    if (mContext.drawViewBounds()) {
        drawBounds(canvas);
    }
#endif // DEBUG

    mViewStateFlag &= ~sf::ViewStateFlags::ForceInvalidate;
}

void sf::View::onDraw(SkCanvas* canvas) {
}

void sf::View::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    int width = resolveAppropriateSize(widthSpec, mMinSize.width);
    int height = resolveAppropriateSize(heightSpec, mMinSize.height);
    setMeasuredSize(width, height);
}

void sf::View::drawBackground(SkCanvas* canvas) {
    if (mBackground) {
        Size size = mMeasuredSize;
        size.width -= mFadeEdge.left + mFadeEdge.right;
        size.height -= mFadeEdge.top + mFadeEdge.bottom;

        SkAutoCanvasRestore lock(canvas, true);
        canvas->translate(mFadeEdge.left, mFadeEdge.top);
        mBackground->setSize(size);
        mBackground->draw(canvas);
    }
}

void sf::View::drawForeground(SkCanvas* canvas) {
    if (mForeground) {
        mForeground->setSize(mMeasuredSize);
        mForeground->draw(canvas);
    }
}

void sf::View::drawBounds(SkCanvas* canvas) {
    SkPaint paint;
    paint.setStrokeWidth(1);
    paint.setColor(SK_ColorBLUE);

    Size size = mMeasuredSize;
    size.height -= 1;
    size.width -= 1;

    canvas->drawLine(0, 0, 0, size.height, paint);
    canvas->drawLine(0, 0, size.width, 0, paint);
    canvas->drawLine(size.width, 0, size.width, size.height, paint);
    canvas->drawLine(0, size.height, size.width + 1, size.height, paint);
}

void sf::View::onVisibilityChanged() {
    bool visible = isVisible();
    if (mBackground) {
        mBackground->setVisible(visible);
    }

    if (mForeground) {
        mForeground->setVisible(visible);
    }
}

void sf::View::drawFade(SkCanvas* canvas, SkPoint* pts, const SkRect& dst) {
    SkColor colors[] = {SK_ColorTRANSPARENT, mFadeEdgeColor};
    SkScalar pos[] = {0.0f, 1.0f};
    SkShader* shader = SkGradientShader::CreateLinear(
        pts, colors, pos, 2, SkShader::TileMode::kRepeat_TileMode);

    SkPaint paint;
    paint.setShader(shader);
    shader->unref();
    canvas->drawRect(dst, paint);
}

void sf::View::drawFadeEdge(SkCanvas* canvas) {
    if (SkColorGetA(mFadeEdgeColor) <= 0) {
        return;
    }

    if (mFadeEdge.left > 0) {
        SkPoint pts[] = {SkPoint::Make(0, 0), SkPoint::Make(mFadeEdge.left, 0)};
        drawFade(canvas, pts, SkRect::MakeXYWH(
            0, mFadeEdge.top,
            mFadeEdge.left, mMeasuredSize.height - mFadeEdge.top - mFadeEdge.bottom));
    }

    if (mFadeEdge.top > 0) {
        SkPoint pts[] = {SkPoint::Make(0, 0), SkPoint::Make(0, mFadeEdge.top)};
        drawFade(canvas, pts, SkRect::MakeXYWH(
            mFadeEdge.left, 0,
            mMeasuredSize.width - mFadeEdge.left - mFadeEdge.right, mFadeEdge.top));
    }

    if (mFadeEdge.right > 0) {
        SkPoint pts[] = {
            SkPoint::Make(mMeasuredSize.width, 0),
            SkPoint::Make(mMeasuredSize.width - mFadeEdge.right, 0)};
        drawFade(canvas, pts, SkRect::MakeXYWH(
            mMeasuredSize.width - mFadeEdge.right, mFadeEdge.top,
            mFadeEdge.right, mMeasuredSize.height - mFadeEdge.top - mFadeEdge.bottom));
    }

    if (mFadeEdge.bottom > 0) {
        SkPoint pts[] = {
            SkPoint::Make(0, mMeasuredSize.height),
            SkPoint::Make(0, mMeasuredSize.height - mFadeEdge.bottom)};
        drawFade(canvas, pts, SkRect::MakeXYWH(
            mFadeEdge.top, mMeasuredSize.height - mFadeEdge.bottom,
            mMeasuredSize.width - mFadeEdge.left - mFadeEdge.right, mFadeEdge.bottom));
    }
}

void sf::View::setMeasuredSize(int width, int height) {
    if (mWhAspect > 0) {
        if (mAspectFixOrientation == sf::Orientation::Horizontal) {
            height = static_cast<int>(width / mWhAspect);
        } else if (mAspectFixOrientation == sf::Orientation::Vertical) {
            width = static_cast<int>(height * mWhAspect);
        }
    }

    Size old = mMeasuredSize;
    mMeasuredSize.set(width, height);

    if (mSizeChangedListener && old != mMeasuredSize) {
        mSizeChangedListener(old, mMeasuredSize);
    }
}

bool sf::View::positionInView(int x, int y) const {
    x -= mPosition.x;
    y -= mPosition.y;
    return x >= 0 && y >= 0 && x <= mMeasuredSize.width && y <= mMeasuredSize.height;
}

int sf::View::resolveAppropriateSize(
    MeasureSpec measureSpec, int minSize, int contentSize /* = 0*/) {

    int size = 0;
    if (measureSpec.isExactly()) {
        size = measureSpec.dimension;
    } else {
        size = contentSize;
        if (measureSpec.isAtMost() && size > measureSpec.dimension) {
            size = measureSpec.dimension;
        }
    }

    return std::max(minSize, size);
}

void sf::View::dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    if (sf::checkFlag(mViewStateFlag, sf::ViewStateFlags::ForceLayout)
        || widthSpec != mOldParentMeasureWidth
        || heightSpec != mOldParentMeasureHeight) {

        onMeasure(widthSpec, heightSpec);

        mOldParentMeasureWidth = widthSpec;
        mOldParentMeasureHeight = heightSpec;
    }

    mViewStateFlag &= ~sf::ViewStateFlags::ForceLayout;
}

void sf::View::dispatchLayout() {
}

void sf::View::requestLayout() {
    mViewStateFlag |= sf::ViewStateFlags::ForceLayout;
    if (isViewLocked()) {
        return;
    }

    if (mParent) {
        mParent->requestLayout();
    }
}

void sf::View::invalidate(
    Rect* dirty /*= nullptr*/, bool immediately /*= false*/) {

    mViewStateFlag |= sf::ViewStateFlags::ForceInvalidate;
    if (isViewLocked()) {
        return;
    }

    if (mParent) {
        mParent->invalidateChild(this, dirty, immediately);
    }
}

void sf::View::invalidateSelf(bool immediately /*= false*/) {
    Rect rc = getGlobalRect();
    invalidate(&rc, immediately);
}

void sf::View::invalidateAll() {
    mViewStateFlag |= sf::ViewStateFlags::ForceLayout
                      | sf::ViewStateFlags::ForceInvalidate;
}

void sf::View::layoutTo(int x, int y) {
    Point old = mPosition;
    mPosition.set(x, y);

    if (mPositionChangedListener && old != mPosition) {
        mPositionChangedListener(old, mPosition);
    }
}

void sf::View::setLayoutParams(LayoutParams* params) {
    if (mParent) {
        params = mParent->createDefaultLayoutParams(params);
    }

    mLayoutParams = params;
    requestLayout();
}

sf::LayoutParams& sf::View::getLayoutParams() const {
    return *mLayoutParams;
}

void sf::View::setWidth(int width) {
    mLayoutParams->width = width;
    setLayoutParams(mLayoutParams);
}

void sf::View::setHeight(int height) {
    mLayoutParams->height = height;
    setLayoutParams(mLayoutParams);
}

void sf::View::setSize(Size size) {
    setSize(size.width, size.height);
}

void sf::View::setMinWidth(int minWidth) {
    mMinSize.width = minWidth;
}

void sf::View::setMinHeight(int minHeight) {
    mMinSize.height = minHeight;
}

void sf::View::setWHAspect(float whAspect) {
    mWhAspect = whAspect;
    requestLayout();
}

void sf::View::setAspectFixOrientation(Orientation orientation) {
    mAspectFixOrientation = orientation;
    requestLayout();
}

void sf::View::setToolTip(const std::string& toolTip) {
    mToolTip = toolTip;
}

bool sf::View::canAddChild() const {
    return false;
}

void sf::View::setSize(int width, int height) {
    mLayoutParams->width = width;
    mLayoutParams->height = height;
    setLayoutParams(mLayoutParams);
}

sf::Visibility sf::View::getVisibility() const {
    return mVisibility;
}

sf::ViewGroup* sf::View::getParent() const {
    return mParent;
}

sf::Window* sf::View::getWindow() const {
    return mParent ? mParent->getWindow() : nullptr;
}

void sf::View::lockUpdate() {
    ++mUpdateLockCnt;
}

void sf::View::unLockUpdate() {
    --mUpdateLockCnt;
}

bool sf::View::isViewLocked() const {
    return mUpdateLockCnt > 0;
}

void sf::View::setParent(ViewGroup* parent) {
    mParent = parent;
}

int sf::View::getViewState() const {
    int flag = ViewState::StateNormal;
    flag |= mEnable ? ViewState::Enabled : ViewState::ViewDisabled;
    flag |= mMouseHovered ? ViewState::MouseHover : ViewState::UnMouseHovered;
    flag |= mSelected ? ViewState::Selected : ViewState::UnSelected;
    flag |= mPressed ? ViewState::Pressed : ViewState::UnPressed;

    return flag;
}

bool sf::View::isVisible() const {
    return getVisibility() == sf::Visibility::Visible;
}

bool sf::View::isInvisible() const {
    return getVisibility() == sf::Visibility::Invisible;
}

bool sf::View::isGone() const {
    return getVisibility() == sf::Visibility::Gone;
}

bool sf::View::isEnable() const {
    return mEnable;
}

bool sf::View::isSelected() const {
    return mSelected;
}

bool sf::View::isClickable() const {
    return mClickable;
}

bool sf::View::isPressed() const {
    return mPressed;
}

bool sf::View::isMouseHovered() const {
    return mMouseHovered;
}

int sf::View::getWidth() const {
    return mMeasuredSize.width;
}

int sf::View::getHeight() const {
    return mMeasuredSize.height;
}

const sf::Size& sf::View::getSize() const {
    return mMeasuredSize;
}

int sf::View::getXPosition() const {
    return mPosition.x;
}

int sf::View::getYPosition() const {
    return mPosition.y;
}

const sf::Point& sf::View::getPosition() const {
    return mPosition;
}

sf::Rect sf::View::getGlobalRect() const {
    Rect rc(0, 0, mMeasuredSize.width, mMeasuredSize.height);
    rc.move(mPosition.x, mPosition.y);

    if (mParent) {
        Rect parentRc = mParent->getGlobalRect();
        rc.move(parentRc.left, parentRc.top);
    }

    return rc;
}

sf::Rect sf::View::getLocalRect() const {
    return Rect(0, 0, mMeasuredSize.width, mMeasuredSize.height)
        .move(mPosition.x, mPosition.y);
}

int sf::View::getLeftPadding() const {
    return mPadding.left;
}

int sf::View::getTopPadding() const {
    return mPadding.top;
}

int sf::View::getRightPadding() const {
    return mPadding.right;
}

int sf::View::getBottomPadding() const {
    return mPadding.bottom;
}

const sf::Directionx4& sf::View::getPadding() const {
    return mPadding;
}

int sf::View::getLeftMargin() const {
    return mLayoutParams->margin.left;
}

int sf::View::getTopMargin() const {
    return mLayoutParams->margin.top;
}

int sf::View::getRightMargin() const {
    return mLayoutParams->margin.right;
}

int sf::View::getBottomMargin() const {
    return mLayoutParams->margin.bottom;
}

const sf::Directionx4& sf::View::getMargin() const {
    return mLayoutParams->margin;
}

void sf::View::setSizeChangedListener(OnSizeChangedListener listener) {
    mSizeChangedListener = listener;
}

void sf::View::setPositionChangedListener(OnPositionChangedListener listener) {
    mPositionChangedListener = listener;
}

std::string sf::View::getString(const std::string& name) const {
    return getContext().getString(name);
}

sf::Drawable* sf::View::getDrawable(const std::string& name) const {
    return getContext().getDrawable(name);
}

void sf::View::markInvalidate() {
    mViewStateFlag |= ViewStateFlags::ForceInvalidate;
}

void sf::View::updatePadding() {
    requestLayout();
}

