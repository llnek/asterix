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

  GLayer() {
    tMode= c::Touch::DispatchMode::ONE_BY_ONE;
  }

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onTouchStart(f::ComObj*, c::Touch*);
  virtual void onTouchEnd(f::ComObj*, c::Touch*);

  virtual void postReify();
  void startGame();

  DECL_PTR(a::NodeList, terrains)
  DECL_PTR(a::NodeList, players)
  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  terrains = engine->getNodeList(TerrainNode().typeId());
  players = engine->getNodeList(PlayerNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto py=CC_GNLF(Player,players,"player");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb=cx::visBox();

  auto background = cx::reifySprite("background.png");
  background->setAnchorPoint(cx::anchorBL());
  addAtlasItem("game-pics",background, kBackground);

  auto repeat = cx::reifySprite("background.png");
  auto rw= repeat->getContentSize().width;
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw - 1, 0);
  background->addChild(repeat, kBackground);

  repeat = cx::reifySprite("background.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(2 * (rw - 1), 0);
  background->addChild(repeat, kBackground);

  auto foreground = cx::reifySprite("lamp.png");
  foreground->setAnchorPoint(cx::anchorBL());
  addAtlasItem("game-pics",foreground, kForeground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw * 4, 0);
  foreground->addChild(repeat, kBackground);

  repeat = cx::reifySprite("lamp.png");
  repeat->setAnchorPoint(cx::anchorBL());
  repeat->setPosition(rw * 8, 0);
  foreground->addChild(repeat, kBackground);

  ss->background=background;
  ss->foreground=foreground;

  ss->jam = cx::reifySprite("jam_1.png");
  ss->hat = cx::reifySprite("hat.png");
  CC_HIDE(ss->hat);
  addAtlasItem("game-pics", ss->hat, kMiddleground);

  auto animation = c::Animation::create();
  for (auto i = 1; i <= 3; ++i) {
    auto f = cx::getSpriteFrame("jam_" + s::to_string(i) + ".png");
    animation->addSpriteFrame(f);
  }
  animation->setDelayPerUnit(0.2f / 3.0f);
  animation->setRestoreOriginalFrame(false);
  animation->setLoops(-1);
  ss->jamAnimate = c::Animate::create(animation);
  ss->jamAnimate->retain();
  addAtlasItem("game-pics", ss->jam, kBackground);

  ss->jam->setPosition(wb.right * 0.19f, wb.top * 0.47f);
  ss->jamMove = c::MoveTo::create(6.0f,
      c::Vec2(-wb.right * 0.3f, ss->jam->getPositionY()));
  ss->jamMove->retain();

  ss->speedIncreaseInterval = 15;
  ss->speedIncreaseTimer = 0;
  ss->state = kGameIntro;

  //_intro->setVisible(true);
  //_mainMenu->setVisible(true);

  ss->jam->setPosition(wb.right * 0.19f, wb.top * 0.47f);
  CC_SHOW(ss->jam);
  ss->jam->runAction(ss->jamAnimate);

  cx::pauseMusic();
  cx::sfxMusic("background3", true);

  this->motionees.push_back(py);
  startGame();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startGame() {
    //_tutorialLabel->setVisible(false);
    //_intro->setVisible(false);
    //_mainMenu->setVisible(false);
  auto t=CC_GNLF(Terrain,terrains,"terrain");
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->jam->runAction(ss->jamMove);
  cx::sfxPlay("start");
  t->setStartTerrain (true);
  ss->state = kGamePlay;
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  centerImage("game.bg");
  regoAtlas("game-pics");

  engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *touch) {
  auto tn=CC_GNLF(Terrain,terrains,"terrain");
  auto py=CC_GNLF(Player,players,"player");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto tap = touch->getLocation();

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
        py->setFloating(py->getFloating() ? false : true);
      } else {
        if (py->getState() != kPlayerDying) {
          cx::sfxPlay("jump");
          py->setJumping(true);
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
        cx::sfxPlay("jump");
        py->setJumping(true);
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

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *touch) {
  auto p=CC_GNLF(Player,players,"player");
  auto ss=CC_GNLF(GVars,shared,"slots");
  if (ss->state == kGamePlay) {
    p->setJumping(false);
  }
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




