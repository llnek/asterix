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
#include "n/PathStep.h"
#include "n/Defense.h"
#include "n/Enemy.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(dttower)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  void loadDefensePositions();
  void loadPathSteps();
  void enemyReachedTower();
  void loadEnemy(float);
  void loadWave(float);

  DECL_PTR(c::Label,_lifePointsLabel)
  DECL_PTR(c::Label, _waveLabel)
  DECL_PTR(ecs::Node, _shared)

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
  ss->lifePoints = 10;
  ss->waveNumber=1;

  _waveLabel = cx::reifyLabel("text",15, "Wave " + FTOS(ss->waveNumber));
  _waveLabel->setPosition(40 , wz.height - 25);
  _waveLabel->setColor(cx::black());
  addItem(_waveLabel);

  _lifePointsLabel = cx::reifyLabel(
      "text",15,
      sstr("Life: ") + FTOS(ss->lifePoints) +" ");

  _lifePointsLabel->setPosition(wb.right - 50 , 20);
  _lifePointsLabel->setColor(cx::black());
  addItem(_lifePointsLabel);

  loadDefensePositions();
  loadPathSteps();

  // tower
  auto tower = cx::reifySprite("tower.png");
  auto pos= ss->pathSteps[0]->getPosition();
  tower->setPosition(pos);
  addAtlasItem("game-pics", tower, 2);

  // waves of attack
  schedule(CC_SCHEDULE_SELECTOR(GLayer::loadWave), WAVES_INTERVAL, NUM_WAVES, 0.0);

  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadDefensePositions() {
  auto arr= cx::readXmlAsList(XCFG()->getAtlas("defenses"));
  auto sz = cx::calcSize("defense_level_1.png");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto cnt= arr->count();
  auto sw= sz.width;
  auto gap = 19;

  ss->defensePositions.clear();

  for (auto i=0; i < cnt; ++i) {
    auto s = cx::reifySprite("defense_position.png");
    auto d= (c::Dictionary*) arr->objectAtIndex(i);
      
      auto mX = f::dictVal<c::String>(d,"x")->floatValue();
      auto mY = f::dictVal<c::String>(d,"y")->floatValue();

    s->setPosition(sw * mX + gap, sw * mY);
    addAtlasItem("game-pics",s);

    ss->defensePositions.push_back(s);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadEnemy(float) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->countEnemies < WAVES_NUM_ENEMIES) {
    auto it = ss->pathSteps.back();
    auto e= Enemy::create(ss,it);

    addAtlasItem("game-pics",e);
    ss->enemies.push_back(e);
    ss->countEnemies += 1;
  } else {
    unschedule(CC_SCHEDULE_SELECTOR(GLayer::loadEnemy));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadPathSteps() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
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

  ss->pathSteps.clear();

  PathStep *ps= CC_NIL;
  F__LOOP(it, pts) {
    auto pt= *it;
    auto p= PathStep::create(pt);
    ss->pathSteps.push_back(p);
    addAtlasItem("game-pics",p);
    CC_HIDE(p);
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
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &tap) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  F__LOOP(it,ss->defensePositions) {
    auto dp= *it;
    if (dp->boundingBox().containsPoint(tap)) {
      auto d= Defense::create(ss, levelOne, dp->getPosition());
      addAtlasItem("game-pics",d,1);
      ss->defenses.push_back(d);
    }
  }
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
void GLayer::loadWave(float) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  _waveLabel->setString("Wave " + FTOS(ss->waveNumber));

  ss->countEnemies = 0;
  ss->enemies.clear();

  schedule(CC_SCHEDULE_SELECTOR(GLayer::loadEnemy), 1);
  ss->waveNumber += 1;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::enemyReachedTower() {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->lifePoints -= 1;
  _lifePointsLabel->setString("Life: " + FTOS(ss->lifePoints));
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
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




