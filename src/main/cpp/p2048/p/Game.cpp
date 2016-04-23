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
NS_BEGIN(p2048)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _shared)

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

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  layoutCards();
  //start with 2 random numbers
  createCardNumber(false);
  createCardNumber(false);
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
bool GLayer::onMouseStart(const CCT_PT &loc) {
  return true;
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

void GLayer::createCardNumber(bool animation) {
  while (true) {
    auto i = cx::randInt(4);
    auto j = cx::randInt(4);
    auto c= ss->cardArr[i]->get(j);
    if (c->getNumber() == 0) {
      c->setNumber(cx::randSign() > 0 ? 4 : 2);
      if (animation) {
        c->->runNewNumberAction();
      }
      break;
    }
    if (!shouldCreateCardNumber()) {
        break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
#define CELL_SPACE 10
void GLayer::layoutCards() {
  auto wz= cx::visSize();
  ss->cellSize = (wz.width - 3*CELL_SPACE-40)/4;
  auto sz= CCT_SZ(cellSize, cellSize);
  for (auto i = 0; i < 4; ++i) {
    for (auto j = 0; j < 4; ++j) {
      auto card = Card::create(0, sz, getPosition(i, j));
      ss->cardArr[i]->set(j, card);
      addAtlasItem("gpics", card);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto bg = c::LayerColor::create(c::Color3B(180, 170, 160));
  addItem(bg);
  _engine = mc_new(GEngine);
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




