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
#include "n/GNodes.h"
#include "HUD.h"
#include "Game.h"
#include "MMenu.h"
#include "End.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  void onMotion(f::ComObj*, const c::Vec2&, const c::Vec2&);

  virtual void onTouchMotion(f::ComObj*,
      const c::Vec2&, const c::Vec2&);

  virtual void onMouseMotion(f::ComObj*,
      const c::Vec2&, const c::Vec2&);

  virtual int getIID() { return 2; }
  virtual void decoUI();

  void deco(int pnum,
      const sstr &p1k, const sstr &p1n,
      const sstr &p2k, const sstr &p2n);

  void showMenu();
  void doDone(int);

  void onWinner(const sstr&, int, int );
  void updatePoints(j::json);

  DECL_PTR(a::NodeList, paddleNode)
  DECL_PTR(a::NodeList, arenaNode)

  virtual ~GLayer() {}
  GLayer() {}
  NOCPYASS(GLayer)

  DECL_GETLAYER(HUDLayer,getHUD,3)
  STATIC_REIFY_LAYER(GLayer)
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
void GLayer::onTouchMotion(f::ComObj *c,
    const c::Vec2 &loc, const c::Vec2 &dt) {
  onMotion(c,loc,dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(f::ComObj *c,
      const c::Vec2 &loc, const c::Vec2 &dt) {
  onMotion(c,loc,dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMotion(f::ComObj *co,
    const c::Vec2 &loc, const c::Vec2 &delta) {

  auto box= MGMS()->getEnclosureBox();
  auto p= SCAST(Paddle*,co);
  auto pos = p->pos();
  auto pnum=p->pnum;
  auto x= pos.x;
  auto y= pos.y;

  if (cx::isPortrait()) {
    if ((pnum == 2 && loc.y > pos.y) ||
        (pnum == 1 && loc.y < pos.y)) {
      x = pos.x + delta.x;
    }
  } else {
    if ((pnum == 2 && loc.x > pos.x) ||
        (pnum == 1 && loc.x < pos.x)) {
      y = pos.y + delta.y;
    }
  }

  auto cur= cx::clamp(c::ccp(x,y), box);
  p->setPos(cur.x, cur.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  centerImage("game.bg");

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
    incIndexZ();
  }

  auto ctx = (GCXX*) MGMS()->getCtx();
  auto ppids = ctx->data["ppids"];
  auto pnum= JS_INT(ctx->data["pnum"]);
  sstr p2k;
  sstr p1k;
  sstr p2n;
  sstr p1n;

  J__LOOP(it, ppids) {
    auto &arr=  it.value();
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
      throw "bad pnum " + s::to_string(n);
    }
  }

  deco(pnum, p1k, p1n, p2k, p2n);
  getHUD()->reset();

  CCLOG("seed =\n%s", ctx->data.dump(0).c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::deco(int cur, const sstr &p1k, const sstr &p1n,
    const sstr &p2k, const sstr &p2n) {
  auto e= mc_new(GEngine);
  auto a= e->mkArena(cur);
  int p1cat,p2cat;

  switch (MGMS()->getMode()) {
    case f::GMode::NET:
      p2cat = CC_CSV(c::Integer, "NETP");
      p1cat = CC_CSV(c::Integer, "NETP");
    break;
    case f::GMode::ONE:
      p1cat= CC_CSV(c::Integer, "HUMAN");
      p2cat= CC_CSV(c::Integer, "BOT");
    break;
    case f::GMode::TWO:
      p2cat= CC_CSV(c::Integer, "HUMAN");
      p1cat= CC_CSV(c::Integer, "HUMAN");
    break;
  }

  Player p1(p1cat, CC_CSV(c::Integer, "CV_X"), 1, CC_CSS("P1_COLOR"));
  Player p2(p2cat, CC_CSV(c::Integer, "CV_O"), 2, CC_CSS("P2_COLOR"));
  auto ps= (Players*) a->get("n/Players");

  p2.setName(p2k,p2n);
  p1.setName(p1k,p1n);
  ps->parr[2]=p2;
  ps->parr[1]=p1;

  e->mkOnePaddle(cur, p2, 0, 0);
  e->mkBall(0,0);
  e->mkOnePaddle(cur, p1, 0, 0);
  e->forceSync();

  this->paddleNode= e->getNodeList(PaddleNode().typeId());
  this->arenaNode= e->getNodeList(ArenaNode().typeId());
  this->engine=e;

  //add the motionables
  for (auto n=paddleNode->head;n;n=n->next) {
    auto p= CC_GNF(Paddle,n,"paddle");
    this->motionees.push_back(p);
  }

  getHUD()->regoPlayers(p1,p2);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updatePoints(j::json scores) {
  getHUD()->updateScores(scores);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onWinner(const sstr &color, int pnum, int score) {
  getHUD()->updateScore(color,pnum,score);
  int win= getHUD()->isDone();
  if (win > 0) {
    doDone( win);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::doDone(int p) {
  getHUD()->drawResult(p);
  getHUD()->endGame();
  surcease();
  cx::sfxPlay("game_end");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {

}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = SCAST(GLayer*, getLayer(3));
  auto msg= (j::json*) m;

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
    y->onWinner(
        JS_STR(msg->operator[]("color")),
        JS_INT(msg->operator[]("pnum")),
        JS_INT(msg->operator[]("score")));
  }
  else
  if (topic == "/hud/score/sync") {
    y->updatePoints(msg->operator[]("points"));
  }
  else
  if (topic == "/hud/end") {
    y->doDone(
        //JS_STR(msg->operator[]("winner")),
        JS_INT(msg->operator[]("winner")));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this,3);
  GLayer::reify(this,2);
  play();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
  return state > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  state= 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  state = 911;
}


NS_END(pong)

