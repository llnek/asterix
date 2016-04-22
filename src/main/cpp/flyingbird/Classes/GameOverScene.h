//
//  GameOverScene.h
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#ifndef __FlyingBird__GameOverScene__
#define __FlyingBird__GameOverScene__

#include "cocos2d.h"
USING_NS_CC;

class GameOverLayer : public CCLayer
{
public:
    GameOverLayer();
    ~GameOverLayer();
    virtual bool init();
    static cocos2d::CCScene* scene();
    void addMenuList();
    CREATE_FUNC(GameOverLayer);
    void addTitle();
    void addScoreBoard();
    void restartCallback(CCObject* obj);
    void shareCallback(CCObject* obj);
private:
    int currScore;
    int lastScore;
    char strScore[10];
    CCLabelTTF* currLabel;
    CCLabelTTF* highLabel;
};

#endif /* defined(__FlyingBird__GameOverScene__) */
