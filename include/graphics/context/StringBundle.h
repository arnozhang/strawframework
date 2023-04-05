/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <string>

#include "graphics/i18n/I18nInfo.h"


namespace sf {

    class StringBundle {

    public:
        StringBundle(Context& context);
        ~StringBundle();

        const std::string& getString(const std::string& name);

    private:
        Context& mContext;
        I18nInfo* mCurrI18nInfo;
        std::map<std::string, I18nInfo*> mI18nList;
    };

}
