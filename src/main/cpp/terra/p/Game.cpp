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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

BEGIN_NS_UNAMED()
//this.regoAtlas('back-tiles', 1);
//this.regoAtlas('game-pics', 0);

class CC_DLL GLayer : public f::GameLayer {
protected:

  void inizGame();

public:

  virtual int getIID() { return 2; }

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::inizGame() {
  const arr= [['op-pics', true],
    ['game-pics', false],
    ['explosions', true]];

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  auto a= regoAtlas("explosions");
  a->setBlendFunc(c::BlendFunc::ADD);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(c::BlendFunc::ADD);

  getHUD()->reset();
}

void GLayer::initBackTiles() {
  moveBackTiles();
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5.0f);
}

GLayer::moveBackTiles() {
  auto ps= XCFG()->getPool("BackTiles");
  auto wz= cx::visRect();
  auto tm = ps->get();

  if (ENP(tm)) {
    factory->createBackTiles();
    tm= ps->get();
  }

  tm->inflate(CC_RANDOM_1(wz.width),
               wz.height);

  auto move = c::MoveBy::create(CC_RANDOM(2) + 10,
                   ccp(0, -wz.height - wz.height * 0.5f));
  auto fun = c::CallFunc::create([=]() {
    tm->deflate();
  });

  tm->sprite->runAction(c::Sequence::create(move,fun));
}

void GLayer::play() {

  inizGame();
  initAsh();

  options->setObject(CC_INT(0), "secCount");
  cx::sfxMusic("bgMusic", true);

  schedule([=]() {
    // counter used to spawn enemies
    ++this->options.secCount;
  },1.0f);
}

void GLayer::spawnPlayer() {
  bornShip();
}

void GLayer::onPlayerKilled() {
  //sh.sfxPlay('xxx-explode');
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    spawnPlayer();
  }
}

void GLayer::onNewGame(f::GMode mode) {
  //sh.sfxPlay('start_game');
  setMode(mode);
  play();
}

void GLayer::onEarnScore(j::json* msg) {
  auto n= msg->operator[]("score").get<j::json::number_integer_t>();
  getHUD()->updateScore(n);
}

void GLayer::onDone() {
  cx::pauseAudio();
  playable=false;
}

const f::Box4 GLayer::getEnclosureBox() {
  auto wb= cx::visBox();
  return Box4( wb.top + 10, wb.right, wb.bottom, wb.left);
}

GLayer::~GLayer() {

}

GLayer::GLayer() {

}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return (f::GameLayer*) getLayer(2);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const stdstr& topic, void* msg) {

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
    y->replay();
  }

  if ("/game/players/killed" == topic) {
    y->onPlayerKilled(json);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Game* Game::reify(f::GMode m) {
  auto g = f::reifyRefType<Game>();
  g->setMove(m);
  g->realize();
  return g;
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::realize() {
  auto y = f::reifyRefType<GLayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
Game::Game() {

}

//////////////////////////////////////////////////////////////////////////////
//
Game::~Game() {

}


NS_END(terra)


