/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/view/Attributes.h"


#define BEGIN_ATTRS(attrs) \
    { \
        if (!attrs) { \
            return; \
        } \
        auto __attrs = attrs; \

#define ATTR_STRING(title, fn) \
    ATTR_SET_PROC(title, fn, getStringAttr)

#define ATTR_RAW_STRING(title, fn) \
    ATTR_SET_PROC(title, fn, getRawStringAttr)

#define ATTR_INT(title, fn) \
    ATTR_SET_PROC(title, fn, getIntAttr)

#define ATTR_FLOAT(title, fn) \
    ATTR_SET_PROC(title, fn, getFloatAttr)

#define ATTR_DRAWABLE(title, fn) \
    ATTR_SET_PROC(title, fn, getDrawableAttr)

#define ATTR_DIMENSION(title, fn) \
    ATTR_SET_PROC(title, fn, getDimensionAttr)

#define ATTR_BOOL(title, fn) \
    ATTR_SET_PROC(title, fn, getBoolAttr)

#define ATTR_DIRECTION_X4(title, fn) \
    ATTR_SET_PROC(title, fn, getDirectionX4Attr)

#define ATTR_POINT(title, fn) \
    ATTR_SET_PROC(title, fn, getPointAttr)

#define ATTR_SIZE(title, fn) \
    ATTR_SET_PROC(title, fn, getSizeAttr)

#define ATTR_BITMAP(title, fn) \
    ATTR_SET_PROC(title, fn, getBitmapAttr)

#define ATTR_COLOR(title, fn) \
    if (__attrs->hasAttribute(#title) && !__attrs->isStateColor(#title)) { \
        fn(__attrs->getColorAttr(#title)); \
    } \

#define ATTR_STATE_COLOR(title, fn) \
    if (__attrs->hasAttribute(#title)) { \
        fn(__attrs->getStateColorAttr(#title)); \
    } \

#define ATTR_ENUM(title, fn, enum_type) \
    if (__attrs->hasAttribute(#title)) { \
        fn(static_cast<enum_type>(__attrs->getEnumAttr(#title))); \
    } \

#define ATTR_SET_PROC(title, fn, attrFn) \
    if (__attrs->hasAttribute(#title)) { \
        fn(__attrs->attrFn(#title)); \
    } \


#define END_ATTRS() \
    } \

