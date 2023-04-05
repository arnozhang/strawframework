#include "MainLayoutExtAgent.h"

#include "DemoPluginApplication.h"
#include "graphics/view/TextView.h"
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/ColorDrawable.h"


DECLARE_EXTENSION_AGENT(DemoPluginApplication::getPluginName(), MainLayoutExtAgent);


MainLayoutExtAgent::MainLayoutExtAgent() {

}

MainLayoutExtAgent::~MainLayoutExtAgent() {

}

void MainLayoutExtAgent::getProperties(const std::string& extensionName) {

}

void MainLayoutExtAgent::extension(
    const std::string& extensionName, sf::ViewGroup* parent) {

    sf::Context& context = DemoPluginApplication::getInstance().getContext();
    sf::TextView* view = new sf::TextView(context);
    view->setText(extensionName);
    view->setTextSize(20);
    view->setBottomPadding(10);
    view->setLayoutParams(new sf::LayoutParams(
        sf::LayoutParams::WRAP_CONTENT,
        sf::LayoutParams::WRAP_CONTENT));

    sf::StateListDrawable* drawable = new sf::StateListDrawable(context);
    drawable->addDrawable(sf::ViewState::Pressed,
        new sf::ColorDrawable(context, SK_ColorYELLOW));
    drawable->addDrawable(sf::ViewState::StateNormal,
        new sf::ColorDrawable(context, SK_ColorWHITE));
    view->setBackground(drawable);

    view->setClickable(true);
    view->addListener(sf::ViewListener::create(
        &sf::ViewListener::onClicked, [extensionName](sf::View*) -> void {
        ::MessageBoxA(NULL, extensionName.c_str(), "", MB_OK | MB_ICONINFORMATION);
    }));

    parent->addView(view);
}
