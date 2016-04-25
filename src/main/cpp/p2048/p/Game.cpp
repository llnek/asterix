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
  void drawCard(GVars*, bool);
  bool hasBlanks(GVars*);
  void layoutCards(GVars*);
  const CCT_PT getPosition(GVars*, int, int);
  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _player)
  __decl_create_layer(GLayer)
  __decl_get_iid(2)
  __decl_deco_ui()
  __decl_vec(CCT_PT,_tpts)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

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

  layoutCards(ss);
  //start with 2 random numbers
  drawCard(ss, false);
  drawCard(ss, false);

  ss->enabled=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  auto g=CC_GEC(f::CGesture,_player,"f/CGesture");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->enabled=true;
  _tpts.push_back(tap);
  g->reset();
  if (_tpts.size() < 3) { return; }

  auto last = _tpts.size() -1;
  auto dx = _tpts[last].x - _tpts[0].x;
  auto dy = _tpts[last].y - _tpts[0].y;

  if (abs(dx) > abs(dy)) {
    if (dx > 0) {
      g->right=true;
    } else {
      g->left=true;
    }
  } else {
    if (dy > 0) {
      g->up=true;
    } else {
      g->down=true;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  _tpts.push_back(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->enabled) {
    ss->enabled=false;
    _tpts.clear();
    _tpts.push_back(tap);
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::drawCard(GVars *ss, bool animation) {
  while (true) {
    auto i = cx::randInt(4);
    auto j = cx::randInt(4);
    auto c= ss->cardArr[i]->get(j);
    if (c->getNumber() == 0) {
      c->setNumber(cx::randSign() > 0 ? 4 : 2);
      if (animation) {
        c->runNewNumberAction();
      }
      break;
    }
    if (!hasBlanks(ss)) {
        break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::hasBlanks(GVars *ss) {
  auto hasZero = false;
  for (auto i = 0; i < ss->cardArr.size(); ++i) {
    auto arr= ss->cardArr[i];
    for (auto j = 0; j < arr->size(); ++j) {
      if( 0 == arr->get(j)->getNumber() ) {
        hasZero = true;
        break;
      }
    }
  }
  return hasZero;
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT GLayer::getPosition(GVars *ss, int row, int col) {
  auto y= 20 + row * ss->cellSize + HTV(ss->cellSize);
  auto x= 20 + col * (ss->cellSize + CELL_SPACE) + HTV(ss->cellSize);
  return CCT_PT(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::layoutCards(GVars *ss) {
  auto wz= cx::visSize();
  ss->cellSize = (wz.width - 3*CELL_SPACE-40) /4;
  auto sz= CCT_SZ(ss->cellSize, ss->cellSize);
  for (auto r = 0; r < 4; ++r) {
    auto arr= mc_new1(CardArr,4);
    for (auto c = 0; c < arr->size(); ++c) {
      auto card = Card::create(0, sz, getPosition(ss, r, c));
      arr->set(c, card);
      addItem(card);
    }
    ss->cardArr.push_back(arr);
  }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto bg = c::LayerColor::create(c::Color4B(180, 170, 160,255));
  addItem(bg);
  regoAtlas("gpics");
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




