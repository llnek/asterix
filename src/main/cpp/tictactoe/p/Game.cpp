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

#include "s/EFactory.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "Game.h"
NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : f::XLayer {
public:

  virtual f::XLayer* realize() {
    centerImage("game.bg");
    return this;
  }

  NO__CPYASS(BGLayer)
  IMPL_CTOR(BGLayer)
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayLayer : public f::GameLayer {
private:

  NO__CPYASS(PlayLayer)
  EFactory* factory;
  bool playable;

  HUDLayer* getHUD();
  void mkAsh();

public:

  virtual void sendMsg(const stdstr& topic, void* msg);
  virtual void reset(bool newFlag) ;
  virtual void replay() ;
  virtual void play();
  virtual void onGameOver();

  virtual f::XLayer* realize();

  void onPlayerKilled();
  void onEarnScore(int);
  void spawnPlayer();

  virtual int getIID() { return 2; }
  virtual void inizGame();

  DECL_CTOR(PlayLayer)
};

//////////////////////////////////////////////////////////////////////////
//
PlayLayer::~PlayLayer() {

}

//////////////////////////////////////////////////////////////////////////
//
PlayLayer::PlayLayer() {
  playable=false;
}

//////////////////////////////////////////////////////////////////////////
//
HUDLayer* PlayLayer::getHUD() {
  return static_cast<HUDLayer*>( getSceneX()->getLayer(3));
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* PlayLayer::realize() {
  enableEventHandlers();
  cx::pauseAudio();
  playable=false;
  inizGame();
  scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void PlayLayer::inizGame() {

  CC_LOOP(it, atlases) { it->second->removeAllChildren(); }

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
  }

  f::EmptyQueue<stdstr>( MGMS()->msgQueue() );
  mkAsh();

  auto seed = XCFG()->getSeedData();
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  auto ppids = seed["ppids"];
  stdstr p1k;
  stdstr p2k;
  stdstr p1n;
  stdstr p2n;

  CC_LOOP(it, ppids) {
    auto arr= it->second;
    if (arr[0].int_value() == 1) {
      p1k= it->first;
      p1n= arr[1].string_value();
    } else {
      p2k= it->first;
      p2n= arr[1].string_value();
    }
  }

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  getHUD()->reset();

  this->options->setObject(CC_INT(0), "lastWinner");
  this->playable=true;

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////
//
void PlayLayer::mkAsh() {
  auto e = a::Engine::reify();
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  CC_DROP(this->options)
  this->options= d;

  this->engine = e;
  this->factory=f;

  e->regoSystem(new Resolve(f, d));
  e->regoSystem(new Stager(f, d));
  e->regoSystem(new Motion(f, d));
  e->regoSystem(new Logic(f, d));

  CC_KEEP(this->options)
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayLayer::replay() {
  if (MGMS()->isLive()) {
    // request server to restart a new game
    ws::netSend(MGMS()->wsock(), new ws::OdinEvent(
      ws::MType::SESSION,
      ws::EType::REPLAY
    ));
  } else {
    inizGame();
    reset(false);
    initAsh();
    getSceneX()->resume();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayLayer::updateHUD() {
  if (this->playable) {
    getHUD()->drawStatus(CC_GDV(c::Integer, this->options, "pnum"));
  } else {
    getHUD()->drawResult(CC_GDV(c::Integer, this->options, "lastWinner"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayLayer::playTimeExpired() {
  MGMS()->msgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayLayer::overAndDone(int winner) {
  getHUD()->endGame(winner);
}

//////////////////////////////////////////////////////////////////////////
//
void PlayLayer::update(float dt) {
  if (playable && NNP(engine)) {
    engine->update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayLayer::sendMsg(const stdstr& topic, void* msg) {
  auto par = CC_PCAST(Game*);

  if ("/hud/showmenu" == topic) {
    showMenu();
  }
  else
  if ("/hud/timer/show" == topic) {
    getHUD()->showTimer();
  }
  else
  if ("/net/restart" == topic) {
    getHUD()->killTimer();
    play(false);
  }
  else
  if ("/net/stop" == topic) {
    overAndDone(msg.status);
  }
  else
  if ("/hud/timer/hide" == topic) {
    getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto p = (j::Json*) msg;
    getHUD()->updateScore(p["color"].int_value(), p["score"].int_value());
  }
  else
  if ("/hud/end" == topic) {
    auto p = (j::Json*) msg;
    overAndDone( p["winner"].int_value());
  }
  else
  if ("/hud/update" == topic) {
    auto p= (j::Json*) msg;
    getHUD()->draw(p["running"].int_value(), p["pnum"].int_value());
  }
  else
  if ("/player/timer/expired" == topic) {
    playTimeExpired();
  }
  else
  if ("/game/stop" == topic) {
    playable=false;
  }
  else
  if ("/game/play" == topic) {
    playable=true;
  }
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  getGLayer()->sendMsg("/game/stop");
}

//////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  getGLayer()->sendMsg("/game/play");
}

//////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return static_cast<f::GameLayer*>( getLayer(2));
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::realize() {

  auto g = f::ReifyRefType<GameLayer>();
  auto b = f::ReifyRefType<BGLayer>();
  auto h = f::ReifyRefType<HUDLayer>();

  addLayer( static_cast<f::XLayer*>(b), 1)->realize();
  addLayer( static_cast<f::XLayer*>(g), 2);
  addLayer( static_cast<f::XLayer*>(h), 3)->realize();

  // set this to be THE main game
  bind(this);

  // realize game layer last
  g->realize();

  return this;
}


NS_END(tttoe)

