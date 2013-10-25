//
//  SpriteDS.cpp
//  AllienEscape
//
//  Created by Xician on 10/25/13.
//
//

#include "SpriteDS.h"

void SpriteDS::setEnemyID(int id){
    this->enemyID = id;
}
void SpriteDS::setSprite(cocos2d::CCSprite *sprite){
    this->sprite = sprite;
    this->sprite->retain();
}

int SpriteDS::getEnemyID(){
    return this->enemyID;
}
cocos2d::CCSprite * SpriteDS::getSprite(){
    return this->sprite;
}
