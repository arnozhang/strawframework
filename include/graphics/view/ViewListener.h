/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/28
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/listener/ListenerBinder.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;


    class STRAW_FRAMEWORK_API ViewListener : public ListenerBinder<ViewListener> {

    public:
        virtual void onClicked(View* view) {
            INVOKE(onClicked, view);
        }

        virtual void onMouseHover() {
            INVOKE(onMouseHover);
        }

        virtual void onMouseLeave() {
            INVOKE(onMouseLeave);
        }
    };

}
