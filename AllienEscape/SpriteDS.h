//
//  SpriteDS.h
//  AllienEscape
//
//  Created by Xician on 10/25/13.
//
//

#ifndef __AllienEscape__SpriteDS__
#define __AllienEscape__SpriteDS__

#include <iostream>
#include "cocos2d.h"
class SpriteDS
{
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    
    void setEnemyID(int id);
    void setSprite(cocos2d::CCSprite *sprite);

    int getEnemyID();
    cocos2d::CCSprite * getSprite();
    
private:
    int enemyID;
    cocos2d::CCSprite *sprite;
};

#endif /* defined(__AllienEscape__SpriteDS__) */
