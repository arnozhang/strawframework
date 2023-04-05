#include <Windows.h>

#include "DemoPluginApplication.h"
#include "graphics/app/StrawApplication.h"
#include "graphics/window/Window.h"


namespace {

    DemoPluginApplication* plugin = nullptr;

} // anonymous namespace ends here.


BOOL APIENTRY DllMain(
    void* _DllHandle, unsigned long _Reason, void* _Reserved) {

    switch (_Reason) {
    case DLL_PROCESS_ATTACH: {
//         if (!plugin) {
//             plugin = new DemoPluginApplication;
//             sf::StrawApplication::getInstance().addPlugin(plugin);
//             plugin->initialize();
//         }
// 
//         sf::ref_ptr<sf::Window> wnd = sf::Window::createByFile(
//             DemoPluginApplication::getInstance().getContext(),
//             "@layout/plugin_wnd");
//         if (wnd) {
//             wnd->show();
//         }
        break;
    }

    case DLL_PROCESS_DETACH:
        break;

    default:
        break;
    }

    return TRUE;
}
