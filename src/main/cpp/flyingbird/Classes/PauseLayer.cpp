//
//  PauseLayer.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-10.
//
//

#include "PauseLayer.h"
#include "MenuScene.h"

PauseLayer::PauseLayer(){}
PauseLayer::~PauseLayer(){}

CCScene* PauseLayer::scene(){
    
    CCScene *scene = CCScene::create();
    PauseLayer *layer = PauseLayer::create();
    scene->addChild(layer);
    return scene;
}

bool PauseLayer::init(){
    
    if (!CCLayerColor::init()) {
        return false;
    }
    
    addMenuList();
    return true;
}

void PauseLayer::addMenuList(){
//    CCLOG("PauseLayer");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemImage* pauseBtn = CCMenuItemImage::create("menu-1.png", "menu-2.png", this, menu_selector(PauseLayer::menuCallback));
    
    CCMenu* menu = CCMenu::create(pauseBtn,NULL);
    menu->setAnchorPoint(CCPoint(0,1));
    menu->setPosition(CCPoint(size.width/2,size.height/2));
    addChild(menu,1);
}

void PauseLayer::menuCallback(CCObject* obj){
    CCDirector::sharedDirector()->pushScene(MenuLayer::scene());
}