/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <memory>

#include "base/def/PlatformDefine.h"
#include "base/ptr/RefPtrObject.h"


namespace sf {

    typedef std::string LayoutType;

    class STRAW_FRAMEWORK_API Attributes;


    class STRAW_FRAMEWORK_API LayoutParams : public RefPtrObject {

    public:
        enum {
            MATCH_PARENT = -1,
            WRAP_CONTENT = -2
        };

    public:
        LayoutParams();
        LayoutParams(int _width, int _height);
        LayoutParams(const LayoutParams& rhs);
        LayoutParams(const LayoutParams* rhs);
        virtual ~LayoutParams();

        LayoutParams& operator=(const LayoutParams& rhs);

        virtual LayoutType getLayoutType() const;
        virtual void setAttributes(Attributes* attrs);

        void setWidth(int _width) {
            width = _width;
        }

        void setHeight(int _height) {
            height = _height;
        }

        void setMargin(const Directionx4& _margin) {
            margin = _margin;
        }

        bool isWidthMatchParent() const {
            return width == MATCH_PARENT;
        }

        bool isHeightMatchParent() const {
            return height == MATCH_PARENT;
        }

        bool isWidthWrapContent() const {
            return width == WRAP_CONTENT;
        }

        bool isHeightWrapContent() const {
            return height == WRAP_CONTENT;
        }

        bool isWidthSpecific() const {
            return width >= 0;
        }

        bool isHeightSpecific() const {
            return height >= 0;
        }

    public:
        int width;
        int height;
        Directionx4 margin;
    };
}