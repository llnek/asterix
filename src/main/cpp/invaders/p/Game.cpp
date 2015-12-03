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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/EFactory.h"
#include "s/Stager.h"
#include "s/Motions.h"
#include "s/Move.h"
#include "s/Collide.h"
#include "s/Resolve.h"
#include "s/Aliens.h"
#include "Menu.h"
#include "Game.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GLayer : public f::GameLayer {
protected:


  EFactory* fac;

  DECLCZ(GLayer)
  NOCPYASS(GLayer)

public:

  virtual int getIID() { return 2; }

  virtual f::XLayer* realize();
  HUDLayer* getHUD();
  void reset();
  void play();

  void onPlayerKilled();
  void onEarnScore(int);
  void onStop();
  void spawnPlayer();

  bool playable;
  STATIC_REIFY_LAYER(GLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer* GLayer::getHUD() {
  return (HUDLayer*) MGMS()->getLayer(3);
}

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
  mc_del_ptr(fac)
}

//////////////////////////////////////////////////////////////////////////////
//
GLayer::GLayer() {
  playable=false;
  SNPTR(fac)
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GLayer::realize() {
  centerImage("game.bg");
  enableListeners();
    cx::resumeAudio();
  reset();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  mc_del_ptr(this->engine)
  mc_del_ptr(this->fac)
  CC_DROP(this->options)

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
  }

  MGMS()->resetPools();
  getHUD()->reset();

  auto e= mc_new(a::Engine);
  auto d= CC_DICT();
  auto f= new EFactory(e, d);

  e->regoSystem(new Stager(f, d));
  e->regoSystem(new Motions(f, d));
  e->regoSystem(new Move(f, d));
  e->regoSystem(new Aliens(f, d));
  e->regoSystem(new Collide(f, d));
  e->regoSystem(new Resolve(f, d));
  e->forceSync();

  CC_KEEP(d)

  this->options= d;
  this->fac= f;
  this->engine=e;

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnPlayer() {
  fac->bornShip();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("xxx-explode");
  if (getHUD()->reduceLives(1)) {
    onStop();
  } else {
    spawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onStop() {
  disableListeners();
  cx::pauseAudio();
  MGMS()->stop();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(int score) {
  getHUD()->updateScore(score);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic& topic, void* msg) {

  auto y = SCAST(GLayer*, getLayer(2));

  if (topic == "/game/player/earnscore") {
    j::json* js = (j::json*) msg;
    auto n= js->operator[]("score").get<j::json::number_integer_t>();
    y->onEarnScore(n);
  }
  else
  if (topic == "/hud/showmenu") {
    auto f= []() { CC_DTOR()->popScene(); };
    CC_DTOR()->pushScene(MainMenu::reifyWithBackAction(f));
  }
  else
  if (topic == "/hud/replay") {
  }
  else
  if (topic == "/game/player/killed") {
    y->onPlayerKilled();
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  static_cast<GLayer*>(getGLayer())->playable = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  static_cast<GLayer*>(getGLayer())->playable = true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
  return static_cast<GLayer*>(getGLayer())->playable;
}

//////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////
//
f::GameScene* Game::realizeWithCtx(f::GContext* ctx) {

  auto h = HUDLayer::reify();
  auto g = GLayer::reify();

  reifyPool("explosions");
  reifyPool("aliens");
  reifyPool("missiles");
  reifyPool("bombs");

  this->context= ctx;
  addLayer(g, 2);
  addLayer(h, 3)->realize();

  // realize game layer last
  g->realize();

  play();
  return this;
}


NS_END(invaders)

