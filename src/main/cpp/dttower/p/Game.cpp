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
NS_BEGIN(dttower)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  DECL_PTR(ecs::Node, _shared)

    NSMutableArray *_pathSteps;
    float _squareSize;
    NSMutableArray *_defensePositions;
    NSMutableArray *_defenses;
    int _waveNumber;
    CCLabelTTF *_waveLabel;
    int _countEnemies;
    int _lifePoints;
    CCLabelTTF *_lifePointsLabel;

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual bool onMouseStart(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

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

  ss->squareSize = wz.height / 8;

  loadPathSteps(ss);
  loadDefensePositions();

  _defenses.clear();


  // Schedule waves
  //[self schedule:@selector(loadWave) interval:WAVES_INTERVAL repeat:NUM_WAVES delay:0.0];

  // Add tower
  auto tower = cx::reifySprite("tower.png");
  auto pos= _pathSteps[0].position;
  tower->setPosition(pos);
  addAtlasItem("game-pics", tower, 2);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadPathSteps(GVars *ss) {

  auto wb= cx::visBox();
  s_vec<c::Vec2> pts = {
    c::Vec2(-50, wb.top - ss->squareSize * 3.30),
    c::Vec2(ss->squareSize * 2.5, wb.top - ss->squareSize * 3.3),
    c::Vec2(ss->squareSize * 2.5, ss->squareSize * 2),
    c::Vec2(ss->squareSize * 5.2, ss->squareSize * 2),
    c::Vec2(ss->squareSize * 5.2, wb.top - ss->squareSize*2),
    c::Vec2(ss->squareSize * 7.8, wb.top - ss->squareSize*2),
    c::Vec2(ss->squareSize * 7.8, ss->squareSize * 3.5),
    c::Vec2(ss->squareSize * 10.5, ss->squareSize * 3.5),
    c::Vec2(ss->squareSize * 10.5, wb.top - ss->squareSize*2),
    c::Vec2(wb.right - ss->squareSize * 1.2, wb.top - ss->squareSize * 2)
  };

  _pathSteps.clear();

  PathStep *ps= CC_NIL;
  F__LOOP(it, pts) {
    auto pt= *it;
    auto p= PathStep::create();
    auto s= (c::Sprite*)p;
    s->setPosition(pt);
    _pathSteps.push_back(p);
    addAtlasItem("game-pics",s);
    CC_HIDE(s);
    if (ps) {
      ps->next=p;
      ps=p;
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
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

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");

  _waveLabel = cx::reifyLabel("text",15,"Wave " + FTOS(_level));
  _waveLabel->setPosition(40 , wz.height - 25);
  _waveLabel->setColor(cx::black());
  addItem(_waveLabel);

  // Initialize life points
  _lifePoints = 10;
  _lifePointsLabel = cx::reifyLabel(
      "text",15,
      sstr("Life: ") + FTOS(_lifePoints) +" ");

  _lifePointsLabel->setPosition(wb.right - 50 , 20);
  _lifePointsLabel->setColor(cx::black());

  addItem(_lifePointsLabel);

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


NS_END




