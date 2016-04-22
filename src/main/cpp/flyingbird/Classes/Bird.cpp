//
//  Bird.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-9.
//
//

#include "Bird.h"
Bird *Bird::create(cpSpace *space)
{
    Bird *bird = new Bird(space);
    bird->autorelease();
    return bird;
}

Bird::Bird(cpSpace *space):m_offsetPx(100.0)
{
    this->space = space;
    initWithSpriteFrameName("wugui_1.png");
    runningSize = this->getContentSize();
    
    initAction();
    initBody();
    initShape();
    setCPBody(body);
    runAction(runningAction);
    
    this->shape->collision_type = 1;
    this->shape->sensor = false;
    cpShapeSetUserData(shape, this);
}

Bird::~Bird()
{
    runningAction->release();
    cpBodyFree(body);
    cpShapeFree(shape);
}

float Bird::getoffsetPx() {
    
    return m_offsetPx;
}

void Bird::initAction()
{
    CCArray *animFrames = CCArray::create();
    for (int i = 1; i < 4; i++)
    {
        CCString *name = CCString::createWithFormat("wugui_%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
    runningAction =CCRepeatForever::create(CCAnimate::create(animation));
    runningAction->retain();
}

void Bird::initBody()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    body = cpBodyNew(1.0f, cpMomentForBox(1.0f,this->runningSize.width, this->runningSize.height));
    body->p = cpv(100, 50 + size.height / 2);
    body->v = cpv(100, 0);
    cpSpaceAddBody(space, body);
}

void Bird::initShape()
{
    shape = cpBoxShapeNew(body, runningSize.width, runningSize.height);
    shape->e = 0;
    shape->u = 0;
    cpSpaceAddShape(space, shape);
}