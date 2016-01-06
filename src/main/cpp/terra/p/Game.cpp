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
#include "s/Stage.h"
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
struct CC_DLL BLayer : public f::XLayer {

  STATIC_REIFY_LAYER(BLayer)
  MDECL_DECORATE()
};

void BLayer::decorate() {
  regoAtlas("back-tiles", 1);
  regoAtlas("game-pics", 0);
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  MDECL_GET_LAYER(HUDLayer,getHUD,3)
  STATIC_REIFY_LAYER(GLayer)
  MDECL_GET_IID(2)
  MDECL_DECORATE()

  DECL_PTR(f::ComObj, ship)
  DECL_PTR(EFactory, fac)

  void onEarnScore(j::json* );
  void incSecCount(float);
  void onPlayerKilled();
  void moveBackTiles(float);
  void initBackTiles();
  void showMenu();
  void onDone();
  void deco();

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

  tm->sprite->runAction(c::Sequence::create(move,fun,nullptr));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  MGMS()->reifyPools(s_vec<sstr> {
      "BackTiles", "BackSkies", "Missiles", "Baddies",
      "Bombs", "Explosions", "Sparks", "HitEffects"
  });

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  auto a= regoAtlas("explosions");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  incIndexZ();

  this->engine= mc_new(GEngine);

  d->setObject(CC_INT(0), "secCount");
  f->createShip();

  e->regoSystem(mc_new_2(Stage, f, d));
  e->regoSystem(mc_new_2(Motions, f, d));
  e->regoSystem(mc_new_2(Move, f, d));
  e->regoSystem(mc_new_2(Aliens, f, d));
  e->regoSystem(mc_new_2(Collide, f, d));
  e->regoSystem(mc_new_2(Resolve, f, d));
  e->regoSystem(mc_new_2(Render, f, d));
  e->forceSync();

  this->options= d;
  CC_KEEP(d)
  this->fac= f;
  this->engine=e;

  schedule(CC_SCHEDULE_SELECTOR(GLayer::incSecCount), 1.0f);

  ShipNode n;
  a::Node* s= e->getNodeList(n.typeId())->head;

  ship = CC_GNF(Ship, s, "ship");

  getHUD()->reset();
  cx::sfxMusic("bgMusic", true);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::incSecCount(float) {
  // counter used to spawn enemies
  auto n= CC_GDV(c::Integer, options, "secCount");
  options->setObject(CC_INT(n+1), "secCount");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("explodeEffect");
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    bornShip(ship);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(j::json *msg) {
  auto n= JS_INT(msg->operator[]("score"));
  getHUD()->updateScore(n);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  disableListeners();
  cx::pauseAudio();
  MGMS()->stop();
  unscheduleUpdate();
  //ELayer::reify(getSceneX(), 999);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto wb= cx::visBox();
  return f::Box4( wb.top + 10, wb.right, wb.bottom, wb.left);
}

//////////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {

  GLayer* y = (GLayer*) getLayer(2);
  j::json* json= (j::json*) msg;

  if ("/game/players/earnscore" == topic) {
    y->onEarnScore(json);
  }
  else
  if ("/game/backtiles" == topic) {
    y->initBackTiles();
  }
  else
  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
  else
  if ("/hud/replay" == topic) {
    //y->replay();
  }
  else
  if ("/game/players/killed" == topic) {
    y->onPlayerKilled();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  BLayer::reify(this, 1);
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



NS_END(terra)


