//
//  Monster.h
//  FlyingBird
//
//  Created by cocos on 14-2-15.
//
//

#ifndef __FlyingBird__Monster__
#define __FlyingBird__Monster__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
USING_NS_CC;

class Monster : public extension::CCPhysicsSprite{
public:
    static Monster *create(cpSpace *space);
    Monster(cpSpace *space);
    ~Monster();
    void initAction();
    void initBody();
    void initShape();
    float getoffsetPx();
    cpBody *body;
private:
    CCSize  runningSize;
    cpSpace *space;
    cpShape *shape;
    float m_offsetPx;
    CCAction *runningAction;
};

#endif /* defined(__FlyingBird__Monster__) */
