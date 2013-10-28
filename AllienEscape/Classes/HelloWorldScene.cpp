/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */


#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CharacterEnemy.h"
using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //    CCDictionary *plistValues = CCDictionary::createWithContentsOfFile("level1.plist");
    //    enemies=(cocos2d::CCArray *)plistValues->objectForKey("enemies");
    //    CCLOG("total enemies: %i ",enemies->count());
    
    
    
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerPanZoom::init() )
    {
        return false;
    }
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    
    //    actualDuration = CCArray::create();
    
    
    //    for(int i=0; i<enemies->count(); i++){
    //        int direction = ( arc4random() % 7 );
    //        this->actualDuration[i] = direction;
    //        this->addNewEnemyToScreen(i);
    //    }
    
    _map = CCTMXTiledMap::create("iso-test-zorder.tmx");
    addChild(_map,-1);
    this->setContentSize(_map->getContentSize());
    spriteTags = CCArray::create();
    spriteTags->retain();
    
        CCDictionary *plistValues = CCDictionary::createWithContentsOfFile("level1.plist");
        enemies=(cocos2d::CCArray *)plistValues->objectForKey("enemies");
    for(int i=0; i<enemies->count(); i++){
        Enemy *enem = Enemy::create();
        enem->_map = this->_map;
        enem->addEnemyToMap(ccp(600,480));
        spriteTags->addObject(enem);
        enem->retain();
        this->addChild(enem);

    }


    CCCallFunc *getUpdPos = CCCallFunc::create(this, callfunc_selector(HelloWorld::getUpDatedPositions));
    this->runAction( CCRepeatForever::create( CCSpawn::create(getUpdPos, NULL) ));

    this->setTouchEnabled(true);
    
    
    return true;
}
void HelloWorld::getUpDatedPositions(){
    float minimumDistance = -1.0;
    Enemy *tempModule;

    for(int i=0; i < spriteTags->count() ; i++){
        Enemy *temp = (Enemy *)spriteTags->objectAtIndex(i);
        CCPoint point = temp->getCurrentPosition();
        for(int j=0; j < spriteTags->count() ; j++){
            if(j != i){
            tempModule = (Enemy *)spriteTags->objectAtIndex(j);
            float distance = ccpDistance(tempModule->getCurrentPosition(), point);
            if(minimumDistance < 0 || minimumDistance >= distance){
                minimumDistance = distance;
            }
            }
        }
        temp->colied = tempModule->getCurrentCharacter();
        tempModule->colied = temp->getCurrentCharacter();
        
        //CCLog("%f - %f/%f - %f/%f",minimumDistance,point.x, point.y, tempModule->getCurrentPosition().x, tempModule->getCurrentPosition().y);
        minimumDistance = -1.0;
    }
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
