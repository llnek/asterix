// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "x2d/GameLayer.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/Stage.h"
#include "s/Generate.h"
#include "s/Clear.h"
#include "s/Motion.h"
#include "s/Move.h"
#include "s/Resolve.h"
#include "s/utils.h"
#include "Game.h"
#include "Menu.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*) MGMS()->getLayer(3); }

  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);

  virtual int getIID() { return 2; }
  virtual void decorate();

  void endGame();
    void showMenu();
  void deco();
  void reset();

  EFactory* factory=nullptr;

  STATIC_REIFY_LAYER(GLayer)

  virtual ~GLayer() {}
  GLayer() {}

  NOCPYASS(GLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnded(c::Touch*, c::Event*) {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event*) {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  enableListeners();
  cx::resumeAudio();
  deco();
  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::deco() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  centerImage("game.bg");

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
    incIndexZ();
  }

  auto e= mc_new(a::Engine);
  auto d= CC_DICT();
  auto f= mc_new_2(EFactory, e, d);

  f->reifyArena();

  e->regoSystem(mc_new_2(Stage, f, d));
  e->regoSystem(mc_new_2(Generate, f, d));
  e->regoSystem(mc_new_2(Clear, f, d));
  e->regoSystem(mc_new_2(Motions, f, d));
  e->regoSystem(mc_new_2(Move, f, d));
  e->regoSystem(mc_new_2(Resolve, f, d));
  e->forceSync();

  this->options= d;
  CC_KEEP(d)
  this->factory= f;
  this->engine=e;

  getHUD()->reset();
}

void GLayer::showMenu() {

}
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  getHUD()->endGame();
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
  GLayer *y = (GLayer*) getLayer(2);
  j::json *json= (j::json*) msg;

  if ("/hud/end" == topic) {
    y->endGame();
  }

  if ("/hud/score/update" == topic) {
    auto v= JS_INT(json->operator[]("score"));
    y->getHUD()->updateScore(v);
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  return  cx::visBox();
}

//////////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
   return state > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  state= 911;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  state= -1;
}



NS_END(tetris)



