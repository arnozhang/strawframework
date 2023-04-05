/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "graphics/layout/LinearLayout.h"


namespace sf {

    namespace impl {

        class DefaultCaptionView
            : public ElementInherit<DefaultCaptionView, LinearLayout> {

            IMPL_ELEMENT(DefaultCaptionView);

        public:
            DefaultCaptionView(const Context& context);
            virtual ~DefaultCaptionView();

        protected:
            virtual void setAttributesInternal(Attributes* attrs) override;
        };
    }
}
