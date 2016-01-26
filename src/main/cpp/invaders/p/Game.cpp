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
#include "MMenu.h"
#include "Game.h"
#include "HUD.h"
#include "END.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)
BEGIN_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*) getSceneX()->getLayer(3); }
  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  void onPlayerKilled();
  void showMenu();
  void onEarnScore(int);

  DECL_PTR(Ship,player)
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  auto cannon = engine->getNodeList(CannonCtrlNode().typeId());
  auto ship = engine->getNodeList(ShipMotionNode().typeId());
  auto gun = CC_GNLF(Cannon, cannon, "cannon");
  auto lpr= CC_GNLF(Looper, cannon, "looper");
  auto sp= CC_GNLF(Ship, ship, "ship");
  auto cfg= MGMS()->getLCfg()->getValue();

  // pre-population objects in pools
  SCAST(GEngine*,engine)->reifyExplosions();
  SCAST(GEngine*,engine)->reifyMissiles();
  SCAST(GEngine*,engine)->reifyBombs();

  lpr->timer7 = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;

  this->motionees.push_back(sp);
  this->player=sp;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= []() { CC_DTOR()->popScene(); };
  CC_DTOR()->pushScene(MMenu::reify(mc_new1(MCX, f)));
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  this->engine= mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("xxx-explode");
  if (getHUD()->reduceLives(1)) {
    MGMS()->stop();
    ELayer::reify(getSceneX(),999);
  } else {
    this->player->inflate();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(int score) {
  getHUD()->updateScore(score);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = SCAST(GLayer*, getLayer(2));

  if (topic == "/game/player/earnscore") {
    auto msg = (j::json*) m;
    auto n= JS_INT(msg->operator[]("score"));
    y->onEarnScore(n);
  }
  else
  if (topic == "/hud/showmenu") {
    y->showMenu();
  }
  else
  if (topic == "/hud/replay") {
  }
  else
  if (topic == "/game/player/killed") {
    y->onPlayerKilled();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {

  MGMS()->reifyPool("explosions");
  MGMS()->reifyPool("aliens");
  MGMS()->reifyPool("missiles");
  MGMS()->reifyPool("bombs");

  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);

  play();
}


NS_END(invaders)

