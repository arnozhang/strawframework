#pragma once

#include "graphics/app/PluginApplication.h"

class DemoPluginApplication : public sf::PluginApplication {
    DECLARE_PLUGIN(DemoPluginApplication, "com.straw.DemoPlugin");

public:
    DemoPluginApplication();
    virtual ~DemoPluginApplication();

private:
};
