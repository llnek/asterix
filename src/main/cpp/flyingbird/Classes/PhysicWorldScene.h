//
//  PhysicWorldScene.h
//  FlyingBird
//
//  Created by cocos on 14-2-9.
//
//

#ifndef __FlyingBird__PhysicWorldScene__
#define __FlyingBird__PhysicWorldScene__

#include "cocos2d.h"
#include "chipmunk.h"
#include "ObjectManager.h"
#include "GameOverScene.h"
#include "BackgroundLayer.h"
#include "Bird.h"
#include "PropsSprites.h"
#include "Monster.h"

USING_NS_CC;
static int propsCount = 0;
class PhysicWorldLayer : public CCLayer
{
public:
    PhysicWorldLayer();
    ~PhysicWorldLayer();
    CREATE_FUNC(PhysicWorldLayer);
    virtual bool init();
    virtual void update(float delta);
    static CCScene* scene();
    void createPhysicWorld();
    void addBackgroundLayer();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    static int collisionBegin(cpArbiter *arb, cpSpace *space, void *param);
    static int getPropesBegin(cpArbiter *arb, cpSpace *space, void *param);
    static void postStepRemove(cpSpace *space, cpShape *shape, void *param);
    void addProps(float dt);
    void addMonsters(float dt);
    void endCollisionEffect(float dt);
    cpSpace *space;
    int nScore;
    char strScore[10];
private:
    int lastX;
    bool isScoreAdded;
    int collisionpropesType;
    bool isCalledBefore;
    bool isGameOver;
    float lastEyeX;
    CCLabelTTF* label;
    cpShape* wallBottom;
    cpShape* wallTop;
    CCSpriteBatchNode* spriteSheet;
    Bird* bird;
    Monster* monster;
    BackgroundLayer* backgroundLayer;
    PropsSprites* propSp;
};

#endif /* defined(__FlyingBird__PhysicWorldScene__) */
