//
//  Bar.cpp
//  FlyingBird
//
//  Created by 郭 一鸣 on 14-2-10.
//
//

#include "Bar.h"

Bar::~Bar(){
    cpBodyFree(body);
    cpShapeFree(shape);
}

Bar* Bar::create(CCSpriteBatchNode *spriteSheet,cpSpace* node, CCPoint position, int barTag)
{
    Bar* bar = new Bar(spriteSheet,node, position, barTag);
    bar->autorelease();
    return bar;
}

Bar::Bar(CCSpriteBatchNode *spriteSheet,cpSpace* node, CCPoint position, int barTag)
{
    this->space = node;
    if (barTag == 0) {
        this->initWithSpriteFrameName("top.png");
    }
    else if (barTag == 1){
        this->initWithSpriteFrameName("button.png");
    }
    else if (barTag == 2){
        this->initWithSpriteFrameName("topdown.png");
    }
    else if (barTag == 3){
        this->initWithSpriteFrameName("buttonup.png");
    }
    
    pipeSize = getContentSize();
    body = cpBodyNewStatic();
    body->p = cpv(position.x, position.y);
    setCPBody(body);
    shape = cpBoxShapeNew(body, pipeSize.width, pipeSize.height);
    cpSpaceAddStaticShape(space, shape);
    spriteSheet->addChild(this,2);
    this->shape->collision_type = 2;
    this->shape->sensor = false;
    cpShapeSetUserData(shape, this);
}
