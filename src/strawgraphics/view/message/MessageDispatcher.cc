#include "graphics/view/message/MessageDispatcher.h"

#include "graphics/view/View.h"
#include "graphics/view/ViewGroup.h"


sf::MessageDispatcher::MessageDispatcher(View* view) {
    mView = view;
}

sf::MessageDispatcher::~MessageDispatcher() {
}

sf::BaseMessageResult* sf::MessageDispatcher::routeMessage(const BaseMessage& msg) {
    BaseMessageResult* result = nullptr;
    if (onMessage(msg, &result)) {
        return result;
    }

    return routeMessageInternal(msg);
}

void sf::MessageDispatcher::dispatchMessage(const BaseMessage& msg) {
    BaseMessageResult* result = nullptr;
    onMessage(msg, &result);
}

sf::BaseMessageResult* sf::MessageDispatcher::routeMessageInternal(
    const BaseMessage& msg) {

    if (msg.type > SM_MouseBegin && msg.type < SM_MouseEnd) {
        const MouseMessage& mouseMsg = reinterpret_cast<const MouseMessage&>(msg);
        return routeMouseMessage(mouseMsg);
    }

    return nullptr;
}

bool sf::MessageDispatcher::onMessage(
    const BaseMessage& msg, BaseMessageResult** result) {

    switch (msg.type) {
    case SM_LButtonClick:
        for (auto listener : mListeners) {
            listener->onClicked(mView);
        }
        return true;

    default:
        break;
    }

    return false;
}

sf::BaseMessageResult* sf::MessageDispatcher::routeMouseMessage(
    const MouseMessage& msg) {

    if (!mView->isVisible() || !mView->positionInView(msg.x, msg.y)) {
        return nullptr;
    }

    if (mView->canAddChild()) {
        ViewGroup* group = reinterpret_cast<ViewGroup*>(mView);
        MouseMessage childMsg(
            msg.type,
            msg.x - group->getXPosition(),
            msg.y - group->getYPosition(),
            msg.keyDownFlag);

        int count = group->getChildCount();
        for (auto i = count - 1; i >= 0; --i) {
            View* child = group->getChildAt(i);
            if (child->isVisible() && child->positionInView(childMsg.x, childMsg.y)) {
                if (BaseMessageResult* result = child->routeMessage(childMsg)) {
                    return result;
                }
            }
        }
    }

    if (msg.type == MessageType::SM_LButtonDown
        || msg.type == MessageType::SM_LButtonUp
        || msg.type == MessageType::SM_MouseHover
        || msg.type == MessageType::SM_MouseLeave
        || msg.type == MessageType::SM_MouseMove
        || msg.type == MessageType::SM_LButtonDblClick) {

        if (!mView->isEnable() || !mView->isClickable()) {
            return nullptr;
        }
    }

    return new MouseMessageResult(this);
}

void sf::MessageDispatcher::doMousePressed() {
    mView->setPressed(true);
}

void sf::MessageDispatcher::doMouseUnPressed() {
    mView->setPressed(false);
}

void sf::MessageDispatcher::doMouseHover() {
    mView->setMouseHovered(true);
    for (auto listener : mListeners) {
        listener->onMouseHover();
    }
}

void sf::MessageDispatcher::doMouseLeave() {
    mView->setMouseHovered(false);
    for (auto listener : mListeners) {
        listener->onMouseLeave();
    }
}
