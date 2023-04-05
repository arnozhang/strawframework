/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/TextView.h"


namespace sf {

    class STRAW_FRAMEWORK_API Button
        : public ElementInherit<Button, TextView> {

        IMPL_ELEMENT(Button);

    public:
        Button(const Context& context);
        virtual ~Button();

    private:
    };
}
