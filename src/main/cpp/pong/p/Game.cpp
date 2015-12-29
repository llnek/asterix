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
#include "Game.h"
#include "Menu.h"
//#include "End.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onTouchMoved(c::Touch*, c::Event*);
  virtual void onMouseMove(c::Event*);
  void onGUI(const c::Vec2&, const c::Vec2&);

  virtual int getIID() { return 2; }
  virtual void decorate();

  void deco(int pnum, const sstr &p1k, const sstr &p1n,
      const sstr &p2k, const sstr &p2n);
  void deco();

  DECL_PTR(a::NodeList, paddleNode)
  DECL_PTR(a::NodeList, arenaNode)

  virtual ~GLayer();
  GLayer();
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
//////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMove(c::Event *event) {
  auto e= (c::EventMouse*)event;
  if (MOUSE_BUTTON_LEFT == e->getMouseButton()) {
    onGUI(e->getLocationInView(), e->getDelta());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMoved(c::Touch *t, c::Event *evt) {
  onGUI(t->getLocationInView(), t->getDelta());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUI(const c::Vec2 &loc, const c::Vec2 &dt) {
  for (auto node=paddleNode->head; node; node=node->next) {
    auto p= CC_GNF(Paddle,node, "paddle");
    auto box= p->sprite->getBoundingBox();
    if (box.containsPoint(loc)) {
      if (cx::isPortrait()) {
        processP(p,loc,dt);
      } else {
        processL(p,loc,dt);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::processP(Paddle *p, const c::Vec2 &loc, const c::Vec2 &delta) {
  auto wz= MGMS()->getEnclosureRect();
  auto pos = p->pos();
  auto pnum=p->pnum;
  float x,y;

  if ((pnum == 2 && loc.y > pos.y) ||
      (pnum == 1 && loc.y < pos.y)) {
    x= pos.x + delta.x;
    y= pos.y;
    auto cur= cx::clamp(
        c::ccp(x,y), ZEROPT, c::ccp(wz.width, wz.height));
    p->setPos(cur.x, cur.y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::processL(Paddle *p, const c::Vec2 &loc, const c::Vec2 &delta) {
  auto wz= MGMS()->getEnclosureRect();
  auto pnum= p->pnum;
  auto pos = p->pos();
  float x,y;

  if ((pnum == 2 && loc.x > pos.x) ||
      (pnum == 1 && loc.x < pos.x)) {
      y = pos.y + evt.delta.y;
      x=pos.x;
      auto cur= cx::clamp(
          c::ccp(x,y), ZEROPT, c::ccp(wz.width, wz.height));
      p->setPos(cur.x, cur.y);
  }
}

//////////////////////////////////////////////////////////////////////////////
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
void GLayer::deco() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  if (atlases.empty()) {
    regoAtlas("game-pics");
    regoAtlas("lang-pics");
    incIndexZ();
  }

  auto ppids = ctx->data["ppids"];
  auto pnum= ctx->data["pnum"];
  sstr p2k;
  sstr p1k;
  sstr p2n;
  sstr p1n;

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
  auto ps= a->get("n/Players");

  p1.setName(p1k,p1n);
  p2.setName(p2k,p2n);
  ps->parr[1]=p1;
  ps->parr[2]=p2;

  e->mkOnePaddle(cur, p2, 0, 0);
  e->mkBall(0,0);
  e->mkOnePaddle(cur, p1, 0, 0);

  getHUD()->regoPlayers(p1,p2);

  this->paddleNode= e->getNodeList(PaddleNode().typeId());
  this->arenaNode= e->getNodeList(ArenaNode().typeId());

  this->engine=e;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updatePoints(j::json scores) {
  getHUD()->updateScores(scores);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onWinner(const sstr &p, int score) {
  getHUD()->updateScore(p,score);
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
  //this.removeAll();
  sh.sfxPlay("game_end");
  stop();
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  return cx::visBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = (GLayer*) getLayer(3);
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
        JS_INT(msg->operator[]("score")));
  }
  else
  if (topic == "/hud/score/sync") {
    y->updatePoints(msg->operator[]("points"));
  }
  else
  if (topic == "/hud/end") {
    y->doDone(JS_INT(msg->operator[]("winner")));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  GLayer::reify(this,2);
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

