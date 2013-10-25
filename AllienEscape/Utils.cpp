/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */

#include "Utils.h"
#include "cocos2d.h"





using namespace cocos2d;

#define CC_IS_IPAD() (CCDirector::sharedDirector()->getWinSize().width==1024||CCDirector::sharedDirector()->getWinSize().height==1024)

#define CC_IS_IPAD_RETINA() (CCDirector::sharedDirector()->getWinSize().width==1536||CCDirector::sharedDirector()->getWinSize().height==1536)

#define CC_IS_IPHONE() (CCDirector::sharedDirector()->getWinSize().width==480||CCDirector::sharedDirector()->getWinSize().height==480)

#define CC_IS_IPHONE_RETINA() (CCDirector::sharedDirector()->getWinSize().width==960||CCDirector::sharedDirector()->getWinSize().height==960)

#define CC_IS_IPHONE_5() (CCDirector::sharedDirector()->getWinSize().width==1136||CCDirector::sharedDirector()->getWinSize().height==1136)

Utils::Utils()
{
    CCLOG("Utils Constructor");
    
    if(CC_IS_IPAD())
        CCLOG("SELECTED DEVICE:   IPAD ");
    
    else if(CC_IS_IPHONE())
        CCLOG("SELECTED DEVICE:   IPHONE ");
    
    else if(CC_IS_IPAD_RETINA())
        CCLOG("SELECTED DEVICE:   IPAD RETINA ");
    
    else if(CC_IS_IPHONE_RETINA())
        CCLOG("SELECTED DEVICE:   IPHONE RETINA ");
    
    
}
Utils::~Utils()
{
}


void Utils::makeFullSize(cocos2d::CCNode * node)
{
    float screenWidth  =  CCDirector::sharedDirector()->getWinSizeInPixels().width;
    float screenHeight  = CCDirector::sharedDirector()->getWinSizeInPixels().height;
    
    float width = node->boundingBox().size.width;
    float height = node->boundingBox().size.height;
    
    node->setScaleX(screenWidth/width);
    node->setScaleY(screenHeight/height);
}

void Utils::makeMultiResolution(cocos2d::CCNode * sprite)
{
    float oldw = sprite->boundingBox().size.width;
    float oldh = sprite->boundingBox().size.height;
    
   // CCLog("old width = %f  old height =  %f",oldw,oldh);
    
    float width = Utils::getWidth( sprite->boundingBox().size.width) ;
    float height = Utils::getHeight( sprite->boundingBox().size.height );
    
    //CCLog("Scaled width = %f  Scaled height =  %f",width,height);
    
    sprite->setScaleX(width/oldw);
    sprite->setScaleY(height/oldh);
    
    return;
        
}


bool Utils:: DEVICE_IPHONE5()
{
    return CC_IS_IPHONE_5();
}
bool  Utils::DEVICE_IPHONE()
{
    return CC_IS_IPHONE();
}
bool  Utils::DEVICE_IPHONE_RETINA()
{
    return CC_IS_IPHONE_RETINA();
}
bool  Utils::DEVICE_IPAD_RETINA()
{
    return CC_IS_IPAD_RETINA();
}
bool  Utils::DEVICE_IPAD()
{
    return CC_IS_IPAD();
}



float Utils::getWidth(float width)
{
    double x_ = (width * 100) / 960;
    return  (float)(x_ / 100) * CCDirector::sharedDirector()->getWinSizeInPixels().width;
}
float Utils::getHeight(float height)
{
    double y_ = (height * 100) / 640;
    return (float)(y_ / 100) *  CCDirector::sharedDirector()->getWinSizeInPixels().height;
}

cocos2d::CCPoint Utils::makeMultiresolutionPoints( cocos2d::CCPoint   point)
{
    return     CCPointMake(Utils::getWidth(point.x),Utils::getHeight( point.y ) );
}

cocos2d::CCSize Utils::getWinSize()
{
    return CCDirector::sharedDirector()->getWinSize();
}










