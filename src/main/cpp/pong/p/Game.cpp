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
#include "s/Collide.h"
#include "s/Motion.h"
#include "s/Move.h"
#include "s/Net.h"
#include "s/Stage.h"
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

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onTouchMoved(c::Touch*, c::Event*);
  virtual void onMouseMove(c::Event*);

  virtual int getIID() { return 2; }
  virtual void decorate();

  EFactory *factory;

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
void GLayer::ignite() {
  auto ctx = (GCXX*) getSceneX()->getCtx();
  auto e = mc_new(a::Engine);
  auto f = mc_new_1(EFactory, e);
  this->engine = e;
  this->factory=f;

  e->regoSystem(mc_new_1(Resolve, f));
  e->regoSystem(mc_new_1(Collide, f));
  e->regoSystem(mc_new_1(Move, f));
  e->regoSystem(mc_new_1(Motion, f));
  e->regoSystem(mc_new_1(Net, f));
  e->regoSystem(mc_new_1(Stage, f));
  e->forceSync();
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

  ignite();

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

  initPlayers();

  getHUD()->regoPlayers(p1c, p1k, p1n, p2c, p2k, p2n);
  getHUD()->reset();

  this->options->setObject(CC_BOOL(false), "poked");
  this->options->setObject(CC_INT(pnum), "pnum");
  CCLOG("init-game - ok");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initPlayers() {

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

  p1= new cobjs.Player(p1cat, csts.CV_X, 1, csts.P1_COLOR);
  p2= new cobjs.Player(p2cat, csts.CV_O, 2, csts.P2_COLOR);
  this.options.players = [null, p1, p2];
  this.options.colors={};
  this.options.colors[csts.P1_COLOR] = p1;
  this.options.colors[csts.P2_COLOR] = p2;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updatePoints(scores) {
  getHUD()->updateScores(scores);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onWinner(p, int score) {
  getHUD()->updateScore(p,score);
  int win=0;
  if (getHUD()->isDone(&win)) {
    doDone( win);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::doDone(p) {
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
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {

  auto y = (GLayer*) getLayer(3);
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
    y->replay();
  }
  else
  if (topic == "/hud/score/update") {
    y->onWinner(msg.color, msg.score);
  }
  if ("/hud/score/sync") {
    y->updatePoints(msg.points);
  }
  else
  if (topic == "/hud/end") {
    y->doDone(msg.winner);
  }

}





NS_END(pong)

