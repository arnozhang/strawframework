#include "graphics/window/CaptionWindow.h"

#include "graphics/GraphicsDeclare.h"
#include "graphics/view/ImageView.h"
#include "graphics/View/Attributes.h"
#include "graphics/context/Context.h"
#include "graphics/context/LayoutInflater.h"

#include "DefaultCaptionView.h"


sf::CaptionWindow::CaptionWindow(const Context& context)
    : ElementInherit(context) {
    mIsMaximized = false;
}

sf::CaptionWindow::~CaptionWindow() {
}

void sf::CaptionWindow::setIcon(const std::string& resPath, bool large) {
    super::setIcon(resPath, large);

    if (mIconView) {
        Drawable* drawable = mContext.getDrawable(resPath);
        if (drawable) {
            mIconView->setBackground(drawable);
        }
    }
}

sf::Rect sf::CaptionWindow::getWindowShadow() const {
    Rect shadow = Window::getWindowShadow();
    Rect captionShadow = mCaptionView->getPadding();
    shadow.top = captionShadow.top;
    return shadow;
}

void sf::CaptionWindow::setMinimizable(bool enable) {
    super::setMinimizable(enable);
}

void sf::CaptionWindow::setMaximizable(bool enable) {
    super::setMaximizable(enable);
}

void sf::CaptionWindow::createInternal(Attributes* attrs) {
    LayoutInflater& inflater = mContext.getLayoutInflater();
    inflater.inflate("@platform:layout/default_caption_window", mRootView, true);

    ref_ptr<ViewGroup> captionRoot = (ViewGroup*) findViewById(sf::window::id_wndCaptionRoot);
    if (attrs && attrs->hasAttribute(sf::window::attr_wndCaptionLayout)) {
        std::string captionRes = attrs->getRawStringAttr(
            sf::window::attr_wndCaptionLayout);
        if (!captionRes.empty()) {
            mCaptionView = inflater.inflate(captionRes, captionRoot, true);
        }
    }

    if (!mCaptionView) {
        mCaptionView = new sf::impl::DefaultCaptionView(mContext);
        captionRoot->addView(mCaptionView);
    }

    Attributes* captionStyle = nullptr;
    if (attrs && attrs->hasAttribute(sf::window::attr_wndCaptionStyle)) {
        captionStyle = attrs->getStyleAttr(
            sf::window::attr_wndCaptionStyle);
    }

    if (!captionStyle) {
        captionStyle = mContext.getStyle("@platform:style/default_caption");
    }

    mMaxBtnBkg = captionStyle->getRawStringAttr(sf::window::attr_maxBtnDrawable);
    mRestoreBtnBkg = captionStyle->getRawStringAttr(sf::window::attr_restoreBtnDrawable);

    mCaptionView->setAttributes(captionStyle);

    mMinBtn = findViewById(sf::window::id_wndMinBtn);
    mMaxBtn = findViewById(sf::window::id_wndMaxBtn);
    mCloseBtn = findViewById(sf::window::id_wndCloseBtn);
    mIconView = findViewById(sf::window::id_wndIcon);

    if (mMinBtn) {
        mMinBtn->addOnClickListener([this](View*) -> void {
            minWindow();
        });
    }

    if (mMaxBtn) {
        mMaxBtn->addOnClickListener([this](View*) -> void {
            maxOrResotreWindow();
        });
    }

    if (mCloseBtn) {
        mCloseBtn->addOnClickListener([this](View*) -> void {
            close();
        });
    }
}

void sf::CaptionWindow::onSized(int width, int height) {
    Window::onSized(width, height);

    bool maximized = isMaximized();
    if (maximized == mIsMaximized) {
        return;
    }

    mIsMaximized = maximized;
    mMaxBtn->setBackground(mContext.getDrawable(
        mIsMaximized ? mRestoreBtnBkg : mMaxBtnBkg));
}

sf::HitTestZone sf::CaptionWindow::onHitTest(int x, int y) {
    HitTestZone zone = Window::onHitTest(x, y);

    if (zone == HitTestZone::Client || zone == HitTestZone::None) {
        View* caption = findViewById(sf::window::id_wndCaptionRoot);
        if (caption) {
            x -= caption->getXPosition();
            y -= caption->getYPosition();
            if (x >= 0 && x <= caption->getWidth()
                && y >= 0 && y <= caption->getHeight()) {

                if (mMinBtn && mMinBtn->getGlobalRect().contains(x, y)) {
                    return HitTestZone::MinButton;
                }

                if (mMaxBtn && mMaxBtn->getGlobalRect().contains(x, y)) {
                    return HitTestZone::MaxButton;
                }

                if (mCloseBtn && mCloseBtn->getGlobalRect().contains(x, y)) {
                    return HitTestZone::CloseButton;
                }

                if (mIconView && mIconView->getGlobalRect().contains(x, y)) {
                    return HitTestZone::SysMenu;
                }

                return HitTestZone::CaptionBar;
            }
        }
    }

    return zone;
}
