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
#include "s/Resolve.h"
#include "s/Logic.h"
#include "s/Stager.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "HUD.h"
#include "END.h"
#include "Game.h"
#include "Menu.h"
NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BGLayer : public f::XLayer {
public:
  virtual f::XLayer* realize() {
    centerImage("game.bg");
    return this;
  }
  NOCPYASS(BGLayer)
  IMPL_CTOR(BGLayer)
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GLayer : public f::GameLayer {
public:
  void onGUIXXX(const c::Vec2& );
  void updateHUD();
  void playTimeExpired();
  void overAndDone(int);
  virtual void update(float);

  bool onTouchBegan(c::Touch*, c::Event*);
  void onTouchMoved(c::Touch*, c::Event*);
  void onTouchEnded(c::Touch*, c::Event*);

  virtual void initTouch();
  virtual void initMouse();

  void onMouseUp(c::Event*);
  void showMenu();

  HUDLayer* getHUD();

  void replay();
  void inizGame();
  void mkAsh();
  void reset();
  //void onGameOver();

  NOCPYASS(GLayer)

  a::NodeList* boardNode;
  EFactory* factory;
  bool playable;

public:

  virtual int getIID() { return 2; }
  void play();
  void stop();

  virtual f::XLayer* realize();

  //void onPlayerKilled();
  //void onEarnScore(int);
  //void spawnPlayer();

  DECL_CTOR(GLayer)
};

//////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////
//
GLayer::GLayer() {
  SNPTR(boardNode)
  SNPTR(factory)
  playable=false;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::stop() {
  playable=false;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::play() {
  playable=true;
}

//////////////////////////////////////////////////////////////////////////
//
HUDLayer* GLayer::getHUD() {
  return static_cast<HUDLayer*>( getSceneX()->getLayer(3));
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* GLayer::realize() {
  cx::pauseAudio();
  playable=false;
  enableListeners();
  inizGame();
  scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { CC_DTOR()->popScene(); };
  auto m= MainMenu::reifyWithBackAction(f);
  CC_DTOR()->pushScene(m);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::inizGame() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  f::emptyQueue<stdstr>( MGMS()->msgQueue() );

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
  }

  auto seed = XCFG()->getSeedData();
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  stdstr p1k;
  stdstr p2k;
  stdstr p1n;
  stdstr p2n;

  auto ppids = seed["ppids"];
  J__LOOP(it, ppids) {
    auto arr=  it.value() ;
    if (arr[0].get<j::json::number_integer_t>() == 1) {
      p1k= it.key();
      p1n= arr[1].get<j::json::string_t>();
    } else {
      p2k= it.key();
      p2n= arr[1].get<j::json::string_t>();
    }
  }

  CCLOG("seed =\n%s", seed.dump(0).c_str());

  mkAsh();

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  getHUD()->reset();

  this->options->setObject(CC_INT(0), "lastWinner");

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {

}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::mkAsh() {
  auto e = mc_new( a::Engine );
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  CC_DROP(this->options)
  this->options= d;
  this->engine = e;
  this->factory=f;
  CC_KEEP(this->options)

  f->reifyBoard( MGML());
  e->update(0.0f);

  e->regoSystem(new Resolve(f, d));
  e->regoSystem(new Stager(f, d));
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
    reset();
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
  //cx::runScene(EndGame::reify( MGMS()->getMode() ));
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
  onGUIXXX(t->getLocationInView());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initTouch() {
    return;
  auto tc= c::EventListenerTouchOneByOne::create();

  tc->onTouchBegan = CC_CALLBACK_2(GLayer::onTouchBegan, this);
  tc->onTouchMoved = CC_CALLBACK_2(GLayer::onTouchMoved, this);
  tc->onTouchEnded = CC_CALLBACK_2(GLayer::onTouchEnded, this);
  tc->setSwallowTouches(true);
  touch =tc;
  addListener(tc);
  CCLOG("init-touch: listener = %p", touch);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  onGUIXXX(evt->getLocationInView());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initMouse() {
  mouse = c::EventListenerMouse::create();
  mouse->onMouseUp = CC_CALLBACK_1(GLayer::onMouseUp, this);
  addListener(mouse);
  CCLOG("init-mouse: listener = %p", mouse);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(const c::Vec2& pos) {
  if (NNP(boardNode) &&
      NNP(boardNode->head)) {} else { return; }

  auto sel= CC_GNF(UISelection, boardNode->head, "selection");
  auto view = CC_GNF(PlayView, boardNode->head, "view");
  auto cur = CC_GDV(c::Integer, this->options, "pnum");
  int n=0;

  sel->cell =  -1;
  sel->px= pos.x;
  sel->py= pos.y;

  if (cur <=  0) {
    CCLOG("onGUIXXX:  OOOPS, cur ============== %d", cur);
    return;
  }

  //which cell did he click on?
  F__LOOP(it, view->boxes) {
    auto& bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }

    return;
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const stdstr& topic, void* msg) {
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
    //y->play();
  }
  else
  if ("/net/stop" == topic) {
    auto& p= * (j::json*) msg;
      y->overAndDone(p["status"].get<j::json::boolean_t>());
  }
  else
  if ("/hud/timer/hide" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto& p = * (j::json*) msg;
      y->getHUD()->updateScore(p["color"].get<j::json::string_t>(),
                               p["score"].get<j::json::number_integer_t>());
  }
  else
  if ("/hud/end" == topic) {
    auto& p = * (j::json*) msg;
      y->overAndDone( p["winner"].get<j::json::number_integer_t>());
  }
  else
  if ("/hud/update" == topic) {
    auto& p= * (j::json*) msg;
      y->getHUD()->draw(p["running"].get<j::json::boolean_t>(),
                        p["pnum"].get<j::json::number_integer_t>());
  }
  else
  if ("/player/timer/expired" == topic) {
    y->playTimeExpired();
  }
  else
  if ("/game/stop" == topic) {
    y->stop();
  }
  else
  if ("/game/play" == topic) {
    y->play();
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

  auto h = f::reifyRefType<HUDLayer>();
  auto g = f::reifyRefType<GLayer>();
  auto b = f::reifyRefType<BGLayer>();

  addLayer( static_cast<f::XLayer*>(b), 1)->realize();
  addLayer( static_cast<f::XLayer*>(g), 2);
  addLayer( static_cast<f::XLayer*>(h), 3)->realize();

  // set this to be THE main game
  bind(this);

  // realize game layer last
  g->realize();

  return this;
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
bool Game::isLive() {
 return ((GLayer*) getGLayer())->playable;
}

NS_END(tttoe)

