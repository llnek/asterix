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

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(BackgroundManager, _background)
  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void  comboTimeUp(GVars *ss, Player *sp) ;

  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);
  virtual void onMouseMotion(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  void tick(float);
  void  handleInput(const CCT_PT &tap);
  void pauseGame();
  virtual void onAcceleration(c::Acceleration* acc, c::Event*);

  void createBoundary();
  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  setAccelerometerEnabled(true);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::tick), 1);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleInput(const CCT_PT &tap) {

  auto mv= CC_GEC(f::CMove,_player,"f/CMove");
  auto py= CC_GEC(Player,_player,"f/CPixie");

  if (!MGMS()->isLive() ||
      py->isDying) { return;  }

  auto input_abs = CCT_PT(fabs(tap.x), fabs(tap.y));

  // calculate player speed based on how much device has tilted
  // greater speed multipliers for greater tilt values
  mv->speed.x = tap.x * ((input_abs.x > 0.3) ? 36 : ((input_abs.x > 0.2) ? 28 : 20 ));
  mv->speed.y = tap.y * ((input_abs.y > 0.3) ? 36 : ((input_abs.y > 0.2) ? 28 : 20 ) );

  // update the background
  CC_POS2(_background, tap.x * -30, tap.y * -30);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration* acc, c::Event*) {
  handleInput(CCT_PT(acc->x, acc->y));
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto box= MGMS()->getEnclosureRect();
  auto wz= cx::visSize();

  if(!box.containsPoint(tap)) {
    pauseGame();
    return false;
  }

  auto input = CC_ZPT;
  input.x = (tap.x - HWZ(wz)) / CC_ZW(wz);
  input.y = (tap.y - HHZ(wz)) / CC_ZH(wz);
  handleInput(input);

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  auto wz= cx::visSize();
  auto input = CC_ZPT;
  input.x = (tap.x - HWZ(wz)) / CC_ZW(wz);
  input.y = (tap.y - HHZ(wz)) / CC_ZH(wz);
  handleInput(input);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
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

  _background = f::reifyRefType<BackgroundManager>();
  addItem(_background, E_LAYER_BACKGROUND);

  createBoundary();

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createBoundary() {
  // generate vertices for the boundary
  auto box= MGMS()->getEnclosureBox();
  CCT_PT vs[4] = {
    CCT_PT(box.left, box.bottom),
    CCT_PT(box.left, box.top),
    CCT_PT(box.right, box.top),
    CCT_PT(box.right, box.bottom)
  };

  // draw the boundary
  auto bdy = c::DrawNode::create();
  bdy->drawPolygon(vs, 4, c::ccc4f(0,0,0,0), 1, c::ccc4f(1,1,1,0.3));
  addItem(bdy, E_LAYER_FOREGROUND);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::tick(float dt) {
  auto py=CC_GEC(Player,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  // don't tick if player is dying
  if(py->isDying) {
  return; }

  ss->combo_timer -= 1;
  ss->seconds += 1;

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
    addEnemyFormation(ss,py);
  }

  // add a powerup formation every 4 seconds
  if(ss->seconds % 4 == 0) {
    addPowerUp(ss,py);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::comboTimeUp(GVars *ss, Player *sp) {
  // combo is considered only more than 5 enemies were killed
  if(ss->enemies_killed_combo < 5) {
  return; }

  // add combo to score and update the label
  getHUD()->updateScore(ss->enemies_killed_combo * 10);
  getHUD()->flashCombo(ss->enemies_killed_combo,
                       PCAST(c::Node,sp)->getPosition());

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
  return CCT_RT(x,y,w,h);
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




