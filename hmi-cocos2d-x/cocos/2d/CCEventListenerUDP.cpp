//
//  CCEventListenerUDP.cpp
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/19/13.
//
//

#include "CCEventListenerUDP.h"
#include "CCEventUDP.h"


NS_CC_BEGIN

EventListenerUDP::EventListenerUDP()
: _onUDPEvent(nullptr)
{
}

EventListenerUDP* EventListenerUDP::create(std::function<void(EventUDP*)> callback)
{
    EventListenerUDP* ret = new EventListenerUDP();
    if (ret && ret->init(static_cast<ListenerID>(Type::UDP), callback))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerUDP::init(ListenerID listenerId, std::function<void(EventUDP*)>callback)
{
    bool ret = false;
    
    _onUDPEvent = callback;
    
    auto listener = [this](Event* event){
        if (_onUDPEvent != nullptr)
        {
            _onUDPEvent(static_cast<EventUDP*>(event));
        }
    };
    
    if (EventListener::init(EventListener::Type::UDP, listenerId, listener))
    {
        ret = true;
    }
    return ret;
}

EventListenerUDP* EventListenerUDP::clone()
{
    EventListenerUDP* ret = new EventListenerUDP();
    if (ret && ret->init(_listenerID, _onUDPEvent))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerUDP::checkAvailable()
{
    bool ret = false;
    if (EventListener::checkAvailable() && _onUDPEvent != nullptr)
    {
        ret = true;
    }
    return ret;
}

NS_CC_END