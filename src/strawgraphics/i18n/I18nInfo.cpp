#include "graphics/i18n/I18nInfo.h"

#include "base/ptr/RefPtr.h"
#include "base/data/Buffer.h"
#include "base/StrawUtils.h"
#include "base/file/FileUtils.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/context/Context.h"


namespace {

    const std::string tag_STRING = "string";
    const char* const tag_STRING_NAME = "name";

} // anonymous namespace ends here.


sf::I18nInfo::I18nInfo(Context& context, const std::string& languageCode)
    : m_languageCode(languageCode) {

    loadInfo(context);
}

sf::I18nInfo::~I18nInfo() {
}

const std::string& sf::I18nInfo::getLanguageCode() const {
    return m_languageCode;
}

const std::string& sf::I18nInfo::getString(const std::string& name) const {
    auto iter = m_valueList.find(name);
    if (iter != m_valueList.end()) {
        return iter->second;
    }

    return m_invalidValue;
}

bool sf::I18nInfo::isLanguage(const std::string& languageCode) const {
    return m_languageCode == languageCode;
}

void sf::I18nInfo::loadInfo(Context& context) {
    std::string matchCode = "-" + m_languageCode;
    
    auto list = context.listFiles("string",
        [&matchCode](const std::string& fileName)->bool {
            return fileName.rfind(matchCode)
                == fileName.size() - matchCode.size();
        });

    for (auto& file : list) {
        std::string fileName = "@" + file;
        std::unique_ptr<ElementNode> node(context.getElementNode(fileName));
        if (!node) {
            continue;
        }

        if (node->mNodeName.value() != sf::config::RESOURCE_ROOT_TAG) {
            straw_warning("values(styles/strings/colors...) "
                "XML resource must started with root resources: " + fileName);
            continue;
        }

        for (Serializable* sub : node->mChildren) {
            ElementNode* child = reinterpret_cast<ElementNode*>(sub);
            if (child->mNodeName.value() == tag_STRING) {
                auto name = child->mAttributes.getRawStringAttr(tag_STRING_NAME);
                if (!name.empty()) {
                    m_valueList[name] = child->mNodeText;
                }
            }
        }
    }
}
