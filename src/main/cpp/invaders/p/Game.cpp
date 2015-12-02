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
private:

  virtual f::XLayer* realize();
  void initAsh();

  NO__CPYASS(GLayer)
  EFactory* fac;

public:

  virtual void sendMsgEx(const stdstr& topic, void* msg);
  virtual void reset();
  virtual void play();
  virtual void onGameOver();

  void onPlayerKilled();
  void onEarnScore(int);
  void spawnPlayer();

  virtual int getIID() { return 2; }

  DECL_CTOR(GLayer)
};

    //CenterImage("game.bg");
//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
  delete engine;
  delete fac;
}

//////////////////////////////////////////////////////////////////////////////
//
GLayer::GLayer() {
  SNPTR(engine)
  SNPTR(fac)
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GLayer::realize() {
  play();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  mc_del_ptr(this->engine)
  mc_del_ptr(this->fac)
  SNPTR(this->options)

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
  }

  MGMS()->resetPools();
  getHUD()->reset();

  auto e= a::Engine::reify();
  auto d= CC_DICT();
  auto f= new EFactory(e, d);

  e->regoSystem(new Stager(f, d));
  e->regoSystem(new Motions(f, d));
  e->regoSystem(new Move(f, d));
  e->regoSystem(new Aliens(f, d));
  e->regoSystem(new Collide(f, d));
  e->regoSystem(new Resolve(f, d));

  f->reifyExplosions();
  f->reifyMissiles();
  f->reifyBombs();
  f->reifyAliens();
  f->reifyShip();

  e->forceSync();

  this->options= d;
  this->fac= f;
  this->engine=e;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::play() {
  //CCLOG("play game, newflag = %s", newFlag ? "true" : "false");
  reset();
  MGMS()->resume();
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
    finzGame();
  } else {
    spawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(int score) {
  getHUD()->updateScore(score);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onGameOver() {
  MGMS()->pause();
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::sendMsgEx(const stdstr& topic, void* msg) {

  if (topic == "/game/player/earnscore") {
    f::ComObj* i = (f::ComObj*) msg;
    OnEarnScore(i->score);
  }
  else
  if (topic == "/hud/showmenu") {

    auto f= []() { CC_DTOR()->popScene(); };
    auto a= c::CallFunc::create(f);
    CC_DTOR()->pushScene(MainMenu::ReifyWithBackAction(a));
  }
  else
  if (topic == "/hud/replay") {
    Replay();
  }
  else
  if (topic == "/game/player/killed") {
    OnPlayerKilled();
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Resume() {
  running= true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Run() {
  running= true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::Pause() {
  running= false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::IsRunning() {
  return running;
}

//////////////////////////////////////////////////////////////////////////
//
Game::~Game() {
}

//////////////////////////////////////////////////////////////////////////
//
Game::Game() {
}

//////////////////////////////////////////////////////////////////////////
//
f::XGameLayer* Game::GetGLayer() {
  return (f::XGameLayer*) GetLayer(2);
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::Realize() {

  auto g = (f::XLayer*) f::ReifyRefType<GameLayer>();
  auto b = (f::XLayer*) f::ReifyRefType<BGLayer>();
  auto h = (f::XLayer*) f::ReifyRefType<HUDLayer>();

  ReifyPool("explosions");
  ReifyPool("aliens");
  ReifyPool("missiles");
  ReifyPool("bombs");

  AddLayer(b, 1)->Realize();
  AddLayer(g, 2);
  AddLayer(h, 3)->Realize();

  // set this to be THE main game
  Bind(this);

  // realize game layer last
  g->Realize();

  return this;
}

NS_END(invaders)

