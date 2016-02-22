// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createScreen() {
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics", kMiddleground);

    _igloo = Igloo::create(this);
    _igloo->setVisible(false);
    _gameBatchNode->addChild(_igloo, kForeground);

    //create buttons
    _buttons = Vector<Sprite *>(5);

    _btnStart = Sprite::createWithSpriteFrameName("btn_start_off.png");
    _btnStart->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.45f));
    _btnStart->setVisible(false);
    _gameBatchNode->addChild(_btnStart, kForeground, kSpriteBtnStart);
    auto buttonOn = Sprite::createWithSpriteFrameName("btn_start_on.png");
    buttonOn->setAnchorPoint(Vec2(0,0));
    buttonOn->setVisible(false);
    _btnStart->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons.pushBack(_btnStart);

    _btnReset = Sprite::createWithSpriteFrameName("btn_reset_off.png");
    _btnReset->setAnchorPoint(Vec2(0.5, 1));
    _btnReset->setPosition(Vec2(_screenSize.width * 0.9f, _screenSize.height));
    _btnReset->setVisible(false);
    _gameBatchNode->addChild(_btnReset, kForeground, kSpriteBtnReset);
    buttonOn = Sprite::createWithSpriteFrameName("btn_reset_on.png");
    buttonOn->setAnchorPoint(Vec2(0,0));
    buttonOn->setVisible(false);
    _btnReset->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons.pushBack(_btnReset);

    _btnPause = Sprite::createWithSpriteFrameName("btn_pause_off.png");
    _btnPause->setAnchorPoint(Vec2(0.5, 1));
    _btnPause->setPosition(Vec2(_screenSize.width * 0.1f, _screenSize.height));
    _btnPause->setVisible(false);
    _gameBatchNode->addChild(_btnPause, kForeground, kSpriteBtnPause);
    buttonOn = Sprite::createWithSpriteFrameName("btn_pause_on.png");
    buttonOn->setAnchorPoint(Vec2(0,0));
    buttonOn->setVisible(false);
    _btnPause->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons.pushBack(_btnPause);

    _btnAgain = Sprite::createWithSpriteFrameName("btn_again_off.png");
    _btnAgain->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.45f));
    _btnAgain->setVisible(false);
    _gameBatchNode->addChild(_btnAgain, kForeground, kSpriteBtnAgain);
    buttonOn = Sprite::createWithSpriteFrameName("btn_again_on.png");
    buttonOn->setAnchorPoint(Vec2(0,0));
    buttonOn->setVisible(false);
    _btnAgain->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons.pushBack(_btnAgain);

    _btnMenu = Sprite::createWithSpriteFrameName("btn_menu_off.png");
    _btnMenu->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.3f));
    _btnMenu->setVisible(false);
    _gameBatchNode->addChild(_btnMenu, kForeground, kSpriteBtnMenu);
    buttonOn = Sprite::createWithSpriteFrameName("btn_menu_on.png");
    buttonOn->setAnchorPoint(Vec2(0,0));
    buttonOn->setVisible(false);
    _btnMenu->addChild(buttonOn, kForeground, kSpriteBtnOn);
    _buttons.pushBack(_btnMenu);

    _messages = Label::createWithBMFont("font_messages.fnt", "level", TextHAlignment::CENTER);
    _messages->setAnchorPoint(Vec2(0.5f, 0.5f));
    _messages->setVisible(false);
    _messages->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.6f));
    this->addChild(_messages, kForeground);

    _smoke = ParticleSystemQuad::create("smoke.plist");
    _smoke->setVisible(false);
    _smoke->stopSystem();
    this->addChild(_smoke, kForeground);


    _tutorialLabel = Label::createWithTTF(TUTORIAL_1, "fonts/Verdana.ttf", 25);
    _tutorialLabel->setWidth(_screenSize.width * 0.7f);
    _tutorialLabel->setHeight(_screenSize.height * 0.4f);
    _tutorialLabel->setAlignment(TextHAlignment::CENTER);
    _tutorialLabel->setPosition(Vec2 (_screenSize.width * 0.5f, _screenSize.height * 0.2f) );
    this->addChild(_tutorialLabel, kForeground);
    _tutorialLabel->setVisible(false);
}


END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




