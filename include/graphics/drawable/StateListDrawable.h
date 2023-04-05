/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "graphics/drawable/Drawable.h"


namespace sf {

    class STRAW_FRAMEWORK_API StateListDrawable
        : public ElementInherit<StateListDrawable, Drawable> {

        IMPL_ELEMENT(StateListDrawable);

    public:
        StateListDrawable(const Context& context);
        virtual ~StateListDrawable();

        virtual void draw(SkCanvas* canvas) override;
        virtual void decodeByElementNode(ElementNode* node) override;
        virtual Size getRawResourceSize() const override;

        void addDrawable(int state, Drawable* drawable);
        Drawable* getDrawable(int state) const;

    private:
        struct StateDrawableItem {
            StateDrawableItem(int _state, Drawable* _drawable) {
                state = _state;
                drawable = _drawable;
            }

            int state;
            sf::ref_ptr<Drawable> drawable;
        };


        std::vector<StateDrawableItem> mDrawableMap;
    };
}
