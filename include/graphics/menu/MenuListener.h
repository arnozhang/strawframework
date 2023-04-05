/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/29
 */

#pragma once

#include "base/listener/ListenerBinder.h"


namespace sf {

    class STRAW_FRAMEWORK_API Menu;
    class STRAW_FRAMEWORK_API MenuItem;


    class STRAW_FRAMEWORK_API MenuListener : public ListenerBinder<MenuListener> {

    public:
        virtual ~MenuListener() {
        }

        virtual void onMenuCreated(Menu* menu) {
            INVOKE(onMenuCreated, menu);
        }

        virtual void onMenuShown() {
            INVOKE(onMenuShown);
        }

        virtual void onMenuHidden() {
            INVOKE(onMenuHidden);
        }

        virtual void onMenuDestroyed() {
            INVOKE(onMenuDestroyed);
        }

        virtual void onMenuItemClicked(MenuItem* item) {
            INVOKE(onMenuItemClicked, item);
        }
    };
}
