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
#include "BackDrop.h"
//#include "MMenu.h"
#include "n/lib.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)
BEGIN_NS_UNAMED


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseMotion(f::ComObj*, const c::Vec2&);
  virtual void postReify();

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

};

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(f::ComObj *c, const c::Vec2 &loc) {
  auto bx= MGMS()->getEnclosureBox();
  auto pos= c->pos();
  c->setPos(cx::clamp(loc, c->csize(), bx).x, pos.y);
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  auto c= loadLevel(MGMS()->getLevel());
  regoAtlas("game-pics");
  engine= mc_new1(GEngine,c);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  auto aliens = engine->getNodeList(AlienNode().typeId());
  auto ships = engine->getNodeList(ShipNode().typeId());
  auto lpr2= CC_GNLF(Looper,aliens,"looper");
  auto lpr1= CC_GNLF(Looper,ships,"looper");
  auto s= CC_GNLF(Ship,ships,"ship");

  this->motionees.push_back(s);
  spawnPlayer(s);

  // timers for bullets
  lpr2->timer = cx::reifyTimer(MGML(), 1.0f);
  lpr1->timer = cx::reifyTimer(MGML(), 1.0f);
}




END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}



NS_END



