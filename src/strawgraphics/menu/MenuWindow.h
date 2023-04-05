/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/29
 */

#pragma once

#include "graphics/window/Window.h"


namespace sf {

    class MenuWindow : public ElementInherit<MenuWindow, Window> {

    public:
        MenuWindow(const Context& context);
        virtual ~MenuWindow();

    protected:

    private:
    };
}
