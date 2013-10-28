//
//  CharacterHuman.cpp
//  ZombieEscapeTest1
//
//  Created by ImranR on 10/10/13.
//
//

#include "CharacterEnemy.h"


CCScene* Enemy::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Enemy *layer = Enemy::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Enemy::init()
{
    //    CCDictionary *plistValues = CCDictionary::createWithContentsOfFile("level1.plist");
    //    enemies=(cocos2d::CCArray *)plistValues->objectForKey("enemies");
    //    CCLOG("total enemies: %i ",enemies->count());
    
    
    
    
    //////////////////////////////
    // 1. super init first
    if ( !Character::init() )
    {
        return false;
    }
    
    
    return true;
}

void Enemy::addEnemyToMap(CCPoint points){
    characterScale = 0.3;
    
    size = CCDirector::sharedDirector()->getWinSize();
    
    
    // get place enemy on screen and start animating
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("zombi_run.plist");
    animFrames = CCArray::createWithCapacity(21);
    char str[100] = {0};
    for(int i = 1; i < 20; i++)
    {
        if(i<10)
            sprintf(str, "run000%d.png", i);
        else
            sprintf(str, "run00%d.png", i);
        
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( str );
        animFrames->addObject(frame);
    }
    
    CCAnimation *runAnimation=CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
    pSprite = CCSprite::createWithSpriteFrameName("run0001.png");
    
    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(points.x,points.y) );
    pSprite->runAction( CCRepeatForever::create( CCAnimate::create(runAnimation) ));
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    pSprite->setScaleX(characterScale);
    pSprite->setScaleY(characterScale);
    this->actualDuration = ( arc4random() % 7 );
    if(actualDuration == 1 || actualDuration == 2 || actualDuration == 3){
        pSprite->setScaleX(characterScale);
        
    }else if (actualDuration == 5 || actualDuration == 6 || actualDuration == 7){
        pSprite->setScaleX(-characterScale);
    }
    
    CCPoint endPoint = this->getPointsWithDistanceAtAngle(actualDuration*45, 200, pSprite->getPositionX(),pSprite->getPositionY());
    float x = endPoint.x;
    float y = endPoint.y;
    
    CCFiniteTimeAction* actionMove =CCMoveTo::create( 2.0,ccp((int)x,(int)y) );
    CCCallFunc *functions = CCCallFunc::create(this, callfunc_selector(Enemy::executings));
    
    
    CCSequence *seq = CCSequence::create(actionMove,functions,NULL);
    seq->setTag(1);
    pSprite->runAction(seq);
    
    CCCallFunc *getUpdPos = CCCallFunc::create(this, callfunc_selector(Enemy::getUpdatedPoints));
    pSprite->runAction(CCRepeatForever::create(CCSpawn::create(getUpdPos, NULL)));
    
    
}
void Enemy::executings(){
    CCSprite *sprite = pSprite;
    this->actualDuration = ( arc4random() % 7 );
    
    if(actualDuration == 1 || actualDuration == 2 || actualDuration == 3){
        sprite->setScaleX(characterScale);
        
    }else if (actualDuration == 5 || actualDuration == 6 || actualDuration == 7){
        sprite->setScaleX(-characterScale);
    }
    CCPoint endPoint = this->getPointsWithDistanceAtAngle(actualDuration*45, 200, sprite->getPositionX(),sprite->getPositionY());
    float x = endPoint.x;
    float y = endPoint.y;
    
    
    CCFiniteTimeAction* actionMove =CCMoveTo::create( 2.0,ccp((int)x,(int)y) );
    CCCallFunc *functions = CCCallFunc::create(this, callfunc_selector(Enemy::executings));
    CCSequence *seq = CCSequence::create(actionMove,functions,NULL);
    seq->setTag(1);
    sprite->runAction(seq);
    
    
    
}
cocos2d::CCSprite* Enemy::getCurrentCharacter(){
    return pSprite;
}

cocos2d::CCPoint Enemy::getCurrentPosition(){
    return pSprite->getPosition();
}
void Enemy::draw(){
    if(this->colied != NULL && ccpDistance(this->colied->getPosition(), pSprite->getPosition()) <= 200){
            ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
            ccDrawLine(this->colied->getPosition(), pSprite->getPosition());
        
    }
    
}
void Enemy::getUpdatedPoints(){
    
    CCSprite *sprite = pSprite;
    CCPoint points = sprite->getPosition();
    if(this->isColiedable(this->getISOCoord(_map, points))){
        sprite->stopActionByTag(1);
        // Set the opposite path for character
        switch (actualDuration) {
            case 0:
                this->actualDuration = 6;
                pSprite->setPosition(ccp(pSprite->getPosition().x-10,pSprite->getPosition().y-10));
                break;
            case 1:
                this->actualDuration = 6;
                pSprite->setPosition(ccp(pSprite->getPosition().x-10,pSprite->getPosition().y));
                break;
            case 2:
                this->actualDuration = 0;
                pSprite->setPosition(ccp(pSprite->getPosition().x-10,pSprite->getPosition().y+10));
                break;
            case 3:
                this->actualDuration = 0;
                pSprite->setPosition(ccp(pSprite->getPosition().x,pSprite->getPosition().y+10));
                break;
            case 4:
                this->actualDuration = 2;
                pSprite->setPosition(ccp(pSprite->getPosition().x+10,pSprite->getPosition().y+10));
                break;
            case 5:
                this->actualDuration = 2;
                pSprite->setPosition(ccp(pSprite->getPosition().x+10,pSprite->getPosition().y));
                break;
            case 6:
                this->actualDuration = 4;
                pSprite->setPosition(ccp(pSprite->getPosition().x+10,pSprite->getPosition().y-10));
                break;
            case 7:
                this->actualDuration = 4;
                pSprite->setPosition(ccp(pSprite->getPosition().x,pSprite->getPosition().y-10));
                break;
        }
        
        
        if(actualDuration >= 4){
            sprite->setScaleX(-characterScale);
        }else{
            sprite->setScaleX(characterScale);
        }
        
        CCPoint endPoint = this->getPointsWithDistanceAtAngle(45*actualDuration, 80, sprite->getPositionX(),sprite->getPositionY());
        float x = endPoint.x;
        float y = endPoint.y;
        CCFiniteTimeAction* actionMove =CCMoveTo::create( 2.0,ccp((int)x,(int)y) );
        CCCallFunc *functions = CCCallFunc::create(this, callfunc_selector(Enemy::executings));
        CCSequence *seq = CCSequence::create(actionMove,functions,NULL);
        seq->setTag(1);
        sprite->runAction(seq);
    }
    
    
    
}

cocos2d::CCPoint Enemy::getPointsWithDistanceAtAngle(int angle, int distance, float x, float y){
    CCPoint endPoint;
    endPoint.x = sinf(CC_DEGREES_TO_RADIANS(angle)) * distance;
    endPoint.y = cosf(CC_DEGREES_TO_RADIANS(angle)) * distance;
    endPoint = ccpAdd(CCPoint(x,y), endPoint);
    return endPoint;
    
}

cocos2d::CCPoint Enemy::tilePosFromLocation(CCPoint location, CCTMXTiledMap* tileMap) {
    CCPoint characterPos = ccp(location.x-(tileMap->getMapSize().width*tileMap->getTileSize().width), location.y-(tileMap->getMapSize().height*tileMap->getTileSize().height));
    
    CCPoint gridPosition = ccp((int)characterPos.x % 9, (int)characterPos.y % 9);
    return gridPosition;
}
cocos2d::CCPoint Enemy::getISOCoord(CCTMXTiledMap *_map, CCPoint point){
    int posY = _map->getMapSize().height - point.x/_map->getTileSize().width + _map->getMapSize().width/2 - point.y/_map->getTileSize().height;
    
    int posX = _map->getMapSize().height + point.x/_map->getTileSize().width - _map->getMapSize().width/2 - point.y/_map->getTileSize().height;
    
    if(posX<0)
        posX=0;
    if(posY<1)
        posY=1;
    if(posX>_map->getMapSize().width-1)
        posX=_map->getMapSize().width-1;
    if(posY>_map->getMapSize().height)
        posY=_map->getMapSize().height;
    
    return ccp(posX, posY);
}

bool Enemy::isColiedable(CCPoint tileCoord){
    bool flag = false;
    CCTMXLayer *_meta = _map->layerNamed("colied");
    int tileGid = _meta->tileGIDAt(tileCoord);
    if(tileGid){
        flag = true;
    }
    return flag;
}


