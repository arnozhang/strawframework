#include "graphics/context/StringBundle.h"

#include "graphics/app/StrawApplication.h"


sf::StringBundle::StringBundle(Context& context)
    : mContext(context){

    mCurrI18nInfo = nullptr;
}

sf::StringBundle::~StringBundle() {
    for (auto& iter : mI18nList) {
        delete iter.second;
    }
}

const std::string& sf::StringBundle::getString(const std::string& name) {
    std::string code = StrawApplication::getInstance().getLanguageCode();
    if (!mCurrI18nInfo || !mCurrI18nInfo->isLanguage(code)) {
        auto iter = mI18nList.find(code);
        if (iter != mI18nList.end()) {
            mCurrI18nInfo = iter->second;
        } else {
            I18nInfo* pInfo = new I18nInfo(mContext, code);
            mI18nList[code] = pInfo;
            mCurrI18nInfo = pInfo;
        }
    }

    return mCurrI18nInfo->getString(name);
}

