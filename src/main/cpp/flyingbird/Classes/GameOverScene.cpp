//
//  GameOverScene.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#include "GameOverScene.h"
#include "MenuScene.h"

GameOverLayer::GameOverLayer(){
    currScore = 0;
    currScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore",0);
    lastScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("LastScore",0);
}
GameOverLayer::~GameOverLayer(){}

CCScene* GameOverLayer::scene(){
    
    CCScene *scene = CCScene::create();
    GameOverLayer *layer = GameOverLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameOverLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    addTitle();
    addScoreBoard();
    addMenuList();
    return true;
}

void GameOverLayer::addTitle(){
    
}

void GameOverLayer::addScoreBoard(){

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    sprintf(strScore, "Current Score: %d",lastScore);
    currLabel = CCLabelTTF::create(strScore, "Arial", 20);
    currLabel->setPosition(CCPoint(size.width/2,size.height/2+100));
    addChild(currLabel);
    
    if (lastScore > currScore) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore", lastScore);
        currScore = lastScore;
    }else{
        CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore", currScore);
    }
    
    sprintf(strScore, "Highest Score: %d",currScore);
    highLabel = CCLabelTTF::create(strScore, "Arial", 20);
    highLabel->setPosition(CCPoint(size.width/2,size.height/2+50));
    addChild(highLabel);
    CCLOG("CurrentScore:%d",currScore);
}

void GameOverLayer::addMenuList(){
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemLabel* restartBtn = CCMenuItemLabel::create(CCLabelTTF::create("ok", "Arial", 30), this, menu_selector(GameOverLayer::restartCallback));
    restartBtn->setPosition(CCPoint(-50,0));
    
    CCMenuItemLabel* shareBtn = CCMenuItemLabel::create(CCLabelTTF::create("share", "Arial", 30), this, menu_selector(GameOverLayer::shareCallback));
    shareBtn->setPosition(CCPoint(restartBtn->getPositionX()+100,0));
    
    CCMenu* menu = CCMenu::create(restartBtn,shareBtn,NULL);
    menu->setPosition(CCPoint(size.width/2,size.height/2));
    addChild(menu,1);
}

void GameOverLayer::restartCallback(CCObject* obj){
    CCDirector::sharedDirector()->pushScene(MenuLayer::scene());
}

void GameOverLayer::shareCallback(CCObject* obj){
    CCLOG("shareCallback");
}