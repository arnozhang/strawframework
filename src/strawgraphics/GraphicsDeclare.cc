#include "graphics/GraphicsDeclare.h"

#include "graphics/view/View.h"


sf::ViewUpdateLock::ViewUpdateLock(
    View* view, DoWhenUnlock whenUnlock /*= DoWhenUnlock::ReLayout*/) {

    mView = view;
    mDoWhenUnlock = whenUnlock;
    if (mView) {
        mView->lockUpdate();
    }
}

sf::ViewUpdateLock::~ViewUpdateLock() {
    if (mView) {
        mView->unLockUpdate();
        if (!mView->isViewLocked()) {
            if (mDoWhenUnlock == DoWhenUnlock::Invalidate) {
                mView->invalidateSelf(true);
            } else if (mDoWhenUnlock == DoWhenUnlock::ReLayout) {
                mView->requestLayout();
            }
        }
    }
}

bool sf::isImageExtension(const std::string& extension) {
    return extension == sf::extension::PNG
        || extension == sf::extension::JPG
        || extension == sf::extension::BMP
        || extension == sf::extension::JPEG
        || extension == sf::extension::GIF
        || extension == sf::extension::WEBP;
}
