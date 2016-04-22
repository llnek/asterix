//
//  PropsSprites.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-12.
//
//

#include "PropsSprites.h"
PropsSprites *PropsSprites::create(cpSpace *space,int type,CCPoint point)
{
    PropsSprites *props = new PropsSprites(space,type,point);
    props->autorelease();
    return props;
}

PropsSprites::PropsSprites(cpSpace *space,int type,CCPoint point):m_offsetPx(100.0)
{
    this->space = space;
    if(type == 1){
        initWithSpriteFrameName("block.png");
    }
//    else if(type == 2) {
//        initWithSpriteFrameName("tree.png");
//    }
//    else if(type == 3) {  //加命道具
//        initWithSpriteFrameName("block.png");
//    }else if(type == 4) { //加分道具
//        initWithSpriteFrameName("block.png");
//    }
    runningSize = this->getContentSize();
    
    initBody(point);
    initShape();
    setCPBody(body);
    
    this->shape->collision_type = type + 4;
    this->shape->sensor = false;
    cpShapeSetUserData(shape, this);
}

PropsSprites::~PropsSprites()
{
    //cpShapeFree(shape);
    //cpBodyFree(body);
}

float PropsSprites::getoffsetPx() {
    
    return m_offsetPx;
}

void PropsSprites::initBody(CCPoint point)
{
    body = cpBodyNewStatic();
    body->p = cpv(point.x, point.y);
}

void PropsSprites::initShape()
{
    shape = cpBoxShapeNew(body, runningSize.width, runningSize.height);
    shape->e = 0;
    shape->u = 0;
    cpSpaceAddStaticShape(space, shape);
}