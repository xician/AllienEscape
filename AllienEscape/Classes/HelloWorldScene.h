/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */


#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCScrollView.h"
#include "CCLayerPanZoom.h"
#include "Utils.h"
#include "CharacterEnemy.h"
USING_NS_CC;
using namespace extension;
class HelloWorld : public CCLayerPanZoom
{
    
    CCScrollView *gamePlayBGScrollView;
    Enemy *enemy;
    bool shouldMove;
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    void getUpDatedPositions();
    
    CCArray *spriteTags,*spritePositions;
private:
    cocos2d::CCTMXTiledMap *_map;
    CCArray *enemies;

};

#endif // __HELLOWORLD_SCENE_H__
