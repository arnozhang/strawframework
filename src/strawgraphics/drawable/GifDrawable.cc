#include "graphics/drawable/GifDrawable.h"

#include "base/timer/Timer.h"
#include "graphics/view/View.h"
#include "graphics/app/ThreadManager.h"

#include "thirdparty/skia/core/SkTime.h"


sf::GifDrawable::GifDrawable(
    const Context& context,
    SkMovie* movie /*= nullptr*/,
    bool needRelease /*= true*/)
    : ElementInherit(context)
    , mBitmapDrawable(context) {

    mNeedRelease = true;
    setMovie(movie, needRelease);
}

sf::GifDrawable::~GifDrawable() {
    releaseMovie();
}

void sf::GifDrawable::releaseMovie() {
    if (mNeedRelease) {
        if (mMovie) {
            mMovie->unref();
        }
    }

    mInitialized = false;
    mNeedRelease = true;
    mMovie = nullptr;
    mGifFrameTimes.clear();
}

void sf::GifDrawable::setMovie(SkMovie* movie, bool needRelease /*= true*/) {
    releaseMovie();

    mMovie = movie;
    mNeedRelease = needRelease;
    mInitialized = false;

    auto duration = mMovie->duration();
    if (duration > 0) {
        std::thread parser(&this_class::parseGifFrames, this, duration);
        parser.detach();
    } else {
        mMovie->setTime(0);
        mGifFrameTimes.push_back(0);

        mBitmapDrawable.setBitmap(&mMovie->bitmap(), false);
        addFrame(&mBitmapDrawable, 0);
        mInitialized = true;
    }
}

void sf::GifDrawable::parseGifFrames(int duration) {
    int timestamp = 0;
    for (int i = 0; i <= duration; ++i) {
        if (mMovie->setTime(i)) {
            if (timestamp > 0) {
                auto& bitmap = mMovie->bitmap();
                addFrame(&mBitmapDrawable, i - timestamp);
                mGifFrameTimes.push_back(i);
            }

            timestamp = i;
        }
    }

    ThreadManager::getInstance().mainLooper().postTask([this]() {
        mInitialized = true;
        if (mAttachedView) {
            scheduleNextFrame();
        }
    });
}

void sf::GifDrawable::draw(SkCanvas* canvas) {
    if (!mMovie || !mInitialized) {
        return;
    }

    super::draw(canvas);
}

sf::Drawable* sf::GifDrawable::getCurrentDrawable() {
    mMovie->setTime(mGifFrameTimes[mCurrFrameIndex]);
    mBitmapDrawable.setBitmap(&mMovie->bitmap(), false);

    return &mBitmapDrawable;
}
