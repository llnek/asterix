//
//  ScoreLayer.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-12.
//
//

#include "ScoreLayer.h"
ScoreLayer::ScoreLayer(){
    nScore = 0;
}
ScoreLayer::~ScoreLayer(){}

CCScene* ScoreLayer::scene(){
    
    CCScene *scene = CCScene::create();
    ScoreLayer *layer = ScoreLayer::create();
    scene->addChild(layer);
    return scene;
}

bool ScoreLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    
    sprintf(strScore, "%d",nScore);
    label = CCLabelTTF::create(strScore, "Arial", 30);
    label->setPosition(CCPointZero);
    addChild(label);
    
    scheduleUpdate();
    return true;
}


void ScoreLayer::update(float dt){
    
    nScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("LastScore", 0);
    sprintf(strScore, "%d",nScore);
    label->setString(strScore);
}