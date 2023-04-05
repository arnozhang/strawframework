#include "graphics/view/TextView.h"

#include <algorithm>

#include "graphics/context/Context.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/drawable/Drawable.h"

#include "thirdparty/skia/core/SkUtils.h"
#include "thirdparty/skia/core/SkTypeface.h"


namespace {

    SkTypeface::Style resolveStyle(sf::TextStyle textStyle) {
        SkTypeface::Style style = SkTypeface::Style::kNormal;
        bool bold = sf::checkFlag(textStyle, sf::TextStyle::Bold);
        bool italic = sf::checkFlag(textStyle, sf::TextStyle::Italic);

        if (bold && italic) {
            style = SkTypeface::kBoldItalic;
        }
        else if (bold) {
            style = SkTypeface::kBold;
        }
        else if (italic) {
            style = SkTypeface::kItalic;
        }

        return style;
    }

}


sf::TextView::TextView(const Context& context)
    : ElementInherit(context) {

    mTextColor = new StateListColor(context, SK_ColorBLACK);
    mTextSize = 12;
    mMaxLines = std::numeric_limits<int>::max();
    mLineSpace = 1.0f;
    mLineHeight = 0;
    mEllipsize = sf::EllipsizeType::NoEllipseize;
    mTextStyle = sf::TextStyle::Normal;
    mTextGravity = Gravity::TopLeft;
    mPaint.setAntiAlias(true);
}

sf::TextView::~TextView() {
}

void sf::TextView::setAttributesInternal(Attributes* attrs) {
    ViewGroup::setAttributesInternal(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_STRING(text, setText)
        ATTR_COLOR(textColor, setTextColor)
        ATTR_STATE_COLOR(textColor, setTextStateColor)
        ATTR_DIMENSION(textSize, setTextSize)
        ATTR_STRING(typeface, setTypeface)
        ATTR_INT(maxLines, setMaxLines)
        ATTR_BOOL(singleLine, setSingleLine)
        ATTR_FLOAT(lineSpace, setLineSpace)
        ATTR_ENUM(textStyle, setTextStyle, TextStyle)
        ATTR_ENUM(gravity, setGravity, Gravity)
        ATTR_ENUM(ellipsize, setEllipsize, EllipsizeType)
    END_ATTRS()
}

void sf::TextView::setText(const std::string& text) {
    mText = text;
    requestLayout();
}

void sf::TextView::setTextColor(SkColor color) {
    mTextColor = new StateListColor(mContext, color);
    invalidateSelf();
}

void sf::TextView::setTextStateColor(StateListColor* color) {
    mTextColor = color;
    if (!mTextColor) {
        mTextColor = new StateListColor(
            mContext, static_cast<SkColor>(SK_ColorTRANSPARENT));
    }

    invalidateSelf();
}

void sf::TextView::setTextSize(float size) {
    mTextSize = size;
    mPaint.setTextSize(mTextSize);
    requestLayout();
}

void sf::TextView::setMaxLines(int maxLines) {
    mMaxLines = maxLines;
    requestLayout();
}

void sf::TextView::setSingleLine(bool single) {
    setMaxLines(single ? 1 : std::numeric_limits<int>::max());
}

void sf::TextView::setLineSpace(float space) {
    mLineSpace = space;
    requestLayout();
}

void sf::TextView::setEllipsize(EllipsizeType ellipsize) {
    mEllipsize = ellipsize;
    invalidateSelf();
}

void sf::TextView::setTextStyle(TextStyle textStyle) {
    mTextStyle = textStyle;
    mPaint.setStrikeThruText(sf::checkFlag(mTextStyle, TextStyle::Strike));
    mPaint.setUnderlineText(sf::checkFlag(mTextStyle, TextStyle::Underline));

    SkTypeface* face = SkTypeface::CreateFromTypeface(
        mPaint.getTypeface(), resolveStyle(mTextStyle));
    mPaint.setTypeface(face);

    requestLayout();
}

void sf::TextView::setGravity(Gravity textGravity) {
    mTextGravity = textGravity;
    invalidateSelf();
}

void sf::TextView::setTypeface(const std::string& typeface) {
    mTypeface = typeface;

    SkTypeface* face = SkTypeface::CreateFromName(
        mTypeface.c_str(), resolveStyle(mTextStyle));
    mPaint.setTypeface(face);

    requestLayout();
}

void sf::TextView::onMeasure(MeasureSpec widthSpec, MeasureSpec heightSpec) {
    SkRect bounds;
    mPaint.measureText(mText.c_str(), mText.length(), &bounds);
    mLineHeight = std::abs(bounds.height());
    const int textWidth = std::abs(bounds.width());

    int width = textWidth;
    if (widthSpec.isExactly()) {
        width = widthSpec.dimension;
    }

    int lines = 0;
    if (width > 0) {
        lines = std::max(mText.empty() ? 0 : 1, textWidth / width);
        if (mMaxLines > 0 && lines > mMaxLines) {
            lines = mMaxLines;
        }
    }

    int height = mLineHeight * lines + mLineSpace * lines;
    setMeasuredSize(
        resolveAppropriateSize(widthSpec, mMinSize.width, width + mPadding.horzSum()),
        resolveAppropriateSize(heightSpec, mMinSize.height, height + mPadding.vertSum()));
}

void sf::TextView::onDraw(SkCanvas* canvas) {
    if (mText.empty()) {
        return;
    }

    SkRect bounds;
    mPaint.measureText(mText.c_str(), mText.length(), &bounds);
    mPaint.setColor(mTextColor->getColor(getViewState()));

    int left = mPadding.left;
    int top = mPadding.top + bounds.height();

    if (sf::checkFlag(mTextGravity, Gravity::Top)) {
    } else if (sf::checkFlag(mTextGravity, Gravity::Bottom)) {
        top = mMeasuredSize.height - bounds.height()
            - mPadding.bottom
            - bounds.fTop;
    } else if (sf::checkFlag(mTextGravity, Gravity::CenterVertical)) {
        top = (mMeasuredSize.height - bounds.height()) / 2
            + mPadding.top - mPadding.bottom
            - bounds.fTop;
    }

    if (sf::checkFlag(mTextGravity, Gravity::Left)) {
    } else if (sf::checkFlag(mTextGravity, Gravity::Right)) {
        left = mMeasuredSize.width - bounds.width()
            - mPadding.right
            - bounds.fLeft;
    } else if (sf::checkFlag(mTextGravity, Gravity::CenterHorizontal)) {
        left = (mMeasuredSize.width - bounds.width()) / 2
            + mPadding.left - mPadding.right
            - bounds.fLeft;
    }

    canvas->drawText(mText.c_str(), mText.length(), left, top, mPaint);
}
