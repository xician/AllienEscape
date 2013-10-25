/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */

#ifndef __CandyMaker__Utils__
#define __CandyMaker__Utils__

#include "cocos2d.h"

using namespace cocos2d;

class  Utils : public cocos2d::CCNode
{
    
public:
    
    Utils();
    ~Utils();
    
    static void customFlurrLogEvent(std::string eventName);
    static void makeFullSize(cocos2d::CCNode *node);
    static void makeMultiResolution(cocos2d::CCNode *node);
    static  float getWidth(float width);
    static  float getHeight(float height);
    static cocos2d::CCPoint  makeMultiresolutionPoints( cocos2d::CCPoint point);
    static bool  DEVICE_IPHONE5();
    static bool  DEVICE_IPHONE();
    static bool  DEVICE_IPHONE_RETINA();
    static bool  DEVICE_IPAD_RETINA();
    static bool  DEVICE_IPAD();
    static cocos2d::CCSize getWinSize();
    
    static void setUserDefaultsObject ( cocos2d::TypeInfo obj , cocos2d::TypeInfo key);
    static cocos2d::TypeInfo getUserDefaultsObjectForKey(cocos2d::TypeInfo key);

     
};

#endif