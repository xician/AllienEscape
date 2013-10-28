//
//  CharacterHuman.h
//  ZombieEscapeTest1
//
//  Created by ImranR on 10/10/13.
//
//

#ifndef __ZombieEscapeTest1__CharacterHuman__
#define __ZombieEscapeTest1__CharacterHuman__

#include "cocos2d.h"
#include "Character.h"
using namespace cocos2d;

class Enemy : public Character
{
    
    CCSprite* character;
    
    
    
    bool shouldMove;
    float goAtAngle;
    
public:
    
    bool isHurdle;
    CCArray *patharray ;
    
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Enemy);
    cocos2d::CCPoint getPointsWithDistanceAtAngle(int angle, int distance, float x, float y);
    void getUpdatedPoints();
    void executings();
    cocos2d::CCPoint tilePosFromLocation(cocos2d::CCPoint location, cocos2d::CCTMXTiledMap* tileMap);
    cocos2d::CCPoint getISOCoord(CCTMXTiledMap *_map, CCPoint point);
    bool isColiedable(CCPoint tileCoord);
    void addEnemyToMap(CCPoint points);
    CCPoint startingPosition;
    cocos2d::CCTMXTiledMap *_map;
    cocos2d::CCPoint getCurrentPosition();
    cocos2d::CCSprite* getCurrentCharacter();
    
    
    virtual void draw();
    
private:
    cocos2d::CCSprite* pSprite;
    cocos2d::CCArray* animFrames;
    //int actualDuration;
    float characterScale;
    CCSize size;
    int actualDuration;
};


#endif /* defined(__ZombieEscapeTest1__CharacterHuman__) */
