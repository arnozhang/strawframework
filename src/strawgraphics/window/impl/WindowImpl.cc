#include "graphics/window/impl/WindowImpl.h"

#include "base/StrawUtils.h"


sf::WindowImpl::WindowImpl(const Context& context, sf::WindowHost& listener)
    : BaseWindow(context)
    , mHost(listener) {
}

sf::WindowImpl::~WindowImpl() {
}

void sf::WindowImpl::setIcon(Buffer* content, bool large) {
}

void sf::WindowImpl::setCursor(Buffer* content) {
}

void sf::WindowImpl::setSizeable(SizeableType type) {
}

void sf::WindowImpl::setSystemCursor(SystemCursorType type) {
}
void sf::WindowImpl::setCenterInScreen(bool center) {
    if (center) {
        Size size = getSize();
        Size screenSize = sf::getWorkZoneScreenSize();
        setPosition(
            (screenSize.width - size.width) / 2,
            (screenSize.height - size.height) / 2);
    }
}
