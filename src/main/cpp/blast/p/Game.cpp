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
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  DECL_PTR(ecs::Node, _player)
  DECL_PTR(ecs::Node, _shared)

  DECL_TD(c::Rect,_boundRect)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  void createBoundary();
  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
  auto r= CC_GEC(f::CPixie,_player,"f/CPixie");
  r->setPos(loc.x,loc.y);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  auto r= CC_GEC(f::CPixie,_player,"f/CPixie");
  auto loc= touch->getLocation();
  return cx::isClicked(r->node,loc);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  auto r= CC_GEC(f::CPixie,_player,"f/CPixie");
  auto loc= touch->getLocation();
  r->setPos(loc.x, loc.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto background = f::reifyRefType<BackgroundManager>();
  addItem(background, E_LAYER_BACKGROUND);

  createBoundary();
/*

  // initially add some enemies & a powerup
  AddPowerUp();
*/
  //schedule(schedule_selector(GameWorld::Tick), 1.0f);

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createBoundary() {
  auto wb= cx::visBox();
  auto offset = 0.025;
  auto x= wb.right * offset;
  auto y= wb.top * offset;
  auto w= wb.right - wb.right * offset * 2;
  auto h= wb.top - wb.top * offset * 4;

  // calculate the position & dimension of the game's boundary
  _boundRect = c::Rect(x,y,w,h);

  // generate vertices for the boundary
  c::Vec2 vs[4] = {
    c::Vec2(x, y),
    c::Vec2(x, y + h),
    c::Vec2(x + w, y + h),
    c::Vec2(x + w, y)
  };

  // draw the boundary
  auto bdy = c::DrawNode::create();
  bdy->drawPolygon(vs, 4, c::ccc4f(0,0,0,0), 1, c::ccc4f(1,1,1,0.3));
  addItem(bdy, E_LAYER_FOREGROUND);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




