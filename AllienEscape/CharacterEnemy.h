/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */


#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCScrollView.h"
#include "SpriteDS.h"
using namespace cocos2d;
using namespace extension;

class CharacterEnemy : public CCLayer
{
    
    CCScrollView *gamePlayBGScrollView;
    
    bool shouldMove;
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CharacterEnemy);
    cocos2d::CCPoint getPointsWithDistanceAtAngle(int angle, int distance, float x, float y);
    void getUpdatedPoints();
    void executings();
    cocos2d::CCPoint tilePosFromLocation(cocos2d::CCPoint location, cocos2d::CCTMXTiledMap* tileMap);
    cocos2d::CCPoint getISOCoord(CCTMXTiledMap *_map, CCPoint point);
    bool isColiedable(CCPoint tileCoord);
    CCPoint startingPosition;
    cocos2d::CCTMXTiledMap *_map;
    
private:
    cocos2d::CCSprite* pSprite;
    cocos2d::CCArray* animFrames;
    //int actualDuration;
    float characterScale;
    CCSize size;
    int actualDuration;
};

#endif // __HELLOWORLD_SCENE_H__
