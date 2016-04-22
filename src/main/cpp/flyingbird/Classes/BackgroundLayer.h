//
//  BackgroundLayer.h
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-8.
//
//

#ifndef __FlyingBird__BackgroundLayer__
#define __FlyingBird__BackgroundLayer__

#include <iostream>
#include "cocos2d.h"
#include "chipmunk.h"

USING_NS_CC;
class ObjectManager;
class BackgroundLayer : public CCLayer
{
public:
    BackgroundLayer();
    BackgroundLayer(CCSpriteBatchNode* spriteSheet,cpSpace* space);
    ~BackgroundLayer(){}
    bool checkAndReload(int eyeX);
    void loadObject(CCTMXTiledMap* map, int mapIndex);
    static BackgroundLayer* create(CCSpriteBatchNode* spriteSheet,cpSpace* space);
    CCTMXTiledMap* getCurMap(){ return curMap; };
    CCTMXTiledMap* getNextMap(){ return nextMap; };
    int getMapIndex(){return mapIndex; };
    ObjectManager* getObjManager(){return objectManager; };
    cpSpace* pSpace;
    CCArray* barArray;
    CCArray* daojuArray;
    int mapIndex;
    float bgWidth;
    int daojuX[20];
    int daojuY[20];
private:
    CCTMXTiledMap* map00;
    CCTMXTiledMap* map01;
    CCTMXTiledMap* map02;
    CCTMXTiledMap* curMap;
    CCTMXTiledMap* nextMap;
    CCTMXObjectGroup* topBar;
    CCTMXObjectGroup* daoju;
    float lastEyeX;
    CCSpriteBatchNode *mSpriteSheet;
    ObjectManager *objectManager;
    int flag;
};

#endif /* defined(__FlyingBird__BackgroundLayer__) */
