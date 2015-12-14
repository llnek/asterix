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

#include "core/CCSX.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GLayer : public f::XLayer {
public:

  HUDLayer* getHUD() { return (HUDLayer*) MGMS()->getLayer(3); }

  virtual int getIID() { return 2; }
  virtual void decorate();

  virtual void onTouchEnded(c::Touch*, c::Event*);
  virtual void onMouseUp(c::Event*);

  STATIC_REIFY_LAYER(GLayer)
  NOCPYASS(GLayer)

  virtual ~GLayer() {}
  GLayer() {}

  void endGame();
  void reset();

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  F__LOOP(it, atlases) { it->second->removeAllChildren(); }

  centerImage("game.bg");

  regoAtlas("game-pics");
  regoAtlas("lang-pics");

  getHUD()->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  getHUD()->endGame();
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic& topic, void* msg) {
  GLayer* y = (GLayer*) getLayer(2);
  j::json* json= (j::json*) msg;

  if ("/hud/end" == topic) {
    y->endGame();
  }

  if ("/hud/score/update" == topic) {
    auto v= json->operator[]("score").get<j::json::number_integer_t>();
    y->getHUD()->updateScore(v);
  }

  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
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



