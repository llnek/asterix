//
//  MenuScene.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#include "MenuScene.h"
#include "MainScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

MenuLayer::MenuLayer(){
}
MenuLayer::~MenuLayer(){}

CCScene* MenuLayer::scene(){
    
    CCScene *scene = CCScene::create();
    MenuLayer *layer = MenuLayer::create();
    scene->addChild(layer);
    return scene;
}

bool MenuLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    
    CCTMXTiledMap* menuBg = CCTMXTiledMap::create("bg0.tmx");
    menuBg->setPosition(ccp(0, 0));
    addChild(menuBg,0);
    addMenuList();
    // preload music
    CocosDenshion::SimpleAudioEngine *audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    //audioEngine->preloadBackgroundMusic("bg.mp3");
    audioEngine->preloadBackgroundMusic("gameOver.mp3");
    audioEngine->preloadEffect("jump.mp3");
    audioEngine->preloadEffect("getPropes.mp3");
    audioEngine->preloadEffect("dead.mp3");
    audioEngine->preloadEffect("opening_sound.mp3");
    audioEngine->preloadEffect("speed.mp3");
    audioEngine->preloadEffect("times.mp3");
    audioEngine->setBackgroundMusicVolume(0.3);
    audioEngine->setEffectsVolume(0.3);
    return true;
}

void MenuLayer::addMenuList(){
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemImage* startBtn = CCMenuItemImage::create("kaishi-1.png", "kaishi-2.png", this, menu_selector(MenuLayer::startCallback));
    startBtn->setPosition(CCPoint(-100,0));
    CCMenuItemImage* scoreBtn = CCMenuItemImage::create("fenshu-1.png", "fenshu-2.png", this, menu_selector(MenuLayer::scoreCallback));
    scoreBtn->setPosition(CCPoint(startBtn->getPositionX()+200,0));
    
    CCMenu* menu = CCMenu::create(startBtn,scoreBtn,NULL);
    menu->setPosition(CCPoint(size.width/2,size.height/2));
    addChild(menu,1);
}

void MenuLayer::startCallback(CCObject* obj){
    //CCLOG("MenuLayer::start");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CCDirector::sharedDirector()->pushScene(MainLayer::scene());
}

void MenuLayer::scoreCallback(CCObject* obj){
    //CCLOG("MenuLayer::score");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CCDirector::sharedDirector()->pushScene(GameOverLayer::scene());
}