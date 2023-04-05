/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/ptr/RefPtr.h"
#include "graphics/view/Element.h"
#include "graphics/view/MeasureSpec.h"
#include "graphics/view/event/ViewEvent.h"
#include "graphics/view/message/MessageDispatcher.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/layout/LayoutParams.h"

#include "thirdparty/skia/core/SkColor.h"
#include "thirdparty/skia/core/SkCanvas.h"


namespace sf {

    enum ViewStateFlags {
        NoChanged = 0x00,
        ForceLayout = 0x01,
        ForceInvalidate = 0x02
    };


    class STRAW_FRAMEWORK_API ViewGroup;


    class STRAW_FRAMEWORK_API View
        : public ElementInherit<View, Element>
          , public MessageDispatcher {

    IMPL_ELEMENT(View);

    public:
        View(const Context& context);
        virtual ~View();

        virtual void setAttributes(Attributes* attrs) override;
        virtual ElementType getElementType() const override;

        virtual void setId(const std::string& id);
        virtual const std::string& getId() const;
        virtual View* findViewById(const std::string& id) const;

        virtual void setMargin(int margin);
        virtual void setMargin(int left, int top, int right, int bottom);
        virtual void setMargin(const Directionx4& margin);
        virtual void setLeftMargin(int left);
        virtual void setTopMargin(int top);
        virtual void setRightMargin(int right);
        virtual void setBottomMargin(int bottom);

        virtual void setPadding(int padding);
        virtual void setPadding(int left, int top, int right, int bottom);
        virtual void setPadding(const Directionx4& padding);
        virtual void setLeftPadding(int left);
        virtual void setTopPadding(int top);
        virtual void setRightPadding(int right);
        virtual void setBottomPadding(int bottom);

        virtual void setFadeEdge(int fadeEdge);
        virtual void setFadeEdge(int left, int top, int right, int bottom);
        virtual void setFadeEdge(const Directionx4& fadeEdge);
        virtual void setLeftFadeEdge(int left);
        virtual void setTopFadeEdge(int top);
        virtual void setRightFadeEdge(int right);
        virtual void setBottomFadeEdge(int bottom);
        virtual void setFadeEdgeColor(SkColor color);

        virtual void setEnable(bool enabled);
        virtual void setClickable(bool clickable);
        virtual void setSelected(bool selected);
        virtual void setPressed(bool pressed);
        virtual void setMouseHovered(bool hovered);

        virtual void setAlpha(float alpha);
        virtual void setBackground(Drawable* drawable);
        virtual void setBackgroundColor(SkColor color);
        virtual void setForeground(Drawable* drawable);
        virtual void setForegroundColor(SkColor color);

        virtual void setVisibility(Visibility _visibility);
        virtual void setVisible();
        virtual void setInvisible();
        virtual void setGone();

        virtual bool canAddChild() const;
        virtual void setLayoutParams(LayoutParams* params);
        virtual LayoutParams& getLayoutParams() const;

        virtual void setWidth(int width);
        virtual void setHeight(int height);
        virtual void setSize(Size size);
        virtual void setSize(int width, int height);

        virtual void setMinWidth(int minWidth);
        virtual void setMinHeight(int minHeight);
        virtual void setWHAspect(float whAspect);
        virtual void setAspectFixOrientation(Orientation orientation);

        virtual void setToolTip(const std::string& toolTip);

        virtual void dispatchDraw(SkCanvas* canvas);
        virtual void dispatchMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec);
        virtual void dispatchLayout();

        virtual void requestLayout();
        virtual void invalidate(Rect* dirty = nullptr, bool immediately = false);
        virtual void invalidateSelf(bool immediately = false);
        virtual void invalidateAll();
        virtual void layoutTo(int x, int y);
        virtual void setMeasuredSize(int width, int height);
        virtual bool positionInView(int x, int y) const;
        virtual Window* getWindow() const;

        virtual void lockUpdate();
        virtual void unLockUpdate();
        virtual bool isViewLocked() const;

    public:
        void setParent(ViewGroup* parent);
        ViewGroup* getParent() const;

        int getViewState() const;
        Visibility getVisibility() const;
        bool isVisible() const;
        bool isInvisible() const;
        bool isGone() const;

        bool isEnable() const;
        bool isSelected() const;
        bool isClickable() const;
        bool isPressed() const;
        bool isMouseHovered() const;

        int getWidth() const;
        int getHeight() const;
        const Size& getSize() const;

        int getXPosition() const;
        int getYPosition() const;
        const Point& getPosition() const;
        Rect getGlobalRect() const;
        Rect getLocalRect() const;

        int getLeftPadding() const;
        int getTopPadding() const;
        int getRightPadding() const;
        int getBottomPadding() const;
        const Directionx4& getPadding() const;

        int getLeftMargin() const;
        int getTopMargin() const;
        int getRightMargin() const;
        int getBottomMargin() const;
        const Directionx4& getMargin() const;

        void setSizeChangedListener(OnSizeChangedListener listener);
        void setPositionChangedListener(OnPositionChangedListener listener);

        std::string getString(const std::string& name) const;
        Drawable* getDrawable(const std::string& name) const;
        void markInvalidate();

    public:
        template <typename Fn>
        void addOnClickListener(Fn& fn) {
            addListener(ViewListener::create(&ViewListener::onClicked, fn));
        }

    protected:
        virtual void setAttributesInternal(Attributes* attrs);
        virtual void onDraw(SkCanvas* canvas);
        virtual void onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec);
        virtual void drawBackground(SkCanvas* canvas);
        virtual void drawFadeEdge(SkCanvas* canvas);
        virtual void drawForeground(SkCanvas* canvas);
        virtual void drawBounds(SkCanvas* canvas);
        virtual void onVisibilityChanged();

        static int resolveAppropriateSize(
            MeasureSpec measureSpec, int minSize, int contentSize = 0);

        void drawFade(SkCanvas* canvas, SkPoint* pts, const SkRect& dst);

    private:
        void updatePadding();

    protected:
        ViewGroup* mParent;
        sf::ref_ptr<Drawable> mBackground;
        sf::ref_ptr<Drawable> mForeground;
        sf::ref_ptr<LayoutParams> mLayoutParams;

        std::string mId;
        Directionx4 mPadding;
        Directionx4 mFadeEdge;
        Visibility mVisibility;
        Point mPosition;
        Size mMeasuredSize;
        Size mMinSize;
        Orientation mAspectFixOrientation;

        float mAlpha;
        float mWhAspect;
        bool mEnable;
        bool mClickable;
        bool mSelected;
        bool mPressed;
        bool mMouseHovered;
        int mUpdateLockCnt;
        SkColor mFadeEdgeColor;
        std::string mToolTip;

        int mViewStateFlag;
        MeasureSpec mOldParentMeasureWidth;
        MeasureSpec mOldParentMeasureHeight;

        OnSizeChangedListener mSizeChangedListener;
        OnPositionChangedListener mPositionChangedListener;
    };

}
