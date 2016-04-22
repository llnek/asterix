//
//  PropsSprites.h
//  FlyingBird
//
//  Created by cocos on 14-2-12.
//
//

#ifndef __FlyingBird__PropsSprites__
#define __FlyingBird__PropsSprites__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"

USING_NS_CC;

class PropsSprites : public extension::CCPhysicsSprite{
public:
    static PropsSprites *create(cpSpace *space,int type,CCPoint point);
    PropsSprites(cpSpace *space,int type,CCPoint point);
    ~PropsSprites();
    void initBody(CCPoint point);
    void initShape();
    float getoffsetPx();
    cpBody *body;
    cpShape *shape;
private:
    CCSize  runningSize;
    cpSpace *space;
    float m_offsetPx;
};

#endif /* defined(__FlyingBird__PropsSprites__) */
