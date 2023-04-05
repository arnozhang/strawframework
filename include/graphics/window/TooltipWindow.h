/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/25
 */

#pragma once

#include "graphics/window/Window.h"


namespace sf {

    class STRAW_FRAMEWORK_API TooltipWindow
        : public ElementInherit<TooltipWindow, Window> {

        IMPL_ELEMENT(TooltipWindow);

    public:
        TooltipWindow(const Context& context);
        virtual ~TooltipWindow();

    protected:
        virtual void createInternal(Attributes* attrs) override;
    };

}
