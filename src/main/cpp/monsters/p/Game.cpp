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

  DECL_PTR(a::NodeList, shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
    return true;
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
c::MenuItem* GLayer::mkBtn() {
  return cx::reifyMenuBtn("button.png", "button_sel.png");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::cfgBtn(c::MenuItem *b, const sstr &png) {
  auto s = cx::reifySprite(png);
  s->setPosition(CC_CSIZE(b).width * 0.25, HHZ(CC_CSIZE(b)));
  b->addChild(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::lblBtn(c::MenuItem *b, const sstr &msg) {
  
  auto txt = cx::reifyBmfLabel("dft", msg);
  txt->setPosition(CC_CSIZE(b).width * 0.75, HHZ(CC_CSIZE(b)));
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
  auto menu= cx::mkHMenu(btns, bz.width/4);
  menu->setPosition(wb.cx, MARGIN + HHZ(bz));
  btns[0]->setCallback([=](c::Ref*){});
  btns[1]->setCallback([=](c::Ref*){});
  btns[2]->setCallback([=](c::Ref*){});
  cfgBtn(btns[0], "quirk1.png");
  cfgBtn(btns[1], "zap1.png");
  cfgBtn(btns[2], "munch1.png");

  ///////
  lblBtn(btns[0], "10");
  lblBtn(btns[1], "25");
  lblBtn(btns[2], "50");

  addItem(menu);

  auto _stateLabel = cx::reifyBmfLabel("dft", "Idle");
  _stateLabel->setPosition(wb.cx, wb.top * 0.25);
  addItem(_stateLabel);

  auto coin1 = cx::reifySprite("coin.png");
  coin1->setPosition(
      MARGIN + HWZ(CC_CSIZE(coin1)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin1)));
  addItem(coin1);

  auto coin2 = cx::reifySprite("coin.png");
  coin2->setPosition(
      wb.right - MARGIN - HWZ(CC_CSIZE(coin2)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin2)));
  addItem(coin2);

  auto _coin1Label = cx::reifyBmfLabel("dft","10");
//width:winSize.width * 0.25
  _coin1Label->setAlignment(c::TextHAlignment::LEFT);
  _coin1Label->setPosition(
      coin1->getPositionX() + HWZ(CC_CSIZE(coin1)) + MARGIN/2 +
      HWZ(CC_CSIZE(_coin1Label)),
      wb.top - MARGIN*1.6);
  addItem(_coin1Label);

  auto _coin2Label = cx::reifyBmfLabel("dft", "10");
//width:winSize.width * 0.25
  _coin2Label->setAlignment(c::TextHAlignment::RIGHT);
  _coin2Label->setPosition(coin2->getPositionX() - HWZ(CC_CSIZE(coin1)) - MARGIN/2 -
      HWZ(CC_CSIZE(_coin2Label)),
      wb.top - MARGIN*1.6);
  addItem(_coin2Label);

  engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
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
Game::Game()
  : f::GameScene(true) {
}

NS_END




