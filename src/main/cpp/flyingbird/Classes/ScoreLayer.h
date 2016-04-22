//
//  ScoreLayer.h
//  FlyingBird
//
//  Created by cocos on 14-2-12.
//
//

#ifndef __FlyingBird__ScoreLayer__
#define __FlyingBird__ScoreLayer__

#include "cocos2d.h"
USING_NS_CC;

class ScoreLayer : public CCLayer
{
public:
    ScoreLayer();
    ~ScoreLayer();
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(ScoreLayer);
    char strScore[10];
    int nScore;
    void update(float dt);
    CCLabelTTF* label;
};

#endif /* defined(__FlyingBird__ScoreLayer__) */
