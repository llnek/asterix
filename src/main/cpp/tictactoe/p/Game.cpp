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
class CC_DLL GLayer : public f::GameLayer {
friend class Game;
private:

  void onGUIXXX(const Vec2& );
  HUDLayer* getHUD();
  void inizGame();
  void mkAsh();
  void reset();
  //void onGameOver();

  NO__CPYASS(PlayLayer)

  a::NodeList* boardNode;
  EFactory* factory;
  bool playable;

public:

  virtual void sendMsg(const stdstr& topic, void* msg);
  virtual void play();
  virtual void stop();
  virtual int getIID() { return 2; }

  virtual f::XLayer* realize();

  //void onPlayerKilled();
  //void onEarnScore(int);
  //void spawnPlayer();

  DECL_CTOR(PlayLayer)
};

//////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////
//
GLayer::GLayer() {
  playable=false;
}

//////////////////////////////////////////////////////////////////////////
//
HUDLayer* GLayer::getHUD() {
  return static_cast<HUDLayer*>( getSceneX()->getLayer(3));
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GLayer::realize() {
  enableEventHandlers();
  cx::pauseAudio();
  playable=false;
  inizGame();
  scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::inizGame() {

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

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {

}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::mkAsh() {
  auto e = a::Engine::reify();
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  CC_DROP(this->options)
  this->options= d;
  this->engine = e;
  this->factory=f;
  CC_KEEP(this->options)

  e->regoSystem(new Resolve(f, d));
  e->regoSystem(new Stager(f, d));
  e->regoSystem(new Motion(f, d));
  e->regoSystem(new Logic(f, d));

  BoardNode n;
  boardNode = e->getNodeList(n.typeId());

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::replay() {
  if (MGMS()->isLive()) {
    // request server to restart a new game
    ws::netSend(MGMS()->wsock(), new ws::OdinEvent(
      ws::MType::SESSION,
      ws::EType::REPLAY
    ));
  } else {
    inizGame();
    reset(false);
    mkAsh();
    getSceneX()->resume();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateHUD() {
  if (! this->playable) {
    getHUD()->drawResult(CC_GDV(c::Integer, this->options, "lastWinner"));
  } else {
    getHUD()->drawStatus(CC_GDV(c::Integer, this->options, "pnum"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::playTimeExpired() {
  MGMS()->msgQueue().push("forfeit");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::overAndDone(int winner) {
  getHUD()->endGame(winner);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::update(float dt) {
  if (playable && NNP(engine)) {
    engine->update(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchBegan(c::Touch*, c::Event*) {
  return true;
}
void GLayer::onTouchMoved(c::Touch*, c::Event*) {
}
void GLayer::onTouchEnded(c::Touch* t, c::Event*) {
  onGUIXXX(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::initTouch() {
  auto tc= c::EventListenerTouchOneByOne::create();
  addListener(tc);

  tc->onTouchBegan = CC_CALLBACK_2(GLayer::onTouchBegan, this);
  tc->onTouchMoved = CC_CALLBACK_2(GLayer::onTouchMoved, this);
  tc->onTouchEnded = CC_CALLBACK_2(GLayer::onTouchEnded, this);
  tc->setSwallowTouches(true);
  touch =tc;
  CCLOG("init-touch: listener = %p", touch);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  onGUIXXX(evt->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initMouse() {
  mouse = c::EventListenerMouse::create();
  addListener(mouse);
  mouse->onMouseUp = CC_CALLBACK_1(GLayer::onMouseUp, this);
  CCLOG("init-mouse: listener = %p", mouse);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(const Vec2& pos) {
  if (NNP(boardNode) &&
      NNP(boardNode->head)) {
    auto sel= CC_GNF(UISelection, boardNode->head, "selection");
    sel->px= pos.x;
    sel->py= pos.y;
    sel->cell =  -1;
  }
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsg(const stdstr& topic, void* msg) {
  auto y= (GLayer*) getGLayer();

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
  else
  if ("/hud/timer/show" == topic) {
    y->getHUD()->showTimer();
  }
  else
  if ("/net/restart" == topic) {
    y->getHUD()->killTimer();
    y->play();
  }
  else
  if ("/net/stop" == topic) {
    auto p= (j::Json*) msg;
    y->overAndDone(p["status"].bool_value());
  }
  else
  if ("/hud/timer/hide" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto p = (j::Json*) msg;
    y->getHUD()->updateScore(p["color"].int_value(), p["score"].int_value());
  }
  else
  if ("/hud/end" == topic) {
    auto p = (j::Json*) msg;
    y->overAndDone( p["winner"].int_value());
  }
  else
  if ("/hud/update" == topic) {
    auto p= (j::Json*) msg;
    y->getHUD()->draw(p["running"].bool_value(), p["pnum"].int_value());
  }
  else
  if ("/player/timer/expired" == topic) {
    y->playTimeExpired();
  }
  else
  if ("/game/stop" == topic) {
    y->playable=false;
  }
  else
  if ("/game/play" == topic) {
    y->playable=true;
  }
}


//////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  sendMsg("/game/stop");
}

//////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  sendMsg("/game/play");
}

//////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return static_cast<f::GameLayer*>( getLayer(2));
}

//////////////////////////////////////////////////////////////////////////
//
f::XScene* Game::realize() {

  auto h = f::ReifyRefType<HUDLayer>();
  auto g = f::ReifyRefType<GLayer>();
  auto b = f::ReifyRefType<BGLayer>();

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

