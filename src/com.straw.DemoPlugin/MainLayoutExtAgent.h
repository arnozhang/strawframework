#pragma once

#include "graphics/extension/Extension.h"
#include "graphics/extension/Extension.h"


class MainLayoutExtAgent : public sf::ExtensionAgent {

public:
    MainLayoutExtAgent();
    virtual ~MainLayoutExtAgent();

    virtual void getProperties(const std::string& extensionName) override;
    virtual void extension(const std::string& extensionName, sf::ViewGroup* parent) override;
};
