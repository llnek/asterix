// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"
#include "n/lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)
BEGIN_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BLayer : public f::XLayer {
  STATIC_REIFY_LAYER(BLayer)
  MDECL_DECORATE()
};

void BLayer::decorate() {
  regoAtlas("back-tiles", 1);
  regoAtlas("game-pics", 0);
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*) getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)

  virtual void postReify();

  MDECL_GET_IID(2)
  MDECL_DECORATE()

  DECL_PTR(a::NodeList, arena)
  DECL_PTR(a::NodeList, ship)

  void onEarnScore(j::json* );
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
  schedule(CC_SCHEDULE_SELECTOR(GLayer::moveBackTiles), 5.0f);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveBackTiles(float) {
  auto ps= MGMS()->getPool("BackTiles");
  auto wz= cx::visRect();
  auto tm = ps->get();

  if (ENP(tm)) {
    SCAST(GEngine*,engine)->createBackTiles();
    tm= ps->get();
  }

  tm->inflate(cx::randFloat(wz.size.width), wz.size.height);

  auto fun = c::CallFunc::create([=]() { tm->deflate(); });
  auto move = c::MoveBy::create(
      cx::randInt(2) + 10,
      c::ccp(0, -wz.size.height - wz.size.height * 0.5f));

  tm->sprite->runAction(c::Sequence::create(move,fun,nullptr));
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::initBackSkies() {
  auto p = MGMS()->getPool("BackSkies");
  auto g = (Game*) getSceneX();
  auto bs = p->get();

  assert(bs != nullptr);
  bs->inflate(0, 0);

  g->backSkyDim = bs->csize();
  g->backSkyRe = nullptr;
  g->backSky = bs;
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::sharedExplosion() {
  c::Vector<c::SpriteFrame*> fs;
  sstr str;
  for (auto n = 1; n < 35; ++n) {
    auto ns= s::to_string(n);
    if (n < 10 ) { str= "0" + ns; } else { str= ns; }
    fs.pushBack( cx::getSpriteFrame( "explosion_" + str + ".png"));
  }
  auto a = c::Animation::createWithSpriteFrames(fs, 0.04f);
  c::AnimationCache::getInstance()->addAnimation(a, "Explosion");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  arena = engine->getNodeList(ArenaNode().typeId());
  ship = engine->getNodeList(ShipNode().typeId());

  this->motionees.push_back(CC_GNLF(Ship,ship,"ship"));

  bornShip(SCAST(GEngine*,engine), CC_GNLF(Ship,ship,"ship"));
  sharedExplosion();
  initBackSkies();
  initBackTiles();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {

  auto a= regoAtlas("explosions");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  a= regoAtlas("game-pics");
  a= regoAtlas("op-pics");
  a->setBlendFunc(BDFUNC::ADDITIVE);
  incIndexZ();

  this->engine= mc_new(GEngine);
  cx::sfxMusic("bgMusic", true);
  schedule(CC_SCHEDULE_SELECTOR(GLayer::incSecCount), 1.0f);
}

//////////////////////////////////////////////////////////////////////////
//
void GLayer::incSecCount(float) {
  // counter used to spawn enemies
  auto ss= CC_GNLF(GVars, arena, "slots");
  ss->secCount += 1;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  cx::sfxPlay("explodeEffect");
  if ( getHUD()->reduceLives(1)) {
    onDone();
  } else {
    bornShip(SCAST(GEngine*,engine), CC_GNLF(Ship,ship,"ship"));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEarnScore(j::json *msg) {
  auto n= JS_INT(msg->operator[]("score"));
  getHUD()->updateScore(n);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onDone() {
  surcease();
  //ELayer::reify(getSceneX(), 999);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto wb= cx::visBox();
  return f::Box4( wb.top + 10, wb.right, wb.bottom, wb.left);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = (GLayer*) getLayer(2);

  if ("/game/players/earnscore" == topic) {
    auto msg= (j::json*) m;
    y->onEarnScore(msg);
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
void Game::decorate() {
  HUDLayer::reify(this, 3);
  BLayer::reify(this, 1);
  GLayer::reify(this, 2);
  play();
}


NS_END(terra)


