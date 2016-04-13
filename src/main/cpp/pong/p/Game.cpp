// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "s/GEngine.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "HUD.h"
#include "Game.h"
#include "MMenu.h"
#include "Ende.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseMotion(const CCT_PT&);
  virtual void onTouchMotion(c::Touch*);
  virtual void onInited();

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void initPaddles(GVars*);

  void deco(int pnum,
      const sstr &p1k, const sstr &p1n,
      const sstr &p2k, const sstr &p2n);

  void onWinner(const sstr&, int, int );
  void updatePoints(j::json);

  void showMenu();
  void onEnd(int);

  __decl_vec(ecs::Node*, _paddles)
  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(ecs::Node, _ball)

};

/*
  replay() {
    sjs.loggr.debug('replay game called');
    if (sjs.isobj(this.options.wsock)) {

      // request server to restart a new game
      this.options.wsock.send({
        type: evts.MSG_SESSION,
        code: evts.REPLAY
      });
    } else {
      this.play(false);
    }
  }
*/
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *t) {
  onMouseMotion(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {

  auto box= MGMS()->getEnclosureBox();
  auto wb= cx::visBox();

  F__LOOP(it, _paddles) {
    auto &e= *it;
    if (! e->has("f/CGesture")) {
    continue; }
    auto p= CC_GEC(Paddle,e,"n/Paddle");
    auto pos= p->pos();
    auto x= pos.x;
    auto y= pos.y;
    // player1 at the bottom
    if (y < wb.cy) {
      if (loc.y > y) { break; }
    } else {
      if (loc.y < y) { break; }
    }
    x=loc.x;
    auto cur= cx::clamp(CCT_PT(x,y), box);
    p->setPos(cur.x, cur.y);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  auto ctx = (GCXX*) MGMS()->getCtx();
  auto pnum= JS_INT(ctx->data["pnum"]);
  auto ppids = ctx->data["ppids"];
  sstr p2k;
  sstr p1k;
  sstr p2n;
  sstr p1n;

  centerImage("game.bg");
  regoAtlas("game-pics");

  J__LOOP(it, ppids) {
    auto &arr= it.value();
    auto n= JS_INT(arr[0]);
    auto s= JS_STR(arr[1]);
    switch (n) {
      case 2:
      p2k= it.key();
      p2n= s;
      break;

      case 1:
      p1k= it.key();
      p1n= s;
      break;

      default:
      throw "bad pnum " + FTOS(n);
    }
  }

  deco(pnum, p1k, p1n, p2k, p2n);

  CCLOG("seed =\n%s", ctx->data.dump(2).c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::deco(int cur, const sstr &p1k, const sstr &p1n,
    const sstr &p2k, const sstr &p2n) {

  auto p1cat= CC_CSV(c::Integer, "HUMAN");
  auto p2cat= CC_CSV(c::Integer, "BOT");
  auto vx=CC_CSV(c::Integer, "CV_X");
  auto vo=CC_CSV(c::Integer, "CV_O");
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");

  switch (MGMS()->getMode()) {
    case f::GMode::NET:
      p2cat = CC_CSV(c::Integer, "NETP");
      p1cat = CC_CSV(c::Integer, "NETP");
    break;
    case f::GMode::ONE:
    break;
  }

  Player p1(p1cat, vx, 1, p1c);
  Player p2(p2cat, vo, 2, p2c);

  p2.setName(p2k,p2n);
  p1.setName(p1k,p1n);

  _engine= mc_new3(GEngine, cur, p1, p2);
  getHUD()->regoPlayers(p1,p2);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _arena= _engine->getNodes("n/Players")[0];
  _ball= _engine->getNodes("n/Ball")[0];
  _engine->getNodes("n/Paddle", _paddles);

  //
  auto ss= CC_GEC(GVars, _arena, "n/GVars");
  auto world = MGMS()->getEnclosureBox();
  auto ps= cx::calcSize("red_paddle.png");
  auto bs= cx::calcSize("pongball.png");
  auto wb= cx::visBox();

  // position of paddles
  // portrait
  auto p2y = world.top * 0.9 - bs.height - HHZ(ps);
  auto p1y = world.top * 0.1 + bs.height + HHZ(ps);

  ss->pz= CCT_SZ( ps.width, ps.height);
  ss->bz= CCT_SZ( bs.width, bs.height);
  ss->bp= CCT_PT( wb.cx, wb.cy);

  ss->p1p= CCT_PT(wb.cx, p1y);
  ss->p2p= CCT_PT(wb.cx, p2y);

  auto ball=CC_GEC(Ball, _ball, "n/Ball");
  ball->inflate(ss->bp.x, ss->bp.y);

  initPaddles(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initPaddles(GVars *ss) {
  F__LOOP(it, _paddles) {
    auto &e = *it;
    auto p= CC_GEC(Paddle,e,"n/Paddle");
    if (p->pnum == 2) { p->inflate(ss->p2p.x, ss->p2p.y); }
    if (p->pnum == 1) { p->inflate(ss->p1p.x, ss->p1p.y); }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updatePoints(j::json scores) {
  getHUD()->updateScores(scores);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onWinner(const sstr &color, int pnum, int score) {
  getHUD()->updateScore(color, pnum, score);
  int win= getHUD()->isDone();
  if (win > 0) {
    onEnd(win);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd(int p) {
  getHUD()->drawResult(p);
  getHUD()->endGame();
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = SCAST(GLayer*, getLayer(2));

  if (topic == "/hud/showmenu") {
    y->showMenu();
  }
  else
  if (topic == "/game/restart") {
    //play();
  }
  else
  if (topic == "/game/stop") {
  }
  else
  if (topic == "/hud/replay") {
    //y->replay();
  }
  else
  if (topic == "/hud/score/update") {
    auto msg= (j::json*) m;
    y->onWinner(
        JS_STR(msg->operator[]("color")),
        JS_INT(msg->operator[]("pnum")),
        JS_INT(msg->operator[]("score")));
  }
  else
  if (topic == "/hud/score/sync") {
    auto msg= (j::json*) m;
    y->updatePoints(msg->operator[]("points"));
  }
  else
  if (topic == "/hud/end") {
    auto msg= (j::json*) m;
    y->onEnd(
        //JS_STR(msg->operator[]("winner")),
        JS_INT(msg->operator[]("winner")));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  HUDLayer::reify(this,3);
  GLayer::reify(this,2);
  play();
}


NS_END

