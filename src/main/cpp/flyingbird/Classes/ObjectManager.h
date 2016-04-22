//
//  ObjectManager.h
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-10.
//
//

#ifndef __FlyingBird__ObjectManager__
#define __FlyingBird__ObjectManager__

#include <iostream>
#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
#include "Bar.h"
USING_NS_CC;
USING_NS_CC_EXT;
class ObjectManager :CCLayer
{
public:
    ObjectManager(CCSpriteBatchNode *spriteSheet, cpSpace* node, CCPoint position)
    {
        objects = CCArray::create();
        objects->retain();
        space = node;
        sPosition = position;
        this->pSpriteSheet = spriteSheet;
    }
    ~ObjectManager()
    {
        objects->release();
    }
    void initObjectOfMap(int mapIndex, float mapWidth);
    void removeObjectOfMap(int mapIndex);
private:
    CCArray* objects;
    cpSpace* space;
    CCPoint sPosition;
    Bar* upbar;
    Bar* down_upbar;
    Bar* buttonbar;
    Bar* down_buttonbar;
    CCSpriteBatchNode *pSpriteSheet;
};

#endif /* defined(__FlyingBird__ObjectManager__) */
