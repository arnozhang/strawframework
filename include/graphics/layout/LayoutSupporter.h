/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/ptr/RefPtrObject.h"
#include "graphics/layout/LayoutParams.h"


namespace sf {

    class STRAW_FRAMEWORK_API ViewGroup;
    class STRAW_FRAMEWORK_API LayoutParams;


    class STRAW_FRAMEWORK_API LayoutSupporter : public RefCountObject {

    public:
        LayoutSupporter();
        virtual ~LayoutSupporter();

        virtual LayoutType getLayoutType() const = 0;
        virtual LayoutParams* createDefaultLayoutParams(
            LayoutParams* params = 0) = 0;

        virtual void layout(ViewGroup* viewGroup) = 0;

    private:
    };
}
