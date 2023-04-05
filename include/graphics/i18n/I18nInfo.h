/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>

#include "base/def/PlatformDefine.h"


namespace sf {

    class STRAW_FRAMEWORK_API Context;


    class I18nInfo {

    public:
        I18nInfo(Context& context, const std::string& languageCode);
        ~I18nInfo();

        const std::string& getLanguageCode() const;
        const std::string& getString(const std::string& name) const;
        bool isLanguage(const std::string& languageCode) const;


    private:
        void loadInfo(Context& context);


    private:
        std::string m_languageCode;
        std::string m_invalidValue;
        std::map<std::string, std::string> m_valueList;
    };

}
