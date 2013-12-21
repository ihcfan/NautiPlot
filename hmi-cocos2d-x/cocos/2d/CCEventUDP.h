//
//  CCEventUDP.h
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/19/13.
//
//

#ifndef __cocos2d_libs__CCEventUDP__
#define __cocos2d_libs__CCEventUDP__

#include "CCEvent.h"
#include "CCObject.h"


NS_CC_BEGIN

class EventUDP : public Object, public Event
{
public:
    /** Constructor */
    EventUDP();
    EventUDP(void* data, int len);
    ~EventUDP();
    
    /** Sets user data */
    inline void setUDPData(void* data, int len) { _userData = data; _len=len; }
    
    /** Gets user data */
    inline void* getUDPData() const { return _userData; }
    inline int length() const { return _len; }
    
protected:
    void* _userData;       ///< User data
    int _len;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCEventUDP__) */
