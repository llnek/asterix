//
//  Monster.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-15.
//
//

#include "Monster.h"
Monster *Monster::create(cpSpace *space)
{
    Monster *monster = new Monster(space);
    monster->autorelease();
    return monster;
}

Monster::Monster(cpSpace *space):m_offsetPx(100.0)
{
    this->space = space;
    initWithSpriteFrameName("monster-1.png");
    runningSize = this->getContentSize();
    
    initAction();
    initBody();
    initShape();
    setCPBody(body);
    runAction(runningAction);
    
    this->shape->collision_type = 7;
    this->shape->sensor = false;
    cpShapeSetUserData(shape, this);
}

Monster::~Monster()
{
    runningAction->release();
}

float Monster::getoffsetPx() {
    
    return m_offsetPx;
}

void Monster::initAction()
{
    CCArray *animFrames = CCArray::create();
    for (int i = 1; i < 4; i++)
    {
        CCString *name = CCString::createWithFormat("monster-%d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        animFrames->addObject(frame);
    }
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
    runningAction =CCRepeatForever::create(CCAnimate::create(animation));
    runningAction->retain();
}

void Monster::initBody()
{
    body = cpBodyNew(1.0f, cpMomentForBox(1.0f,this->runningSize.width, this->runningSize.height));
    body->v = cpv(-200, 600);
    cpSpaceAddBody(space, body);
}

void Monster::initShape()
{
    shape = cpBoxShapeNew(body, runningSize.width, runningSize.height);
    shape->e = 0;
    shape->u = 0;
    cpSpaceAddShape(space, shape);
}