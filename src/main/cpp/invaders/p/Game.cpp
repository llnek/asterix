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

  virtual bool onTouchBegan(c::Touch*, c::Event*) { return true; }
  virtual void onTouchMoved(c::Touch*, c::Event*);
  virtual void onMouseMove(c::Event*);

  EFactory* fac;
  Ship* player;

public:

  HUDLayer* getHUD() { return (HUDLayer*) MGMS()->getLayer(3); }
  virtual int getIID() { return 2; }
  virtual void decorate();

  void reset();
  void play();

  void onPlayerKilled();
  void onEarnScore(int);
  void onStop();
  void spawnPlayer();

  STATIC_REIFY_LAYER(GLayer)
  NOCPYASS(GLayer)
  DECLCZ(GLayer)

};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
  mc_del_ptr(fac)
}

//////////////////////////////////////////////////////////////////////////////
//
GLayer::GLayer() {
  SNPTR(fac)
  SNPTR(player)
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMove(c::Event* event) {
  auto e= (c::EventMouse*)event;
  auto b= e->getMouseButton();
  if (b == MOUSE_BUTTON_LEFT) {
    auto pos= this->player->sprite->getPosition();
    auto bx= MGMS()->getEnclosureBox();
    auto loc= e->getLocationInView();
    this->player->sprite->setPosition(cx::clamp(loc, bx).x, pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMoved(c::Touch* t, c::Event* evt){
  auto pos= this->player->sprite->getPosition();
  auto bx= MGMS()->getEnclosureBox();
  auto y = pos.y;
  pos= c::ccpAdd(pos,  t->getDelta());
  pos= cx::clamp(pos, bx);
  this->player->sprite->setPosition(pos.x, y);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  centerImage("game.bg");
  enableListeners();
  cx::resumeAudio();
  reset();
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

  this->options= d;
  CC_KEEP(d)
  this->fac= f;
  this->engine=e;

  ShipMotionNode n;
  auto nl= e->getNodeList(n.typeId());
  this->player= CC_GNF(Ship, nl->head, "ship");

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
    auto f= [=]() { CC_DTOR()->popScene(); };
    CC_DTOR()->pushScene(MainMenu::reify(mc_new_1(MContext, f)));
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
  this->state = -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  this->state= 911;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
  return this->state > 0;
}

//////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {

  HUDLayer::reify(this, 3);

  reifyPool("explosions");
  reifyPool("aliens");
  reifyPool("missiles");
  reifyPool("bombs");

  GLayer::reify(this, 2);

  play();
}


NS_END(invaders)

