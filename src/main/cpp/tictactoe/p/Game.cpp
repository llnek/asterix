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
#include "s/EFactory.h"
#include "s/Resolve.h"
#include "s/Logic.h"
#include "s/Stage.h"
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
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);

  HUDLayer* getHUD() {
    return static_cast<HUDLayer*>( getSceneX()->getLayer(3));
  }

  void onGUIXXX(const c::Vec2& );
  void playTimeExpired();
  void overAndDone(int);
  void updateHUD();

  void showMenu();
  void reset();
  void deco();

  a::NodeList *boardNode=nullptr;
  EFactory *factory=nullptr;

  virtual int getIID() { return 2; }
  virtual void decorate();

  STATIC_REIFY_LAYER(GLayer)
  virtual ~GLayer() {}
  GLayer() {}
  NOCPYASS(GLayer)
};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  centerImage("game.bg");
  cx::resumeAudio();
  enableListeners();
  deco();
  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { CC_DTOR()->popScene(); };
  auto m= MMenu::reify(mc_new_1(MCX,f));
  CC_DTOR()->pushScene(m);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::deco() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  f::emptyQueue<sstr>( MGMS()->msgQueue() );

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
    incIndexZ();
  }

  auto ctx = (GCXX*) getSceneX()->getCtx();
  auto ppids = ctx->data["ppids"];
  auto pnum= ctx->data["pnum"];
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  sstr p1k;
  sstr p2k;
  sstr p1n;
  sstr p2n;
  J__LOOP(it, ppids) {
    auto &arr=  it.value() ;
    if (JS_INT(arr[0]) == 1) {
      p1n= JS_STR(arr[1]);
      p1k= it.key();
    } else {
      p2n= JS_STR(arr[1]);
      p2k= it.key();
    }
  }

  CCLOG("seed =\n%s", ctx->data.dump(0).c_str());

  auto e = mc_new(a::Engine);
  auto d = CC_DICT();
  auto f = new EFactory(e, d);

  CC_DROP(this->options)
  this->options= d;
  this->engine = e;
  this->factory=f;
  CC_KEEP(d)

  f->reifyBoard();

  e->regoSystem(new Resolve(f, d));
  e->regoSystem(new Logic(f, d));
  e->regoSystem(new Stage(f, d));
  e->forceSync();

  BoardNode n;
  boardNode = e->getNodeList(n.typeId());

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  getHUD()->reset();

  this->options->setObject(CC_INT(0), "lastWinner");
  this->options->setObject(CC_INT(pnum), "pnum");
  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {
  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
static void replay() {
  /*
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
  */
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateHUD() {
  if (! MGMS()->isLive()) {
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

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnded(c::Touch *t, c::Event*) {
  onGUIXXX(t->getLocationInView());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event *e) {
  auto evt = (c::EventMouse*) e;
  onGUIXXX(evt->getLocationInView());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(const c::Vec2 &pos) {

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
    auto &bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }

}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
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
    auto p= (j::json*) msg;
    y->overAndDone( JS_BOOL(p->operator[]("status")));
  }
  else
  if ("/hud/timer/hide" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto p = (j::json*) msg;
    y->getHUD()->updateScore(
        JS_INT(p->operator[]("pnum")),
        JS_INT(p->operator[]("score")));
  }
  else
  if ("/hud/end" == topic) {
    auto p = (j::json*) msg;
    y->overAndDone( JS_INT(p->operator[]("winner")));
  }
  else
  if ("/hud/update" == topic) {
    auto p= (j::json*) msg;
    y->getHUD()->draw(
        JS_BOOL(p->operator[]("running")),
        JS_INT(p->operator[]("pnum")));
  }
  else
  if ("/player/timer/expired" == topic) {
    y->playTimeExpired();
  }
  else
  if ("/game/stop" == topic) {
    MGMS()->stop();
  }
  else
  if ("/game/play" == topic) {
    MGMS()->play();
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
  return state > 0;
}

//////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  state=0;
}

//////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  state=911;
}

//////////////////////////////////////////////////////////////////////////
//
f::GameLayer* Game::getGLayer() {
  return static_cast<f::GameLayer*>( getLayer(2));
}

//////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}


NS_END(tttoe)


