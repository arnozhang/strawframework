#include "graphics/window/MessageBox.h"

#include "graphics/theme/Theme.h"
#include "graphics/context/Context.h"
#include "graphics/window/MessageBoxWindow.h"


sf::MessageBoxWindow* sf::msgbox::showMsgBox(
    const Context& context,
    IconType iconType,
    const std::string& title,
    const std::string& content,
    const std::string& leftBtnText,
    const std::string& rightBtnText,
    MsgBoxBtnListener leftListener,
    MsgBoxBtnListener rightListener,
    Window* parent /*= nullptr*/,
    MsgBoxDismissListener* dismissListener /*= nullptr*/,
    const char* msgBoxLayoutRes /*= nullptr*/) {

    ref_ptr<MessageBoxWindow> wnd = new MessageBoxWindow(context);
    std::unique_ptr<Attributes> attrs = std::make_unique<Attributes>(context);
    attrs->mergeStyles(context.getTheme().getThemeByTarget(CaptionWindow::getElemenetName()));

    if (msgBoxLayoutRes) {
        attrs->addAttr(msgbox::layout::attr_msgBoxLayout, msgBoxLayoutRes);
    }

    ViewUpdateLock lock(wnd->getRootView());
    wnd->create(parent, attrs.get(), true);
    wnd->setAttributes(attrs.get());
    if (dismissListener) {
        wnd->addListener(WindowListener::create(
            &WindowListener::onClosed, [dismissListener, &wnd]() {
            (*dismissListener)(wnd);
        }));
    }

    wnd->setIconType(iconType);
    wnd->setContentText(content);
    wnd->setLeftBtnText(leftBtnText);
    wnd->setRightBtnText(rightBtnText);
    wnd->setLeftBtnClickListener(leftListener);
    wnd->setRightBtnClickListener(rightListener);
    wnd->setMinimizable(false);
    wnd->setMaximizable(false);

    wnd->setTitle(title);
    wnd->setSize(400, 200);
    wnd->setCenterInScreen(true);
    wnd->show();
    return wnd;
}

sf::MessageBoxWindow* sf::msgbox::showSingleBtnMsgBox(
    const Context& context,
    IconType iconType,
    const std::string& content,
    const std::string& btnText,
    MsgBoxBtnListener btnListener,
    Window* parent /*= nullptr*/,
    MsgBoxDismissListener* dismissListener /*= nullptr*/) {

    MessageBoxWindow* wnd = showMsgBox(
        context, iconType, "", content,
        "", btnText,
        sf::msgbox::dismissListener(), btnListener,
        parent, dismissListener, nullptr);

    wnd->setSingleBtn(true);
    return wnd;
}

sf::msgbox::MsgBoxBtnListener sf::msgbox::dismissListener() {
    return [](MessageBoxWindow* wnd) -> void {
        wnd->dismiss();
    };
}
