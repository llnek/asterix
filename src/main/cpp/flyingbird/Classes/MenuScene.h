//
//  MenuScene.h
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#ifndef __FlyingBird__MenuScene__
#define __FlyingBird__MenuScene__

#include "cocos2d.h"
USING_NS_CC;

class MenuLayer : public CCLayer
{
public:
    MenuLayer();
    ~MenuLayer();
    virtual bool init();
    static cocos2d::CCScene* scene();
    void addMenuList();
    CREATE_FUNC(MenuLayer);
    void startCallback(CCObject* obj);
    void scoreCallback(CCObject* obj);
};

#endif /* defined(__FlyingBird__MenuScene__) */
