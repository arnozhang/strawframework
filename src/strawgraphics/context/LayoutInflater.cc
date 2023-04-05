#include "graphics/context/LayoutInflater.h"

#include <memory>

#include "base/StrawUtils.h"
#include "base/file/FileUtils.h"
#include "graphics/app/CustomizeViewManager.h"
#include "graphics/view/ViewGroup.h"
#include "graphics/window/Window.h"
#include "graphics/GraphicsDeclare.h"
#include "graphics/view/Attributes.h"
#include "graphics/theme/Theme.h"
#include "graphics/context/Context.h"
#include "graphics/context/ElementNode.h"
#include "graphics/menu/Menu.h"

#include "thirdparty/tinyxml2/tinyxml2.h"


namespace {

    const char* const tag_INCLUDE = "include";
    const char* const tag_MERGE = "merge";

} // anonymous namespace ends here.


sf::LayoutInflater::LayoutInflater(const Context& context)
    : mContext(context) {
}

sf::LayoutInflater::~LayoutInflater() {
}

sf::View* sf::LayoutInflater::inflate(const std::string& res) {
    return inflate(res, nullptr);
}

sf::View* sf::LayoutInflater::inflate(const std::string& res, ViewGroup* parent) {
    return inflate(res, parent, false);
}

sf::View* sf::LayoutInflater::inflate(
    const std::string& res, ViewGroup* parent, bool attachToParent) {

    std::unique_ptr<ElementNode> node(mContext.getElementNode(res));
    if (!node) {
        return nullptr;
    }

    ViewUpdateLock lock(parent, attachToParent
                                ? ViewUpdateLock::DoWhenUnlock::ReLayout
                                : ViewUpdateLock::DoWhenUnlock::Nothing);
    View* view = inflate(node.get(), parent, attachToParent);
    if (view != parent && view && parent && attachToParent && !view->getParent()) {
        parent->addView(view);
    }

    return view;
}

sf::View* sf::LayoutInflater::inflate(
    ElementNode* node, ViewGroup* parent, bool attachToParent) {

    if (!node->valid()) {
        return nullptr;
    }

    if (node->mNodeName.value() == tag_INCLUDE) {
        // <include layout="@layout/xxx" />
        //
        if (!node->mAttributes.hasAttribute(sf::layout::attr_layout)) {
            straw_warning("include tag must declare layout: layout=\"@layout/xxx\"!");
            return nullptr;
        }

        std::unique_ptr<ElementNode> include(
            node->mAttributes.getElementNodeAttr(sf::layout::attr_layout));
        if (!include) {
            straw_warning("Cannot get/parse include layout XML!");
            return nullptr;
        }

        View* view = inflate(include.get(), parent, attachToParent);

        parent->setAttributes(&node->mAttributes);
        return view;
    } else if (node->mNodeName.value() == tag_MERGE) {
        // <merge>
        //      ...
        // </merge>
        //
        for (Serializable* child : node->mChildren) {
            inflate(reinterpret_cast<ElementNode*>(child), parent, true);
        }

        parent->setAttributes(&node->mAttributes);
        return parent;
    }

    sf::ElementCreator creator = CustomizeViewManager::getInstance()
        .getViewCreator(node->mNodeName);
    if (!creator) {
        straw_warning("\"" + node->mNodeName.value() + "\" Creator not defined!");
        return nullptr;
    }

    ref_ptr<Element> element = creator(mContext);
    View* view = sf::element_cast<View>(element, ElementType::Element_View);
    if (!view) {
        straw_warning("Can not create element \"" + node->mNodeName.value()
                      + "\" as View! Will skip it.");
        delete element;
        return nullptr;
    }

    if (parent) {
        LayoutParams* params = parent->createDefaultLayoutParams(
            &view->getLayoutParams());
        params->setAttributes(&node->mAttributes);
        view->setLayoutParams(params);

        if (attachToParent) {
            parent->addView(view);
        }
    }

    view->setAttributes(&node->mAttributes);
    if (view->canAddChild()) {
        ViewGroup* group = reinterpret_cast<ViewGroup*>(view);
        for (Serializable* child : node->mChildren) {
            inflate(reinterpret_cast<ElementNode*>(child), group, true);
        }

        std::string tag(sf::layout::attr_extensionName);
        if (node->mAttributes.hasAttribute(tag)) {
            std::string extensionName = node->mAttributes.getStringAttr(tag);
            if (!extensionName.empty()) {
                group->setExtensionName(extensionName);
            }
        }
    }

    return view;
}

sf::Window* sf::LayoutInflater::inflateWindow(
    const std::string& res, Window* parent /*= nullptr*/) {

    std::unique_ptr<ElementNode> node(mContext.getElementNode(res));
    if (!node) {
        straw_warning("Can not inflate Window from file: " + res);
        return nullptr;
    }

    const std::string wndTag = node->mNodeName;
    sf::ElementCreator creator = CustomizeViewManager::getInstance()
        .getViewCreator(wndTag);
    if (!creator) {
        straw_warning("\"" + wndTag + "\" Creator not defined when create Window!");
        return nullptr;
    }

    Element* element = creator(mContext);
    Window* window = sf::element_cast<Window>(element, ElementType::Element_Window);
    if (!window) {
        straw_warning("Can not create element \"" + wndTag + "\" as Window!");
        delete element;
        return nullptr;
    }

    Attributes* styles = mContext.getTheme().getThemeByTarget(wndTag);
    node->mAttributes.mergeStyles(styles);

    ViewUpdateLock lock(window->getRootView());
    window->create(parent, &node->mAttributes);
    window->setAttributes(&node->mAttributes);

    if (!node->mChildren.empty()) {
        ElementNode* child = reinterpret_cast<ElementNode*>(*node->mChildren.begin());
        ViewGroup* contentRoot = window->getContentParent();
        View* content = inflate(child, contentRoot, true);
    }

    return window;
}

sf::Menu* sf::LayoutInflater::inflateMenu(const std::string& res, Window* window) {
    std::unique_ptr<ElementNode> node(mContext.getElementNode(res));
    if (!node) {
        straw_warning("Can not inflate Menu from file: " + res);
        return nullptr;
    }

    return inflateMenu(node.get(), window, nullptr);
}

sf::Menu* sf::LayoutInflater::inflateMenu(
    ElementNode* node, Window* window, Menu* parent) {

    const std::string menuTag = node->mNodeName;
    sf::ElementCreator creator = CustomizeViewManager::getInstance()
        .getViewCreator(menuTag);
    if (!creator) {
        straw_warning("\"" + menuTag + "\" Creator not defined when create Menu!");
        return nullptr;
    }

    Element* element = creator(mContext);
    Menu* menu = sf::element_cast<Menu>(element, ElementType::Element_Menu);
    if (!menu) {
        straw_warning("Can not create element \"" + menuTag + "\" as Menu!");
        delete element;
        return nullptr;
    }

    Attributes* styles = mContext.getTheme().getThemeByTarget(menuTag);
    node->mAttributes.mergeStyles(styles);

    Attributes* attrs = new Attributes(std::move(node->mAttributes));
    if (parent) {
        attrs->mergeStyles(parent->getAttributes());
        window = parent->getWindow();
    }

    menu->attachWindow(window, parent);
    menu->setAttributes(attrs);

    for (Serializable* iter : node->mChildren) {
        ElementNode* item = reinterpret_cast<ElementNode*>(iter);
        const std::string& name = item->mNodeName;
        Attributes& itemAttrs = item->mAttributes;

        if (name == sf::config::RESOURCE_ITEM_TAG) {
            if (itemAttrs.hasAttribute(sf::menu::attr_isDivider)) {
                bool isDivider = itemAttrs.getBoolAttr(sf::menu::attr_isDivider);
                if (isDivider) {
                    BaseMenuItem* divider = new BaseMenuItem(mContext);
                    divider->createMenuItem();
                    divider->setAttributes(&itemAttrs);
                    menu->addItem(divider);
                    continue;
                }
            }

            MenuItem* menuItem = new MenuItem(mContext);
            menuItem->createMenuItem();
            menuItem->setAttributes(&itemAttrs);
            if (!item->mChildren.empty()) {
                ElementNode* sub = reinterpret_cast<ElementNode*>(item->mChildren[0]);
                Menu* subMenu = inflateMenu(sub, window, menu);
                menuItem->mSubMenu = subMenu;
            }

            menu->addItem(menuItem);
        }
    }

    return menu;
}
