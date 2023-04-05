#include "graphics/drawable/ShapeDrawable.h"

#include <algorithm>

#include "base/StrawUtils.h"
#include "graphics/context/ElementNode.h"
#include "graphics/view/AttributesHelper.h"

#include "thirdparty/skia/effects/SkGradientShader.h"


namespace {

    const char* const tag_SOLID = "solid";
    const char* const tag_GRADIENT = "gradient";
    const char* const tag_STROKE = "stroke";
    const char* const tag_CORNERS = "corners";

    const char* const tag_COLOR = "color";
    const char* const tag_STROKE_WIDTH = "width";

    const char* const tag_RADIUS = "radius";
    const char* const tag_TOPLEFT_RADIUS = "topLeftRadius";
    const char* const tag_TOPRIGHT_RADIUS = "topRightRadius";
    const char* const tag_BOTTOMLEFT_RADIUS = "bottomLeftRadius";
    const char* const tag_BOTTOMRIGHT_RADIUS = "bottomRightRadius";

    const char* const tag_GRADIENT_TYPE = "type";
    const char* const tag_START_COLOR = "startColor";
    const char* const tag_END_COLOR = "endColor";
    const char* const tag_CENTER_COLOR = "centerColor";
    const char* const tag_ANGLE_DEGREE = "angle";
    const char* const tag_GRADIENT_RADUIS = "gradientRadius";
    const char* const tag_GRADIENT_CENTER_X = "centerX";
    const char* const tag_GRADIENT_CENTER_Y = "centerY";


} // anonymous namespace ends here.


sf::ShapeDrawable::ShapeDrawable(const Context& context)
    : ElementInherit(context) {

    mSolidColor = SK_ColorTRANSPARENT;
    mStrokeWidth = 0;
    mStrokeColor = SK_ColorBLACK;
    mShapeType = ShapeType::Rectangle;
    mGradientType = GradientType::NoGradient;
    mGradientRadius = 0;
    mGradientStartColor = SK_ColorTRANSPARENT;
    mGradientEndColor = SK_ColorTRANSPARENT;
    mGradientCenterColor = -1;
    mGradientAngleDegree = 0;
    mGradientCenterX = 0.5f;
    mGradientCenterY = 0.5f;
}

sf::ShapeDrawable::~ShapeDrawable() {
}

void sf::ShapeDrawable::setAttributes(Attributes* attrs) {
    super::setAttributes(attrs);

    BEGIN_ATTRS(attrs)
        ATTR_ENUM(shape, setShapeType, ShapeType)
    END_ATTRS()
}

void sf::ShapeDrawable::setShapeType(ShapeType type) {
    mShapeType = type;
}

void sf::ShapeDrawable::setSolidColor(SkColor solidColor) {
    mSolidColor = solidColor;
}

void sf::ShapeDrawable::setGradientType(GradientType type) {
    mGradientType = type;
}

void sf::ShapeDrawable::setGradientStartColor(SkColor startColor) {
    mGradientStartColor = startColor;
}

void sf::ShapeDrawable::setGradientCenterColor(SkColor centerColor) {
    mGradientCenterColor = centerColor;
}

void sf::ShapeDrawable::setGradientEndColor(SkColor endColor) {
    mGradientEndColor = endColor;
}

void sf::ShapeDrawable::setGradientRadius(int radius) {
    mGradientRadius = radius;
}

void sf::ShapeDrawable::setGradientAngleDegree(int degree) {
    mGradientAngleDegree = degree;
}

void sf::ShapeDrawable::setGradientCenterX(float centerX) {
    mGradientCenterX = centerX;
}

void sf::ShapeDrawable::setGradientCenterY(float centerY) {
    mGradientCenterY = centerY;
}

void sf::ShapeDrawable::setStrokeColor(SkColor strokeColor) {
    mStrokeColor = strokeColor;
}

void sf::ShapeDrawable::setStrokeWidth(int width) {
    mStrokeWidth = width;
}

void sf::ShapeDrawable::setCornerRadius(int radius) {
    mCorner.topLeft = radius;
    mCorner.topRight = radius;
    mCorner.bottomLeft = radius;
    mCorner.bottomRight = radius;
}

void sf::ShapeDrawable::setTopLeftRadius(int radius) {
    mCorner.topLeft = radius;
}

void sf::ShapeDrawable::setTopRightRadius(int radius) {
    mCorner.topRight = radius;
}

void sf::ShapeDrawable::setBottomLeftRadius(int radius) {
    mCorner.bottomLeft = radius;
}

void sf::ShapeDrawable::setBottomRightRadius(int radius) {
    mCorner.bottomRight = radius;
}

void sf::ShapeDrawable::draw(SkCanvas* canvas) {
    if (mShapeType == ShapeType::Oval) {
        drawOval(canvas);
    } else {
        drawRectangle(canvas);
    }
}

void sf::ShapeDrawable::decodeByElementNode(ElementNode* node) {
    for (Serializable* iter : node->mChildren) {
        ElementNode* sub = reinterpret_cast<ElementNode*>(iter);
        const std::string& name = sub->mNodeName;
        Attributes& attrs = sub->mAttributes;

        if (name == tag_SOLID) {
            setSolidColor(attrs.getColorAttr(tag_COLOR));
        } else if (name == tag_GRADIENT) {
            handleGradientAttr(attrs);
        } else if (name == tag_STROKE) {
            setStrokeColor(attrs.getColorAttr(tag_COLOR));
            setStrokeWidth(attrs.getDimensionAttr(tag_STROKE_WIDTH));
        } else if (name == tag_CORNERS) {
            handleCornersAttr(attrs);
        } else {
            straw_warning("Find invalid item \"" + name
                + "\" when decode ShapeDrawable.");
        }
    }
}

void sf::ShapeDrawable::handleCornersAttr(Attributes& attrs) {
    if (attrs.hasAttribute(tag_RADIUS)) {
        setCornerRadius(attrs.getDimensionAttr(tag_RADIUS));
    }

    if (attrs.hasAttribute(tag_TOPLEFT_RADIUS)) {
        setTopLeftRadius(attrs.getDimensionAttr(tag_TOPLEFT_RADIUS));
    }

    if (attrs.hasAttribute(tag_TOPRIGHT_RADIUS)) {
        setTopRightRadius(attrs.getDimensionAttr(tag_TOPRIGHT_RADIUS));
    }

    if (attrs.hasAttribute(tag_BOTTOMLEFT_RADIUS)) {
        setBottomLeftRadius(attrs.getDimensionAttr(tag_BOTTOMLEFT_RADIUS));
    }

    if (attrs.hasAttribute(tag_BOTTOMRIGHT_RADIUS)) {
        setBottomRightRadius(attrs.getDimensionAttr(tag_BOTTOMRIGHT_RADIUS));
    }
}

void sf::ShapeDrawable::handleGradientAttr(Attributes& attrs) {
    if (attrs.hasAttribute(tag_GRADIENT_TYPE)) {
        setGradientType((GradientType) attrs.getEnumAttr(tag_GRADIENT_TYPE));
    }

    if (attrs.hasAttribute(tag_START_COLOR)) {
        setGradientStartColor(attrs.getColorAttr(tag_START_COLOR));
    }

    if (attrs.hasAttribute(tag_END_COLOR)) {
        setGradientEndColor(attrs.getColorAttr(tag_END_COLOR));
    }

    if (attrs.hasAttribute(tag_CENTER_COLOR)) {
        setGradientCenterColor(attrs.getColorAttr(tag_CENTER_COLOR));
    }

    if (attrs.hasAttribute(tag_GRADIENT_RADUIS)) {
        setGradientRadius(attrs.getDimensionAttr(tag_GRADIENT_RADUIS));
    }

    if (attrs.hasAttribute(tag_ANGLE_DEGREE)) {
        setGradientAngleDegree(attrs.getIntAttr(tag_ANGLE_DEGREE));
    }

    if (attrs.hasAttribute(tag_GRADIENT_CENTER_X)) {
        setGradientCenterX(attrs.getFloatAttr(tag_GRADIENT_CENTER_X));
    }

    if (attrs.hasAttribute(tag_GRADIENT_CENTER_Y)) {
        setGradientCenterY(attrs.getFloatAttr(tag_GRADIENT_CENTER_Y));
    }
}

void sf::ShapeDrawable::drawRectangle(SkCanvas* canvas) {
    SkPath path;
    int size = mCorner.topLeft * 2;
    path.moveTo(0, size);
    if (size > 0) {
        path.arcTo(SkRect::MakeXYWH(
            0, 0, size, size), 180, 90, false);
    }

    size = mCorner.topRight * 2;
    path.lineTo(mSize.width - size, 0);
    if (size > 0) {
        path.arcTo(SkRect::MakeXYWH(
            mSize.width - size, 0, size, size), 270, 90, false);
    }

    size = mCorner.bottomRight * 2;
    path.lineTo(mSize.width, mSize.height - size);
    if (size > 0) {
        path.arcTo(SkRect::MakeXYWH(
            mSize.width - size, mSize.height - size,
            size, size), 0, 90, false);
    }

    size = mCorner.bottomLeft * 2;
    path.lineTo(size, mSize.height);
    if (size > 0) {
        path.arcTo(SkRect::MakeXYWH(
            0, mSize.height - size,
            size, size), 90, 90, false);
    }

    path.close();

    SkPaint paint;
    paint.setAntiAlias(true);
    if (mGradientType != GradientType::NoGradient) {
        drawGradientRectangle(canvas, path, paint);
    } else {
        drawSolidRectangle(canvas, path, paint);
    }

    if (mStrokeWidth > 0) {
        paint.setColor(mStrokeColor);
        paint.setStrokeWidth(mStrokeWidth);
        paint.setStyle(SkPaint::Style::kStroke_Style);
        canvas->drawPath(path, paint);
    }
}

void sf::ShapeDrawable::drawSolidRectangle(
    SkCanvas* canvas, SkPath& path, SkPaint& paint) {

    paint.setColor(mSolidColor);
    canvas->drawPath(path, paint);
}

void sf::ShapeDrawable::drawGradientRectangle(
    SkCanvas* canvas, SkPath& path, SkPaint& paint) {

    SkShader* shader = createGradientShader();
    paint.setShader(shader);
    canvas->drawPath(path, paint);
    shader->unref();
}

SkShader* sf::ShapeDrawable::createGradientShader() const {
    int count = 2;
    bool hasCenter = mGradientCenterColor != -1;
    if (hasCenter) {
        ++count;
    }

    SkColor* colors = new SkColor[count];
    SkScalar* pos = nullptr;
    SkPoint* pts = new SkPoint[count];

    colors[0] = mGradientStartColor;
    if (hasCenter) {
        colors[1] = mGradientCenterColor;
        colors[2] = mGradientEndColor;
    } else {
        colors[1] = mGradientEndColor;
    }

    SkPoint center = SkPoint::Make(
        mSize.width * mGradientCenterX, mSize.height * mGradientCenterY);
    SkShader::TileMode tileMode = SkShader::TileMode::kRepeat_TileMode;

    SkShader* shader;
    if (mGradientType == GradientType::Radial) {
        int radius = mGradientRadius;
        if (radius <= 0) {
            radius = std::min(mSize.width, mSize.height) / 2;
        }

        shader = SkGradientShader::CreateRadial(
            center, radius, colors, pos, count, tileMode);
    } else if (mGradientType == GradientType::Sweep) {
        shader = SkGradientShader::CreateSweep(
            center.x(), center.y(), colors, pos, count);
    } else {
        if (mGradientAngleDegree >= 90 && mGradientAngleDegree < 180) {
            pts[0] = SkPoint::Make(0, 0);
            pts[1] = SkPoint::Make(0, mSize.height);
        } else if (mGradientAngleDegree >= 180 && mGradientAngleDegree < 270) {
            pts[0] = SkPoint::Make(mSize.width, 0);
            pts[1] = SkPoint::Make(0, 0);
        } else if (mGradientAngleDegree >= 270 && mGradientAngleDegree < 360) {
            pts[0] = SkPoint::Make(0, mSize.height);
            pts[1] = SkPoint::Make(0, 0);
        } else {
            pts[0] = SkPoint::Make(0, 0);
            pts[1] = SkPoint::Make(mSize.width, 0);
        }

        shader = SkGradientShader::CreateLinear(
            pts, colors, pos, count, tileMode);
    }

    delete[] colors;
    delete[] pos;
    delete[] pts;

    return shader;
}

void sf::ShapeDrawable::drawOval(SkCanvas* canvas) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::Style::kFill_Style);
    const SkRect& rc = SkRect::MakeWH(mSize.width, mSize.height);

    if (mGradientType != GradientType::NoGradient) {
        SkShader* shader = createGradientShader();
        paint.setShader(shader);
        canvas->drawOval(rc, paint);
        shader->unref();
    } else {
        paint.setColor(mSolidColor);
        canvas->drawOval(rc, paint);
    }

    if (mStrokeWidth > 0) {
        paint.setColor(mStrokeColor);
        paint.setStrokeWidth(mStrokeWidth);
        paint.setStyle(SkPaint::Style::kStroke_Style);

        canvas->drawOval(rc, paint);
    }
}
