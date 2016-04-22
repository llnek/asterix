//
//  MainScene.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-8.
//
//

#include "MainScene.h"
#include "PauseLayer.h"
#include "ScoreLayer.h"
#include "GameOverScene.h"

MainLayer::MainLayer(){
    isPauseState = false;
    isCalledBefore = false;
    isGameOver = false;
}
MainLayer::~MainLayer(){}

CCScene* MainLayer::scene(){

    CCScene *scene = CCScene::create();
    MainLayer *layer = MainLayer::create();
    scene->addChild(layer);
    return scene;
}

bool MainLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg.mp3",true);
    //add PhysicWorld
    pwLayer = PhysicWorldLayer::create();
    addChild(pwLayer,0);
    
    //add pause layer
    PauseLayer* pauseLayer = PauseLayer::create();
    pauseLayer->setColor(ccc3(80,80, 80));
    pauseLayer->setOpacity(100);
    pauseLayer->setScaleY(0.8);
    pauseLayer->setPosition(CCPointZero);
    pauseLayer->setVisible(false);
    addChild(pauseLayer,100,100);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    ScoreLayer* scoreLayer = ScoreLayer::create();
    scoreLayer->setPosition(CCPoint(size.width/2,size.height-50));
    scoreLayer->setAnchorPoint(CCPoint(0.5,1));
    addChild(scoreLayer,200,200);
    
    CCDirector::sharedDirector()->resume();
    addPauseBtn();
    
    scheduleUpdate();
    return true;
}

void MainLayer::gameOverCall(){
//    CCLOG("GameOver");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("dead.mp3");

    GameOverLayer* gameOverLayer = GameOverLayer::create();
    addChild(gameOverLayer,400);
    CCDirector::sharedDirector()->pause();
}

void MainLayer::addPauseBtn(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemImage* pauseBtn = CCMenuItemImage::create("pause.png", "continue.png", this, menu_selector(MainLayer::pauseCallback));
    pauseBtn->setAnchorPoint(CCPoint(0,1));
    pauseBtn->setPosition(CCPoint(0,0));
    
    CCMenu* menu = CCMenu::create(pauseBtn,NULL);
    menu->setAnchorPoint(CCPoint(0,1));
    menu->setPosition(CCPoint(0,size.height));
    addChild(menu,1);
}

void MainLayer::pauseCallback(cocos2d::CCObject* obj){
    
    CCDirector::sharedDirector()->pause();
    PauseLayer* pauseLayer = (PauseLayer*)getChildByTag(100);
    if (!isPauseState) {
//        CCLOG("pause");
        pauseLayer->setVisible(true);
        isPauseState = true;
    }else{
        pauseLayer->setVisible(false);
        CCDirector::sharedDirector()->resume();
        isPauseState = false;
    }
}

void MainLayer::update(float delta){
    isGameOver = CCUserDefault::sharedUserDefault()->getBoolForKey("IS_GAMEOVER");
    if (isCalledBefore!=true && isGameOver == true) {
        gameOverCall();
        isCalledBefore = true;
    }
}
    