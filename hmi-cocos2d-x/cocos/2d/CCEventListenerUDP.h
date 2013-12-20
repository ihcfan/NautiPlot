//
//  CCEventListenerUDP.h
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/19/13.
//
//

#ifndef __cocos2d_libs__CCEventListenerUDP__
#define __cocos2d_libs__CCEventListenerUDP__

#include "CCEventListener.h"
#include "CCEventUDP.h"

NS_CC_BEGIN

// class EventUDP;

/**
 *  Usage:
 *        auto dispatcher = Director::getInstance()->getEventDispatcher();
 *     Adds a listener:
 *
 *        auto callback = [](EventCustom* event){ do_some_thing(); };
 *        auto listener = EventListenerCustom::create(callback);
 *        dispatcher->addEventListenerWithSceneGraphPriority(listener, one_node);
 *
 *     Dispatchs a custom event:
 *
 *        EventCustom event("your_event_type");
 *        dispatcher->dispatchEvent(&event);
 *
 *     Removes a listener
 *
 *        dispatcher->removeEventListener(listener);
 */
class EventListenerUDP : public EventListener
{
public:
    /** Creates an event listener with type and callback.
     *  @param eventType The type of the event.
     *  @param callback The callback function when the specified event was emitted.
     */
    static EventListenerUDP* create(std::function<void(cocos2d::EventUDP*)> callback);
    
    /// Overrides
    virtual bool checkAvailable() override;
    virtual EventListenerUDP* clone() override;
    
protected:
    /** Constructor */
    EventListenerUDP();
    
    /** Initializes event with type and callback function */
    bool init(ListenerID listenerId, std::function<void(cocos2d::EventUDP*)> callback);
    
    std::function<void(EventUDP*)> _onUDPEvent;
};

NS_CC_END


#endif /* defined(__cocos2d_libs__CCEventListenerUDP__) */
