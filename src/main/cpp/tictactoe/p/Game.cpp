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
#include "core/CCSX.h"
#include "core/Odin.h"
#include "HUD.h"
#include "END.h"
#include "Game.h"
#include "Menu.h"
#include "End.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)


BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void postReify();

  void onGUIXXX(const c::Vec2& );
  void playTimeExpired();
  void overAndDone(int);
  void updateHUD();

  void showMenu();
  void reset();
  void deco();

  MDECL_GET_LAYER(HUDLayer, getHUD, 3)

  DECL_PTR(a::NodeList, boardNode)
  DECL_PTR(a::NodeList, arenaNode)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_GET_IID(2)
  MDECL_DECORATE()
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { CC_DTOR()->popScene(); };
  auto m= MMenu::reify(mc_new_1(MCX,f));
  CC_DTOR()->pushScene(m);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  f::emptyQueue<sstr>( MGMS()->msgQueue() );

  centerImage(this,"game.bg");
  incIndexZ();

  regoAtlas(this,"game-pics");
  regoAtlas(this,"lang-pics");

  auto ctx = (GCXX*) getSceneX()->getCtx();
  auto pnum= JS_INT(ctx->data["pnum"]);
  auto ppids = ctx->data["ppids"];
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  sstr p1k;
  sstr p2k;
  sstr p1n;
  sstr p2n;
  J__LOOP(it, ppids) {
    auto &arr= it.value() ;
    if (JS_INT(arr[0]) == 1) {
      p1n= JS_STR(arr[1]);
      p1k= it.key();
    } else {
      p2n= JS_STR(arr[1]);
      p2k= it.key();
    }
  }

  CCLOG("seed =\n%s", ctx->data.dump(0).c_str());

  this->engine = mc_new_1(GEngine,pnum);

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  getHUD()->reset();

  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  ArenaNode a;
  BoardNode n;
  arenaNode = this->engine->getNodeList(a.typeId());
  boardNode = this->engine->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::reset() {
  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateHUD() {
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  if (! MGMS()->isLive()) {
    getHUD()->drawResult(ss->lastWinner);
  } else {
    getHUD()->drawStatus(ss->pnum);
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
  surcease();
  ELayer::reify(getSceneX(),999);
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

  auto sel= CC_GNLF(CellPos, boardNode, "cellpos");
  auto view = CC_GNLF(PlayView, boardNode, "view");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto cur = ss->pnum;
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
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
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
  }
  else
  if ("/net/stop" == topic) {
    auto msg= (j::json*) m;
    y->overAndDone(
        JS_BOOL(msg->operator[]("status")));
  }
  else
  if ("/hud/timer/hide" == topic) {
    y->getHUD()->killTimer();
  }
  else
  if ("/hud/score/update" == topic) {
    auto msg = (j::json*) m;
    y->getHUD()->updateScore(
        JS_INT(msg->operator[]("pnum")),
        JS_INT(msg->operator[]("score")));
  }
  else
  if ("/hud/end" == topic) {
    auto msg = (j::json*) m;
    y->overAndDone(
        JS_INT(msg->operator[]("winner")));
  }
  else
  if ("/hud/update" == topic) {
    auto msg= (j::json*) m;
    y->getHUD()->draw(
        JS_BOOL(msg->operator[]("running")),
        JS_INT(msg->operator[]("pnum")));
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
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}


NS_END(tttoe)


