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
NS_BEGIN(cuteness)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _enemies)
  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _planet)

  __decl_create_layer(GLayer)
  __decl_get_iid(2)
  __decl_deco_ui()

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();
  void onEnd();
  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _enemies= _engine->getNodes("n/ESquad")[0];
  _planet= _engine->getNodes("f/CHuman")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto h=CC_GEC(f::CHealth,_planet,"f/CHealth");
  auto p=CC_GEC(Planet,_planet,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  h->setHealth(10);
  ss->speedBump=0;

  auto b = cx::reifyMenuText("btns",
      "Tap here to start.\nProtect the planet!" );
  XCFG()->scaleBmfont(b,24);
  auto menu=cx::mkMenu(b);
  b->setColor(c::Color3B(255, 254, 185));
  b->setCallback([=](c::Ref*) {
        ss->enabled=true;
        CC_HIDE(menu);
        menu->removeFromParent();
      });
  CC_POS2(b, wb.cx, wb.top * 0.2);
  addItem(menu);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(getSceneX(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto sq= CC_GEC(EnemySquad, _enemies, "n/ESquad");
  scanAndSmash(sq, tap);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
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
void GLayer::decoUI() {
  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("gpics");


}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/game/player/earnscore" == topic) {
    auto msg= (j::json*) m;
    y->getHUD()->updateScore(
        JS_INT(msg->operator[]("score")));
  }
  else
  if ("/game/player/lose" == topic) {
    y->onEnd();
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




