//
//  PauseLayer.h
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#ifndef __FlyingBird__PauseLayer__
#define __FlyingBird__PauseLayer__

#include "cocos2d.h"
USING_NS_CC;

class PauseLayer : public CCLayerColor
{
public:
    PauseLayer();
    ~PauseLayer();
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(PauseLayer);
    void addMenuList();
    void menuCallback(CCObject* obj);
};

#endif /* defined(__FlyingBird__PauseLayer__) */
