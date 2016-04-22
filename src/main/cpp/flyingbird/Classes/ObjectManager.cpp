//
//  ObjectManager.cpp
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-10.
//
//

#include "ObjectManager.h"

#define Delta 128

void ObjectManager::initObjectOfMap(int mapIndex, float mapWidth)
{
    // x = 200 y = 160~672 in tiled map deltaY = 32n
    int x = sPosition.x;
    int y1 = sPosition.y - Delta / 2;
    int y2 = sPosition.y + Delta / 2;
    //CCLog("y1 = %d", y1);
    
    // add bar on the ground
    int num = (y1 - 64) / 32 + 1;

    if (num > 1)
    {
        upbar = Bar::create(pSpriteSheet, space, ccp(x, y1), 0);
        upbar->setTag(mapIndex);
        objects->addObject(upbar);
        
        for (int i = 1; i < num - 1; i++)
        {
            buttonbar = Bar::create(pSpriteSheet, space, ccp(x, y1 - 32 * i), 1);
            buttonbar->setTag(mapIndex);
            objects->addObject(buttonbar);
        }
        if (y1 - 32 * (num - 1) <= 80) {
            buttonbar = Bar::create(pSpriteSheet, space, ccp(x, 80), 1);
            buttonbar->setTag(mapIndex);
            objects->addObject(buttonbar);
            //CCLog("height < 96");
        }
        else
        {
            buttonbar = Bar::create(pSpriteSheet, space, ccp(x, y1 - 32 * (num - 1)), 1);
            buttonbar->setTag(mapIndex);
            objects->addObject(buttonbar);
            //CCLog("height >= 96");
            buttonbar = Bar::create(pSpriteSheet, space, ccp(x, 80), 1);
            buttonbar->setTag(mapIndex);
            objects->addObject(buttonbar);
        }
    }
    else
    {
        upbar = Bar::create(pSpriteSheet, space, ccp(x, 80), 0);
        upbar->setTag(mapIndex);
        objects->addObject(upbar);
    }
    
    
    // add bar in the sky
    int num2 = (464 - y2) / 32 + 1;
    
    if (num2 > 1)
    {
        down_upbar = Bar::create(pSpriteSheet, space, ccp(x, y2), 2);
        down_upbar->setTag(mapIndex);
        objects->addObject(down_upbar);
        
        for (int i = 1; i < num2 - 1; i++)
        {
            down_buttonbar = Bar::create(pSpriteSheet, space, ccp(x, y2 + 32 * i), 3);
            down_buttonbar->setTag(mapIndex);
            objects->addObject(down_buttonbar);
        }
        if (y2 + 32 * (num2 - 1) >= 464) {
            down_buttonbar = Bar::create(pSpriteSheet, space, ccp(x, 464), 3);
            down_buttonbar->setTag(mapIndex);
            objects->addObject(down_buttonbar);
            //CCLog("height >= 464 num2 = %d", num2);
        }
        else
        {
            down_buttonbar = Bar::create(pSpriteSheet, space, ccp(x, y2 + 32 * (num2 - 1)), 3);
            down_buttonbar->setTag(mapIndex);
            objects->addObject(down_buttonbar);
            down_buttonbar = Bar::create(pSpriteSheet, space, ccp(x, 464), 3);
            down_buttonbar->setTag(mapIndex);
            objects->addObject(down_buttonbar);
            //CCLog("num2 = %d", num2);
        }
    }
    else
    {
        down_upbar = Bar::create(pSpriteSheet, space, ccp(x, 464), 2);
        down_upbar->setTag(mapIndex);
        objects->addObject(down_upbar);
    }
    
}

void ObjectManager::removeObjectOfMap(int mapIndex)
{
    CCArray *toBeRemove = CCArray::create();// autorelease in next game frame
    
    CCObject *obj = NULL;
    CCPhysicsSprite* sprite = NULL;
    CCARRAY_FOREACH(this->objects, obj)
    {
        //sprite = (CCSprite *)obj;
        sprite = (CCPhysicsSprite *)obj;
        if (mapIndex == sprite->getTag()) {
            toBeRemove->addObject(sprite);
            sprite->removeFromParent();
        }
    }
    
    //remove objects here
    this->objects->removeObjectsInArray(toBeRemove);
    //CCLog("remove all objects");
}
