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
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "Game.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {
  MDECL_GET_LAYER(HUDLayer,getHUD,3)
  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  void onEarnScore(j::json*);
  void onPlayerKilled();
  void spawnPlayer();
  void onDone();
  void showMenu();

  DECL_PTR(a::NodeList, paddleNode)
  DECL_PTR(a::NodeList, ballNode)
  DECL_PTR(a::NodeList, arenaNode)

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  paddleNode=engine->getNodeList(PaddleMotionNode().typeId());
  ballNode=engine->getNodeList(BallMotionNode().typeId());
  arenaNode=engine->getNodeList(ArenaNode().typeId());

  motionees.push_back(CC_GNLF(Paddle,paddleNode,"paddle"));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  this->engine = mc_new(GEngine);
  regoAtlas("game-pics");
  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnPlayer() {

  SCAST(GEngine*,engine)->bornPaddle(
      CC_GNLF(Paddle,paddleNode,"paddle"),
      CC_GNLF(Ball,ballNode,"ball"));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  if (getHUD()->reduceLives(1)) {
    onDone();
  } else {
    spawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
    getHUD()->reset();
  surcease();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(j::json *msg) {
  getHUD()->updateScore(
      JS_INT(msg->operator[]("value")));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {
  auto y = (GLayer*) getLayer(2);

  if (t == "/game/bricks/killed") {
    auto msg = (j::json*)m;
    //y->onBrickKilled(msg);
  }

  if (t == "/game/players/killed") {
    y->onPlayerKilled();
  }

  if (t == "/game/players/earnscore") {
    auto msg = (j::json*)m;
    y->onEarnScore(msg);
  }

  if (t == "/hud/showmenu") {
    y->showMenu();
  }

}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto tile = CC_CSV(c::Integer, "TILE");
  auto wb= cx::visBox();

  return f::Box4(
      wb.top - JS_INT(cfg["TOP"]) * tile,
      wb.right - tile,
      tile,
      tile);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this,3);
  GLayer::reify(this,2);
  play();
}

NS_END(breakout)



