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
#include "s/Resolve.h"
#include "s/Collide.h"
#include "s/Move.h"
#include "s/Aliens.h"
#include "s/Motion.h"
#include "s/Render.h"
#include "HUD.h"
#include "Game.h"
#include "s/utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
class BLayer : public f::XLayer {
public:
  STATIC_REIFY_LAYER(BLayer)
  NOCPYASS(BLayer)
  IMPLCZ(BLayer)
  virtual void decorate() {
    regoAtlas("back-tiles", 1);
    regoAtlas("game-pics", 0);
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GLayer : public f::GameLayer {
public:

  virtual const f::Box4 getEnclosureBox();
  virtual int getIID() { return 2; }
  virtual void decorate();

  STATIC_REIFY_LAYER(GLayer)
  NOCPYASS(GLayer)
  IMPLCZ(GLayer)

  HUDLayer* getHUD() { return (HUDLayer*) MGMS()->getLayer(3); }

  void incSecCount(float);
  void onEarnScore(j::json* );
  void onPlayerKilled();
  void moveBackTiles(float);
  void initBackTiles();
  void showMenu();
  void onDone();

    EFactory* fac;
};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackTiles() {
  moveBackTiles(0);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5.0f);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveBackTiles(float) {
  auto ps= MGMS()->getPool("BackTiles");
  auto wz= cx::visRect();
  auto tm = ps->get();

  if (ENP(tm)) {
    fac->createBackTiles();
    tm= ps->get();
  }

  tm->inflate(cx::randFloat(wz.size.width), wz.size.height);

  auto move = c::MoveBy::create(cx::randInt(2) + 10,
                                c::ccp(0, -wz.size.height - wz.size.height * 0.5f));
  auto fun = c::CallFunc::create([=]() {
    tm->deflate();
  });

  tm->sprite->runAction(c::Sequence::create(move,fun));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  options->setObject(CC_INT(0), "secCount");

  auto a= regoAtlas("explosions");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BDFUNC::ADDITIVE);

  MGMS()->reifyPool("BackTiles");
  MGMS()->reifyPool("BackSkies");

  MGMS()->reifyPool("Missiles");
  MGMS()->reifyPool("Baddies");
  MGMS()->reifyPool("Bombs");

  MGMS()->reifyPool("Explosions");
  MGMS()->reifyPool("Sparks");
  MGMS()->reifyPool("HitEffects");

  auto e= mc_new(a::Engine);
  auto d= CC_DICT();
  auto f= new EFactory(e, d);

  e->regoSystem(new Stager(f, d));
  e->regoSystem(new Motions(f, d));
  e->regoSystem(new Move(f, d));
  e->regoSystem(new Aliens(f, d));
  e->regoSystem(new Collide(f, d));
  e->regoSystem(new Resolve(f, d));
  e->regoSystem(new Render(f, d));
  e->forceSync();

  this->options= d;
  CC_KEEP(d)
  this->fac= f;
  this->engine=e;

  schedule(CC_SCHEDULE_SELECTOR(GLayer::incSecCount), 1.0f);
  getHUD()->reset();

  cx::sfxMusic("bgMusic", true);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::incSecCount(float) {
  // counter used to spawn enemies
  //++this->options.secCount;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  //sh.sfxPlay('xxx-explode');
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    //bornShip(nullptr);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(j::json* msg) {
  auto n= msg->operator[]("score").get<j::json::number_integer_t>();
  getHUD()->updateScore(n);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  cx::pauseAudio();
  MGMS()->stop();
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 GLayer::getEnclosureBox() {
  auto wb= cx::visBox();
    return f::Box4( wb.top + 10, wb.right, wb.bottom, wb.left);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic& topic, void* msg) {

  GLayer* y = (GLayer*) getLayer(2);
  j::json* json= (j::json*) msg;

  if ("/game/players/earnscore" == topic) {
    y->onEarnScore(json);
  }

  if ("/game/backtiles" == topic) {
    y->initBackTiles();
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }

  if ("/hud/replay" == topic) {
    //y->replay();
  }

  if ("/game/players/killed" == topic) {
    y->onPlayerKilled();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  GLayer::reify(this);
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



NS_END(terra)


