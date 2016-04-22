//
//  MainScene.h
//  FlyingBird
//
//  Created by cocos on 14-2-8.
//
//

#ifndef __FlyingBird__MainScene__
#define __FlyingBird__MainScene__

#include "cocos2d.h"
#include "PhysicWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class MainLayer : public CCLayer
{
public:
    MainLayer();
    ~MainLayer();
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MainLayer);
    void addPauseBtn();
    void pauseCallback(CCObject* obj);
    bool isPauseState;
    void gameOverCall();
    virtual void update(float dt);
private:
    bool isCalledBefore;
    bool isGameOver;
    CCLabelTTF* label;
    PhysicWorldLayer* pwLayer;
    CocosDenshion::SimpleAudioEngine *audioEngine;
};

#endif /* defined(__FlyingBird__MainScene__) */
