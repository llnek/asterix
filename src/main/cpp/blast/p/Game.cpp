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

  init(ss);

  setAccelerometerEnabled(true);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::tick), 1);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleInput(const c::Vec2 &tap) {

  auto mv= CC_GEC(f::CMove,_player,"f/CMove");
  auto py= CC_GEC(Ship,_player,"f/CPixie");

  if (!MGMS()->isLive()
      py->_is_dying) { return;  }

  auto input_abs = c::Vec2(fabs(tap.x), fabs(tap.y));

  // calculate player speed based on how much device has tilted
  // greater speed multipliers for greater tilt values
  mv->speed.x = tap.x * ((input_abs.x > 0.3) ? 36 : ((input_abs.x > 0.2) ? 28 : 20 ));
  mv->speed.y = tap.y * ((input_abs.y > 0.3) ? 36 : ((input_abs.y > 0.2) ? 28 : 20 ) );

  // update the background
  _background->setPosition(tap.x * -30, tap.y * -30);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::didAccelerate(c::Acceleration *acc) {
  handleInput(c::Vec2(acc->x, acc->y));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseStart(const c::Vec2 &tap) {
  auto box= MGMS()->getEnclosureRect();
  auto wz= cx::visRect();

  if(!box.containsPoint(tap)) {
    pauseGame();
    return;
  }

  auto input = CC_ZPT;
  input.x = (tap.x - HWZ(wz.size)) / CC_ZW(wz.size);
  input.y = (tap.y - HHZ(wz.size)) / CC_ZH(wz.size);
  handleInput(input);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &tap) {
  auto wz= cx::visRect();
  auto input = CC_ZPT;
  input.x = (tap.x - HWZ(wb.size)) / CC_ZW(wz.size);
  input.y = (tap.y - HHZ(wb.size)) / CC_ZH(wz.size);
  handleInput(input);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &tap) {
  handleInput(CC_ZPT);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  pauseGame();
  return false;
}
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::pauseGame() {

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto background = f::reifyRefType<BackgroundManager>();
  addItem(background, E_LAYER_BACKGROUND);

  createBoundary();

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createBoundary() {
  // generate vertices for the boundary
  auto box= MGMS()->getEnclosureBox();
  c::Vec2 vs[4] = {
    c::Vec2(box.left, box.bottom),
    c::Vec2(box.left, box.top),
    c::Vec2(box.right, box.top),
    c::Vec2(box.right, box.bottom)
  };

  // draw the boundary
  auto bdy = c::DrawNode::create();
  bdy->drawPolygon(vs, 4, c::ccc4f(0,0,0,0), 1, c::ccc4f(1,1,1,0.3));
  addItem(bdy, E_LAYER_FOREGROUND);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::tick(float dt) {
  auto py=CC_GEC(Ship,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  // don't tick if player is dying
  if(py->_is_dying) {
  return; }

  ss->seconds += 1;
  ss->combo_timer -= 1;

  // show the combo achieved if time is up
  if(ss->combo_timer < 0) {
    ss->combo_timer = 0;
  }
  else if(ss->combo_timer == 0) {
    comboTimeUp(ss, py);
  }

  // Tick each enemy
  c::Object *object = CC_NIL;
  CCARRAY_FOREACH(ss->enemies, object) {
    auto enemy = (Enemy*)object;
    if(enemy) {
      enemy->tick();
    }
  }

  // Tick each power-up
  object = CC_NIL;
  CCARRAY_FOREACH(ss->powerups, object) {
    auto powerup = (PowerUp*)object;
    if(powerup) {
      powerup->tick();
    }
  }

  // add an enemy formation every 5 seconds
  if(ss->seconds % 5 == 0) {
    addEnemyFormation(ss);
  }

  // add a powerup formation every 4 seconds
  if(ss->seconds % 4 == 0) {
    addPowerUp(ss);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::comboTimeUp(GVars *ss, Ship *sp) {
  // combo is considered only more than 5 enemies were killed
  if(ss->enemies_killed_combo < 5) {
  return; }

  // add combo to score and update the label
  getHUD()->updateScore(ss->enemies_killed_combo * 10);
  getHUD()->flashCombo(ss->enemies_killed_combo, sp->pos());

  // reset combo kill counter
  ss->enemies_killed_combo = 0;
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

//////////////////////////////////////////////////////////////////////////////
//
const c::Rect Game::getEnclosureRect() {

  auto wb= cx::visBox();
  auto offset = 0.025;
  auto x= wb.right * offset;
  auto y= wb.top * offset;
  auto w= wb.right - wb.right * offset * 2;
  auto h= wb.top - wb.top * offset * 4;

  // calculate the position & dimension of the game's boundary
  return c::Rect(x,y,w,h);
}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto r= getEnclosureRect();
  return f::Box4(
      r.origin.y + r.size.height,
      r.origin.x + r.size.width,
      r.origin.y,
      r.origin.x
      );
}



NS_END




