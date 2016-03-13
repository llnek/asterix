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
#include "Game.h"
//#include "Ende.h"
#include "n/lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)
BEGIN_NS_UNAMED

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BLayer : public f::XLayer {
  STATIC_REIFY_LAYER(BLayer)
  MDECL_DECORATE()
};
void BLayer::decoUI() {
  regoAtlas("back-tiles", 1);
  regoAtlas("game-pics", 0);
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*) getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)

  virtual void onInited();

  MDECL_GET_IID(2)
  MDECL_DECORATE()

  DECL_PTR(ecs::Node, _arena)
  DECL_PTR(ecs::Node, _ship)

  void incSecCount(float);
  void onPlayerKilled();
  void moveBackTiles(float);
  void sharedExplosion();
  void initBackTiles();
  void initBackSkies();
  void showMenu();
  void onDone();
  void deco();

};

//////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackTiles() {
  moveBackTiles(0);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5.0);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveBackTiles(float) {
  auto po= MGMS()->getPool("BackTiles");
  auto wz= cx::visRect();
  auto tm = po->getAndSet(true);
  auto ui= CC_GEC(f::CDraw, tm, "f/CDraw");
/*
  if (ENP(tm)) {
    SCAST(GEngine*,engine)->createBackTiles();
    tm= ps->get();
  }
*/
  ui->inflate(cx::randFloat(wz.size.width), wz.size.height);

  auto fun = c::CallFunc::create([=]() { ui->deflate(); });
  auto move = c::MoveBy::create(
      cx::randInt(2) + 10,
      c::Vec2(0, -wz.size.height - HHZ(wz.size)));

  ui->node->runAction(c::Sequence::create(move,fun,CC_NIL));
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackSkies() {
  auto p = MGMS()->getPool("BackSkies");
  auto g = (Game*) getSceneX();
  auto bs = p->getAndSet(true);
  auto ui= CC_GEC(f::CDraw, bs, "f/CDraw");
  ui->inflate(0, 0);

  g->backSkyDim = bs->csize();
  g->backSkyRe = CC_NIL;
  g->backSky = bs;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::sharedExplosion() {
  auto a = c::Animation::create();//WithSpriteFrames(fs, 0.04);
  sstr str;
  for (auto n = 1; n < 35; ++n) {
    auto ns= s::to_string(n);
    if (n < 10 ) { str= "0" + ns; } else { str= ns; }
    a->addSpriteFrame(
      cx::getSpriteFrame("explosion_" + str + ".png"));
  }
  a->setDelayPerUnit(0.04);
  CC_ACAC()->addAnimation(a, "Explosion");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];

  bornShip(SCAST(GEngine*,engine), _ship);
  sharedExplosion();
  initBackSkies();
  initBackTiles();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  schedule(CC_SCHEDULE_SELECTOR(GLayer::incSecCount), 1.0);
  _engine= mc_new(GEngine);

  auto a= regoAtlas("explosions");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BDFUNC::ADDITIVE);

  cx::sfxMusic("bgMusic", true);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::incSecCount(float) {
  // counter used to spawn enemies
  auto ss= CC_GEC(GVars, _arena, "n/GVars");
  ss->secCount += 1;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("explodeEffect");
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    bornShip(SCAST(GEngine*,engine), _ship);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  this->setOpacity(0.1 * 255);
  MGMS()->stop();
  surcease();
  //Ende::reify(MGMS(), 4);
}


END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto wb= cx::visBox();
  return f::Box4(wb.top + 10, wb.right, wb.bottom, wb.left);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = (GLayer*) getLayer(2);

  if ("/game/players/earnscore" == topic) {
    auto msg= (j::json*) m;
    y->getHUD()->updateScore(
        JS_INT(msg->operator[]("score")));
  }
  else
  if ("/game/backtiles" == topic) {
    //y->initBackTiles();
  }
  else
  if ("/hud/showmenu" == topic) {
    y->showMenu();
  }
  else
  if ("/hud/replay" == topic) {
    //y->replay();
  }
  else
  if ("/game/players/killed" == topic) {
    y->onPlayerKilled();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  BLayer::reify(this, 1);
  GLayer::reify(this, 2);
  play();
}


NS_END


