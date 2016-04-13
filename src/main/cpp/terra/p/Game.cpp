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
#include "MMenu.h"
#include "HUD.h"
#include "Game.h"
#include "Ende.h"
#include "n/C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)
BEGIN_NS_UNAMED

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BLayer : public f::XLayer {
  __decl_create_layer(BLayer)
  __decl_deco_ui()
};
void BLayer::decoUI() {
  regoAtlas("back-tiles", 1);
  regoAtlas("game-pics", 0);
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  __decl_create_layer(GLayer)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  __decl_get_iid(2)
  __decl_deco_ui()

  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(ecs::Node, _ship)

  void incSecCount(float);
  void onPlayerKilled();
  void moveBackTiles(float);
  void sharedExplosion();
  void initBackTiles();
  void initBackSkies();
  void showMenu();
  void onEnd();
  void deco();

};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto ui=CC_GEC(Ship,_ship,"f/CPixie");
  return ui->boundingBox().containsPoint(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {
  processTouch(_ship,tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *t) {
  onMouseMotion(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *t) {
  return onMouseStart(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
    auto b= []() { cx::pop(); };
  cx::pushEx(MMenu::reify(mc_new1(MCX,b)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackTiles() {
  moveBackTiles(0);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveBackTiles(float) {
  auto po= MGMS()->getPool("BackTiles");
  auto tm = (ecs::Node*)po->take(true);
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto ui= CC_GEC(f::CPixie, tm, "f/CPixie");

  cx::resurrect(tm, cx::randInt(wb.right), wb.top);
  ui->runAction(
      c::Sequence::create(
        c::MoveBy::create(
          cx::randInt(2) + 10,
          CCT_PT(0, -wz.height - HTV(wz.height))),
        c::CallFunc::create([=]() {
          cx::hibernate(tm); }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackSkies() {
  auto ss=CC_GEC(GVars,_arena,"n/GVars");
  auto p = MGMS()->getPool("BackSkies");
  auto bs = (ecs::Node*)p->take(true);
  auto ui= CC_GEC(f::CPixie, bs, "f/CPixie");

  ss->backSkyDim = CC_CSIZE(ui);
  ss->backSkyRe = CC_NIL;
  ss->backSky = bs;

  cx::resurrect(bs,0,0);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::sharedExplosion() {
  auto a = cx::createAnimation(0.04,false,0);
  for (auto n = 1; n < 35; ++n) {
    auto ns= FTOS(n);
    auto s= n < 10 ? "0" + ns : ns;
    a->addSpriteFrame(
      cx::getSpriteFrame("explosion_" + s + ".png"));
  }
  CC_ACAC()->addAnimation(a, "Explosion");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];

  sharedExplosion();
  initBackSkies();
  initBackTiles();
  bornShip(_ship);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  schedule(CC_SCHEDULE_SELECTOR(GLayer::incSecCount), 1);
  _engine= mc_new(GEngine);

  auto a= regoAtlas("explosions");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BDFUNC::ADDITIVE);

  regoAtlas("cc-pics");
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
    onEnd();
  } else {
    bornShip(_ship);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
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


