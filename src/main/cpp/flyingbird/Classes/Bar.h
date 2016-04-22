//
//  Bar.h
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-10.
//
//

#ifndef __FlyingBird__Bar__
#define __FlyingBird__Bar__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "chipmunk.h"

USING_NS_CC;

class Bar : public extension::CCPhysicsSprite
{
public:
    static Bar* create(CCSpriteBatchNode *spriteSheet,cpSpace* node, CCPoint position, int barTag);
    Bar(CCSpriteBatchNode *spriteSheet,cpSpace* node, CCPoint position, int barTag);
    ~Bar();
    cpBody *body;
    cpShape *shape;
private:
    CCSize  pipeSize;
    cpSpace *space;
};

#endif /* defined(__FlyingBird__Bar__) */
