/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"
#include "base/listener/ListenerList.h"
#include "graphics/view/event/ViewEvent.h"
#include "graphics/view/message/MessageDef.h"
#include "graphics/view/ViewListener.h"


namespace sf {

    class STRAW_FRAMEWORK_API View;


    class STRAW_FRAMEWORK_API MessageDispatcher : public ListenerList<ViewListener> {

    public:
        MessageDispatcher(View* view);
        virtual ~MessageDispatcher();

        virtual BaseMessageResult* routeMessage(const BaseMessage& msg) final;
        virtual bool onMessage(const BaseMessage& msg, BaseMessageResult** result);
        virtual void dispatchMessage(const BaseMessage& msg) final;


    public:
        void doMousePressed();
        void doMouseUnPressed();
        void doMouseHover();
        void doMouseLeave();

    private:
        BaseMessageResult* routeMouseMessage(const MouseMessage& msg);
        BaseMessageResult* routeMessageInternal(const BaseMessage& msg);

    protected:
        View* mView;
    };

}
