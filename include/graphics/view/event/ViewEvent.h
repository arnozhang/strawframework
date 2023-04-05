/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/event/EventSink.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;
    class STRAW_FRAMEWORK_API Window;


    typedef EventSink<void(View*)> OnClickListener;
    typedef EventSink<void(Size, Size)> OnSizeChangedListener;
    typedef EventSink<void(Point, Point)> OnPositionChangedListener;
    typedef EventSink<void(Window*)> OnCloseListener;
    typedef EventSink<bool(int)> OnMessageListener;

    
}
