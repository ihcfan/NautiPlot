#include "NautiPlotHMI.h"
#include "CCEventUDP.h"

USING_NS_CC;

Scene* NautiPlotHMI::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = NautiPlotHMI::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool NautiPlotHMI::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(NautiPlotHMI::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("NautiPlotHMI", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label,1);
    
    latlong = LabelTTF::create("XYZ", "Arial", 14);
    
    // position the label on the center of the screen
    latlong->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - 2*label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(latlong,2,32);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("SplashScreen.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    listener = EventListenerUDP::create([=](cocos2d::EventUDP* event) {
        udpCallback(event);
    });
                                        
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
    return true;
}

void NautiPlotHMI::udpCallback(cocos2d::EventUDP* event) {
    void* userData= event->getUDPData();
    int len = event->length();
    
//    const LabelTTF *text = static_cast<const LabelTTF *>(getChildByTag(32));
//    if (text != nullptr) removeChild(getChildByTag(32));
    char *test = (char *)userData;
    test[38]=0;
    std::string text(test);
    latlong->setString(text);
//    fprintf(stderr,"set %p",latlong);
}

void NautiPlotHMI::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
