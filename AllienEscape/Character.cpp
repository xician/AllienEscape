/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */

#include "Character.h"

Character* Character::layer()
{
    // 'layer' is an autorelease object
    Character *layer = Character::create();
    
    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool Character::init()
{
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
        return true;
}

