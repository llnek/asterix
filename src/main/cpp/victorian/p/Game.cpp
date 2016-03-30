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
#include "n/Terrain.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onMouseStart(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

  virtual void onInited();
  void startGame();

  __decl_ptr(ecs::Node, _terrain)
  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::onInited() {
  _terrain = _engine->getNodes("n/Terrain")[0];
  _player = _engine->getNodes("f/CGesture")[0];
  _shared = _engine->getNodes("n/GVars")[0];

  auto bg = cx::reifySprite("background.png");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  auto wb=cx::visBox();

  bg->setAnchorPoint(cx::anchorBL());
  addAtlasItem("game-pics",bg, kBackground);

  auto repeat = cx::reifySprite("background.png");
  auto rw= CC_CSIZE(repeat).width;
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw - 1, 0);
  bg->addChild(repeat, kBackground);

  repeat = cx::reifySprite("background.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(2 * (rw - 1), 0);
  bg->addChild(repeat, kBackground);

  auto fg = cx::reifySprite("lamp.png");
  fg->setAnchorPoint(cx::anchorBL());
  addAtlasItem("game-pics", fg, kForeground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw * 4, 0);
  fg->addChild(repeat, kBackground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw * 8, 0);
  fg->addChild(repeat, kBackground);

  ss->background= bg;
  ss->foreground= fg;

  ss->jam = cx::reifySprite("jam_1.png");
  ss->hat = cx::reifySprite("hat.png");
  CC_HIDE(ss->hat);
  addAtlasItem("game-pics", ss->hat, kMiddleground);

  auto anim= c::Animation::create();
  for (auto i = 1; i <= 3; ++i) {
    auto f = cx::getSpriteFrame("jam_" + s::to_string(i) + ".png");
    anim->addSpriteFrame(f);
  }
  anim->setRestoreOriginalFrame(false);
  anim->setDelayPerUnit(0.2 / 3.0);
  anim->setLoops(-1);
  ss->jamAnimate = c::Animate::create(anim);
  CC_KEEP(ss->jamAnimate);
  addAtlasItem("game-pics", ss->jam, kBackground);

  ss->jam->setPosition(wb.right * 0.19, wb.top * 0.47);
  ss->jamMove = c::MoveTo::create(6.0,
      c::Vec2(-wb.right * 0.3, ss->jam->getPositionY()));
  CC_KEEP(ss->jamMove);

  ss->speedIncreaseInterval = 15;
  ss->speedIncreaseTimer = 0;
  ss->state = kGameIntro;

  //_intro->setVisible(true);
  //_mainMenu->setVisible(true);

  ss->jam->setPosition(wb.right * 0.19, wb.top * 0.47);
  CC_SHOW(ss->jam);
  ss->jam->runAction(ss->jamAnimate);

  cx::pauseMusic();
  cx::sfxMusic("background3", true);

  startGame();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startGame() {
    //_tutorialLabel->setVisible(false);
    //_intro->setVisible(false);
    //_mainMenu->setVisible(false);
  auto t=CC_GEC(Terrain, _terrain,"n/Terrain");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  ss->jam->runAction(ss->jamMove);
  cx::sfxPlay("start");
  t->setStartTerrain (true);
  ss->state = kGamePlay;
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  auto tap = touch->getLocation();
  onMouseStart(tap);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseStart(const c::Vec2 &loc) {

  auto tn=CC_GEC(Terrain, _terrain,"n/Terrain");
  auto py=CC_GEC(Player, _player,"f/CPixie");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");

  switch (ss->state) {

    case kGameIntro:
    break;

    case kGameOver:/*
      if (_tryAgain->boundingBox().containsPoint(tap)) {
          _hat->setVisible(false);
          _hat->stopAllActions();
          _tryAgain->setVisible(false);
          _terrain->reset();
          _player->reset();
          resetGame();
      }*/
    break;

    case kGamePlay:
      if (py->getState() == kPlayerFalling) {
        py->setFloating(!py->getFloating());
      } else {
        if (py->getState() != kPlayerDying) {
          py->setJumping(true);
          cx::sfxPlay("jump");
        }
      }
      tn->activateChimneysAt(py);
    break;

    case kGameTutorial:
      //_tutorialLabel->setString("");
      //_tutorialLabel->setVisible(false);
      tn->setStartTerrain(true);
      ss->state = kGamePlay;
    break;

    case kGameTutorialJump:
      if (py->getState() == kPlayerMoving) {
        py->setJumping(true);
        cx::sfxPlay("jump");
      }
    break;

    case kGameTutorialFloat:
      if (!py->getFloating()) {
        py->setFloating (true);
        //_running = true;
      }
    break;

    case kGameTutorialDrop:
      py->setFloating (false);
      //_running = true;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
  auto p=CC_GEC(Player, _player,"f/CPixie");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  if (ss->state == kGamePlay) {
    p->setJumping(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




