/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */

#include "cocos2d.h"




class Character : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the exactly class pointer
    static Character* layer();
    
    // implement the "static node()" method manually
    CREATE_FUNC(Character);
    
    cocos2d::CCSprite *colied;

};