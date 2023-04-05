#include <Windows.h>
#include <algorithm>
#include <windowsx.h>

#include "base/def/FrameworkDef.h"
#include "base/def/PlatformDefine.h"
#include "base/def/FrameworkDef.h"
#include "base/ptr/weak_ptr.h"
#include "base/ptr/RefPtr.h"
#include "base/data/NumericSerializable.h"
#include "base/loop/Looper.h"
#include "base/data/Array.h"
#include "base/log/StrawLog.h"
#include "graphics/app/BaseApplication.h"
#include "graphics/window/Window.h"
#include "graphics/context/Context.h"
#include "graphics/view/TextView.h"
#include "graphics/view/event/ViewEvent.h"
#include "graphics/app/StrawApplication.h"
#include "graphics/window/MessageBox.h"
#include "graphics/window/CaptionWindow.h"
#include "graphics/theme/Theme.h"
#include "graphics/window/MessageBoxWindow.h"
#include "graphics/app/ThreadManager.h"
#include "graphics/menu/Menu.h"


int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd) {

    //logFinal("App", "App Start! %d", 1);

    sf::StrawApplication app;
    app.initialize(lpCmdLine);

    sf::ref_ptr<sf::Window> wnd = sf::Window::createByFile(
        app.getContext(), "@layout/main_wnd");

    sf::ref_ptr<sf::View> login = wnd->findViewById("login");
    if (login) {
        login->addOnClickListener([&app, &wnd](sf::View*) {
            sf::msgbox::showMsgBox(
                app.getContext(), sf::msgbox::Information,
                "Hello", "Hello Message Box!",
                "Cancel", "OK",
                sf::msgbox::dismissListener(),
                [&app, wnd](sf::MessageBoxWindow* box)->void {
                box->dismiss();
                sf::msgbox::showSingleBtnMsgBox(app.getContext(), sf::msgbox::Warning,
                    "Single", "OK", sf::msgbox::dismissListener(),
                    wnd);
            }, wnd);
        });
    }
    
    //wnd->setCloseListener(sf::OnCloseListener(fn));
    wnd->addListener(sf::WindowListener::create(&sf::WindowListener::onClosed, [&app]() {
        app.terminate();
    }));

    //wnd->setTitle(sf::Context::getContext().getString("app_name"));
    //wnd->setSize(600, 400);
    wnd->show();

    sf::ref_ptr<sf::TextView> text = (sf::TextView*) wnd->findViewById("text_view");
    //text->setText(wnd->getTitle());
    wnd->addNativeMsgMonitor(sf::NativeWindowMessageMonitor::create(
        &sf::NativeWindowMessageMonitor::onHandleNativeMessage,
        [&app, &wnd](sf::NativeWindowMessage* msg) {

        if (msg->message == WM_RBUTTONDOWN) {
            sf::ref_ptr<sf::Menu> menu = wnd->trackPopupMenu("@menu/main_menu");
            menu->addListener(sf::MenuListener::create(
                &sf::MenuListener::onMenuItemClicked, [&app, &wnd](sf::BaseMenuItem* item) {
                sf::msgbox::showSingleBtnMsgBox(
                    app.getAppContext(), sf::msgbox::IconType::Warning,
                    item->mId,
                    "OK",
                    sf::msgbox::dismissListener(),
                    wnd);
            }));
        }
    }));

    sf::weak_ptr<sf::Window> weak = wnd;
    sf::Window* get = weak.get();

    sf::StrawApplication::getInstance().run();

    wnd.reset();
    get = weak.get();

    return 0;
}
