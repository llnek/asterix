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

#include "x2d/GameLayer.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/utils.h"
#include "Game.h"
#include "Menu.h"
#include "HUD.h"
#include "s/EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual bool onTouchBegan(c::Touch*, c::Event*) {
    return true;
  }
  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void postReify();

  MDECL_GET_LAYER(HUDLayer,getHUD,3)
  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void onGUI(const c::Vec2&);
  void showMenu();
  void endGame();
  void deco();
  void reset();

  DECL_PTR(a::NodeList,arenaNode)
};


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnded(c::Touch *t, c::Event*) {
  auto pos= t->getLocationInView();
  onGUI(pos);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseUp(c::Event *e) {
  auto evt= (c::EventMouse*)e;
  auto pos= evt->getLocationInView();
  auto b= evt->getMouseButton();
  if (b == MOUSE_BUTTON_LEFT) {
    onGUI(pos);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::onGUI(const c::Vec2 &pos) {
  auto motion= CC_GNLF(Motion, arenaNode, "motion");
  auto cpad= CC_GNLF(CtrlPad, arenaNode, "cpad");
  auto &hsps= cpad->hotspots;

  if (cx::pointInBox(hsps["rr"], pos)) {
    motion->rotr=true;
  }

  if (cx::pointInBox(hsps["rl"], pos)) {
    motion->rotl=true;
  }

  if (cx::pointInBox(hsps["sr"], pos)) {
    motion->right=true;
  }

  if (cx::pointInBox(hsps["sl"], pos)) {
    motion->left=true;
  }

  if (cx::pointInBox(hsps["cd"], pos)) {
    motion->down=true;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  arenaNode= engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  centerImage("game.bg");
  incIndexZ();

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  this->engine = mc_new(GEngine);
  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  surcease();
  getHUD()->endGame();
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *msg) {
  auto y = (GLayer*) getLayer(2);

  if ("/hud/end" == topic) {
    y->endGame();
  }

  if ("/hud/score/update" == topic) {
    auto json= (j::json*) msg;
    auto v= JS_INT(json->operator[]("score"));
    y->getHUD()->updateScore(v);
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  return  cx::visBox();
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Game::isLive() {
   return state > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::play() {
  state= 911;
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::stop() {
  state= -1;
}



NS_END(tetris)



