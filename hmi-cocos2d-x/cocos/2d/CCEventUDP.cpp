//
//  CCEventUDP.cpp
//  cocos2d_libs
//
//  Created by Keith Ballantyne on 12/19/13.
//
//

#include "CCEventUDP.h"

NS_CC_BEGIN

EventUDP::EventUDP()
: Event(Type::UDP)
, _userData(nullptr)
, _len(0)
{
}

EventUDP::EventUDP(void *data, int len)
: Event(Type::UDP)
, _len(len)
{
    _userData = (void *) new char[len];
    memcpy(_userData,data,len);
}

EventUDP::~EventUDP() {
    delete (char *)_userData;  // slightly scary
}

NS_CC_END