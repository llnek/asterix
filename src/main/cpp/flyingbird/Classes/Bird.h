//
//  Bird.h
//  FlyingBird
//
//  Created by cocos on 14-2-9.
//
//

#ifndef __FlyingBird__Bird__
#define __FlyingBird__Bird__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
USING_NS_CC;

class Bird : public extension::CCPhysicsSprite{
public:
    static Bird *create(cpSpace *space);
    Bird(cpSpace *space);
    ~Bird();
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

#endif /* defined(__FlyingBird__Bird__) */
