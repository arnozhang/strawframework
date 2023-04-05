#include "graphics/drawable/AnimationDrawable.h"

#include "base/data/Serializable.h"
#include "graphics/view/View.h"
#include "graphics/view/AttributesHelper.h"
#include "graphics/context/ElementNode.h"


namespace {

    const char* const tag_Item = "item";
    const char* const tag_Drawable = "drawable";
    const char* const tag_Duration = "duration";

} // anonymous namespace ends here.


sf::AnimationDrawable::AnimationDrawable(const Context& context)
    : ElementInherit(context) {
    mCurrFrameIndex = 0;
    mScheduleId = sf::INVALID_ID;
    mRepeat = true;
}

sf::AnimationDrawable::~AnimationDrawable() {
}

void sf::AnimationDrawable::setAttributes(Attributes* attrs) {
    BEGIN_ATTRS(attrs)
        ATTR_BOOL(repeat, setRepeat)
    END_ATTRS()
}

void sf::AnimationDrawable::addFrame(Drawable* drawable, int duration) {
    if (duration < 0) {
        straw_warning("Can not add frame by duration less than 0!");
        return;
    }

    mDrawableFrames.push_back(DrawableFrame(drawable, duration));
}

void sf::AnimationDrawable::setRepeat(bool repeat) {
    mRepeat = repeat;
}

void sf::AnimationDrawable::onVisibilityChanged() {
    mCurrFrameIndex = 0;

    if (!mVisible) {
        Looper::mainLooper().cancelTask(mScheduleId);
        mScheduleId = sf::INVALID_ID;
    } else {
        scheduleNextFrame();
    }
}

void sf::AnimationDrawable::scheduleNextFrame() {
    if (mDrawableFrames.empty() || !mAttachedView || !mVisible) {
        return;
    }

    if (mScheduleId != sf::INVALID_ID) {
        return;
    }

    if (mCurrFrameIndex >= mDrawableFrames.size()) {
        if (mRepeat) {
            mCurrFrameIndex = 0;
        } else {
            return;
        }
    }

    auto& frame = mDrawableFrames[mCurrFrameIndex];
    mAttachedView->invalidateSelf(true);

    mScheduleId = Looper::mainLooper().postTaskDelay([this]() {
        mScheduleId = sf::INVALID_ID;
        ++mCurrFrameIndex;

        if (mDrawableFrames.size() > 1) {
            scheduleNextFrame();
        }
    }, (uint64_t) frame.duration);
}

sf::Drawable* sf::AnimationDrawable::getCurrentDrawable() {
    auto& frame = mDrawableFrames[mCurrFrameIndex];
    return frame.drawable.get();
}

void sf::AnimationDrawable::draw(SkCanvas* canvas) {
    if (mDrawableFrames.empty()) {
        return;
    }

    if (mCurrFrameIndex < 0 || mCurrFrameIndex >= mDrawableFrames.size()) {
        mCurrFrameIndex = 0;
    }

    Drawable* drawable = getCurrentDrawable();
    if (drawable) {
        drawable->draw(canvas);
    }
}

void sf::AnimationDrawable::decodeByElementNode(ElementNode* node) {
    for (Serializable* iter : node->mChildren) {
        ElementNode* sub = reinterpret_cast<ElementNode*>(iter);
        const std::string& name = sub->mNodeName;
        Attributes& attrs = sub->mAttributes;

        if (name == tag_Item) {
            if (attrs.hasAttribute(tag_Drawable) && attrs.hasAttribute(tag_Duration)) {
                int duration = attrs.getIntAttr(tag_Duration);
                if (duration > 0) {
                    Drawable* drawable = attrs.getDrawableAttr(tag_Drawable);
                    addFrame(drawable, duration);
                }
            }
        } else {
            straw_warning("Invalid child item \"" + name
                          + "\" when decode AnimationDrawable.");
        }
    }
}

void sf::AnimationDrawable::setSize(Size size) {
    Drawable::setSize(size);

    for (auto& frame : mDrawableFrames) {
        if (frame.drawable) {
            frame.drawable->setSize(size);
        }
    }
}

void sf::AnimationDrawable::setVisible(bool visible) {
    Drawable::setVisible(visible);

    for (auto& frame : mDrawableFrames) {
        if (frame.drawable) {
            frame.drawable->setVisible(visible);
        }
    }
}

sf::Size sf::AnimationDrawable::getRawResourceSize() const {
    size_t count = mDrawableFrames.size();
    if (count > 0 && mCurrFrameIndex >= 0 && mCurrFrameIndex < count) {
        AnimationDrawable* visit = const_cast<AnimationDrawable*>(this);
        auto drawable = visit->getCurrentDrawable();
        if (drawable) {
            return drawable->getRawResourceSize();
        }
    }

    return super::getRawResourceSize();
}
