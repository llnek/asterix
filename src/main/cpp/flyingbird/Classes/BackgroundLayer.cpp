 //
//  BackgroundLayer.cpp
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-8.
//
//

#include "BackgroundLayer.h"
#include "ObjectManager.h"
#include "PhysicWorldScene.h"

BackgroundLayer* BackgroundLayer::create(CCSpriteBatchNode* spriteSheet,cpSpace* space){
    
    BackgroundLayer* bgLayer = new BackgroundLayer(spriteSheet,space);
    bgLayer->autorelease();
    return bgLayer;
}

BackgroundLayer::BackgroundLayer(CCSpriteBatchNode* spriteSheet,cpSpace* space)
{
    mSpriteSheet = spriteSheet;
    this->pSpace = space;
    objectManager = NULL;
    map00 = NULL;
    map01 = NULL;
    map02 = NULL;
    topBar = NULL;
    bgWidth = 0;
    mapIndex = 0;
    curMap = map00;
    nextMap = map01;
    flag = 0;
    
    map00 = CCTMXTiledMap::create("bg0.tmx");
    bgWidth = map00->getContentSize().width;
    map00->setPosition(ccp(0, 0));
    addChild(map00);
    
    map01 = CCTMXTiledMap::create("bg1.tmx");
    map01->setPosition(ccp(bgWidth, 0));
    addChild(map01);
    
    map02 = CCTMXTiledMap::create("bg2.tmx");
    map02->setPosition(ccp(bgWidth * 2, 0));
    addChild(map02);

    loadObject(map00, 0);
//    loadObject(map01, 1);
}

void BackgroundLayer::loadObject(CCTMXTiledMap* map, int mapIndex)
{
    //CCLog("loadObject map0%d", mapIndex);
    // add bars
    topBar = map->objectGroupNamed("center");
    barArray = topBar->getObjects();
    for (int i = 0; i < barArray->count(); i++) {
        CCDictionary* bar = (CCDictionary*)barArray->objectAtIndex(i);
        int x = bar->valueForKey("x")->intValue() / 2 + bgWidth * mapIndex;
        int y = bar->valueForKey("y")->intValue() / 2;
        objectManager = new ObjectManager(mSpriteSheet, pSpace, ccp(x, y));
        objectManager->initObjectOfMap(mapIndex, bgWidth);
    }
    
    daoju = map->objectGroupNamed("center");
    daojuArray = daoju->getObjects();
    for (int j = 0; j < daojuArray->count(); j++) {
        CCDictionary* dj = (CCDictionary*)daojuArray->objectAtIndex(j);
        daojuX[j] = dj->valueForKey("x")->intValue() / 2 + bgWidth * mapIndex;
        daojuY[j] = dj->valueForKey("y")->intValue() / 2;
    }
}

bool BackgroundLayer::checkAndReload(int eyeX)
{
    // to be done
    int newMapIndex = eyeX / this->bgWidth;
    //CCLog("eyex = %d bgwidth = %f", eyeX, bgWidth);
    if (this->mapIndex == newMapIndex) {
        if (flag == 0) {
            if (380.0 <= eyeX && eyeX <= 400) {
                CCLog("load map01");
                loadObject(map01, 1);
                flag = 1;
            }
        }
        
        return false;
    }

    if (1 == newMapIndex % 2) {
        // change second bg
        map02->setPositionX(bgWidth * (newMapIndex+1));
        curMap = map01;
        nextMap = map02;
//        objectManager->removeObjectOfMap(newMapIndex - 1);
//        loadObject(map01, newMapIndex + 1);
    }
    else{
        // change 3rd bg
        map01->setPositionX(bgWidth * (newMapIndex +1));
        curMap = map02;
        nextMap = map01;
//        objectManager->removeObjectOfMap(newMapIndex - 1);
//        loadObject(map02, newMapIndex + 1);
    }
    curMap->setPositionX(bgWidth * newMapIndex);
    mapIndex = newMapIndex;
    CCLog("reload curMap = %d", mapIndex % 2);
    return true;
}
