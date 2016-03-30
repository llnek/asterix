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
NS_BEGIN(monsters)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  c::MenuItem* mkBtn();
  void cfgBtn(c::MenuItem*, const sstr&);
  void lblBtn(c::MenuItem*, const sstr&);
  void onBtnTapped(int,int);
  void onEnd();

  __decl_ptr(ecs::Node,_human)
  __decl_ptr(ecs::Node,_enemy)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onMouseStart(const c::Vec2&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};


//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  this->setOpacity(255 * 0.1);
  surcease();
  MGMS()->stop();
  Ende::reify(MGMS(),4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onBtnTapped(int cost, int count) {
  auto player = CC_GEC(Player,_human,"n/Player");
  if (player->coins < cost) { return; } else {
    player->coins -= cost;
  }
  createMonsters(_engine,cost,1,count);
  cx::sfxPlay("spawn");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _human= _engine->getNodes("f/CGesture")[0];
  _enemy= _engine->getNodes("f/CAutoma")[0];

}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
  auto render= CC_GEC(f::CPixie,_human,"f/CPixie");
  auto player= CC_GEC(Player,_human,"n/Player");
  if (render->bbox().containsPoint(loc)) {
    player->attacking = !player->attacking;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* GLayer::mkBtn() {
  return cx::reifyMenuBtn("button.png", "button_sel.png");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::cfgBtn(c::MenuItem *b, const sstr &png) {
  auto s = cx::reifySprite(png);
  s->setPosition(CC_ZW(CC_CSIZE(b)) * 0.25, HHZ(CC_CSIZE(b)));
  b->addChild(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::lblBtn(c::MenuItem *b, const sstr &msg) {

  auto txt = cx::reifyBmfLabel("dft", msg);
  txt->setPosition(CC_ZW(CC_CSIZE(b)) * 0.75, HHZ(CC_CSIZE(b)));
   // txt->setVerticalAlignment(c::TextVAlignment::TOP);
  b->addChild(txt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  auto MARGIN = 26 / CC_CONTENT_SCALE_FACTOR();
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  // quirk,zap,munch
  s_vec<c::MenuItem*> btns {mkBtn(),mkBtn(),mkBtn()};
  auto bz= CC_CSIZE(btns[0]);
  auto menu= cx::mkHMenu(btns, CC_ZW(bz)/4);
  menu->setPosition(wb.cx, MARGIN + HHZ(bz));
  btns[0]->setCallback([=](c::Ref*){
      this->onBtnTapped(COST_QUIRK,2); });
  btns[1]->setCallback([=](c::Ref*){
      this->onBtnTapped(COST_ZAP,1); });
  btns[2]->setCallback([=](c::Ref*){
      this->onBtnTapped(COST_MUNCH,1); });
  cfgBtn(btns[0], "quirk1.png");
  cfgBtn(btns[1], "zap1.png");
  cfgBtn(btns[2], "munch1.png");

  ///////
  lblBtn(btns[0], "10");
  lblBtn(btns[1], "25");
  lblBtn(btns[2], "50");

  addItem(menu);

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic=="/game/player/lose") {
    y->onEnd();
  }

  if (topic=="/game/player/earnscore") {
    auto msg= (j::json*) m;
    y->getHUD()->updateCoins(
      JS_INT(msg->operator[]("team")),
      JS_INT(msg->operator[]("score")));
  }

  if (topic=="/game/hud/setstate") {
    auto msg= (j::json*) m;
    y->getHUD()->updateAIState(JS_STR(msg->operator[]("state")));
  }

  if (topic=="") {
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




