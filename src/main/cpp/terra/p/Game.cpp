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

  void onPlayerKilled();
  void spawnPlayer();
  void moveBackTiles();
  void initBackTiles();
  void onEarnScore(int );
  void onDone();

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackTiles() {
  moveBackTiles();
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5.0f);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveBackTiles() {
  auto ps= XCFG()->getPool("BackTiles");
  auto wz= cx::visRect();
  auto tm = ps->get();

  if (ENP(tm)) {
    factory->createBackTiles();
    tm= ps->get();
  }

  tm->inflate(cx::randFloat(wz.width), wz.height);

  auto move = c::MoveBy::create(cx::randInt(2) + 10,
                   ccp(0, -wz.height - wz.height * 0.5f));
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
  a->setBlendFunc(BLFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BLFUNC::ADDITIVE);

  MGMS()->reifyPool("BackTiles");
  MGMS()->reifyPool("BackSkies");

  MGMS()->reifyPool("Missiles");
  MGMS()->reifyPool("Baddies");
  MGMS()->reifyPool("Bombs");

  MGMS()->reifyPool("Explosions");
  MGMS()->reifyPool("Sparks");
  MGMS()->reifyPool("HitEffects");

  factory->createBackSkies();

  sharedExplosion();
  initBackSkies();

  factory->createBackTiles();
  MGMS()->sendMsg("/game/backtiles");

  factory->createMissiles();
  factory->createBombs();
  factory->createEnemies();

  factory->createExplosions();
  factory->createSparks();
  factory->createHitEffects();

  bornShip(ships->head->ship);

  schedule([=]() {
    // counter used to spawn enemies
    ++this->options.secCount;
  },1.0f);

  getHUD()->reset();

  cx::sfxMusic("bgMusic", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnPlayer() {
  bornShip();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  //sh.sfxPlay('xxx-explode');
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    spawnPlayer();
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
  return Box4( wb.top + 10, wb.right, wb.bottom, wb.left);
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
    y->onPlayerKilled(json);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  GLayer::reify(this);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isAlive() {
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


