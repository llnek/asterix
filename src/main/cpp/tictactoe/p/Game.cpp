// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "n/lib.h"
#include "MMenu.h"
#include "Ende.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
BEGIN_NS_UNAMED
class CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseClick(const c::Vec2&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  void onGUIXXX(const c::Vec2&);
  void showGrid();

public:

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void overAndDone(int winner);
  void playTimeExpired();
  void showMenu();

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onInited();

  DECL_PTR(ecs::Entity, board)
  DECL_PTR(ecs::Entity, arena)

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  arena = engine->getEntities("n/CSquares")[0];
  board = engine->getEntities("n/Grid")[0];

  showGrid();

  auto ps = CC_GEC(Players,board,"n/Players");
  auto ss = CC_GEC(GVars,arena,"n/GVars");
  auto human= CC_CSV(c::Integer,"HUMAN");
  // random start?
  auto pnum= cx::randSign() > 0 ? 2 : 1;

  if (MGMS()->isOnline()) {
    ss->pnum= 0;
    return;
  } else {
    ss->pnum= pnum;
  }

  if (ps->parr[pnum]->category == human) {
    SENDMSG("/hud/timer/show");
  }

  auto msg= j::json({
      {"category", ps->parr[pnum]->category},
      {"running", true },
      {"pnum", pnum}
      });
  SENDMSGEX("/hud/update",&msg);
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
  auto x= mc_new1(ECX, arena);
  Ende::reify(getSceneX(), x, 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto f= [=]() { cx::pop(); };
  auto m= MMenu::reify(mc_new1(MCX,f));
  cx::pushEx(m);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *t) {
  onGUIXXX(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
  onGUIXXX(loc);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUIXXX(const c::Vec2 &pos) {

  auto sel= CC_GEC(CellPos, board, "n/CellPos");
  auto css= CC_GEC(CSquares,arena,"n/CSquares");
  auto ss= CC_GEC(GVars,arena,"n/GVars");
  auto cur = ss->pnum;

  sel->cell =  -1;
  sel->px= pos.x;
  sel->py= pos.y;

  if (cur <=  0) {
    CCLOG("onGUIXXX: OOOPS, cur ============== %d", cur);
    return;
  }

  //which cell did he click on?
  auto n=0;
  F__LOOP(it, css->boxes) {
    auto &bx = *it;
    if (sel->px >= bx.left && sel->px <= bx.right &&
        sel->py >= bx.bottom && sel->py <= bx.top) {
      sel->cell= n;
      break;
    }
    ++n;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  f::emptyQueue<sstr>( MGMS()->msgQueue() );

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

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

  CCLOG("seed =\n%s", ctx->data.dump(2).c_str());

  engine = mc_new1(GEngine,pnum);

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showGrid() {
  auto css= CC_GEC(CSquares,arena, "n/CSquares");
  auto gps= mapGridPos(1);
  for (auto i=0; i < gps.size(); ++i) {
    auto s= css->sqs[i];
    auto &bx= gps[i];
    assert(s->cell == i);
    s->node->setPosition(cx::vboxMID(bx));
    addAtlasItem("game-pics", s->node);
  }
}

END_NS_UNAMED
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
                        JS_INT(msg->operator[]("category")),
        JS_INT(msg->operator[]("pnum")));
  }
  else
  if ("/player/timer/expired" == topic) {
    y->playTimeExpired();
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




