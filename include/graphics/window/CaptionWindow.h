/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/21
 */

#pragma once

#include "graphics/window/Window.h"


namespace sf {

    class STRAW_FRAMEWORK_API CaptionWindow
        : public ElementInherit<CaptionWindow, Window> {

        IMPL_ELEMENT(CaptionWindow);

    public:
        CaptionWindow(const Context& context);
        virtual ~CaptionWindow();

        virtual void setIcon(const std::string& resPath, bool large) override;
        virtual Rect getWindowShadow() const override;
        virtual void setMinimizable(bool enable) override;
        virtual void setMaximizable(bool enable) override;

    protected:
        virtual HitTestZone onHitTest(int x, int y) override;
        virtual void createInternal(Attributes* attrs) override;
        virtual void onSized(int width, int height) override;

    private:
        ref_ptr<View> mMinBtn;
        ref_ptr<View> mMaxBtn;
        ref_ptr<View> mCloseBtn;
        ref_ptr<View> mIconView;
        ref_ptr<View> mCaptionView;

        bool mIsMaximized;
        std::string mMaxBtnBkg;
        std::string mRestoreBtnBkg;
    };

}
