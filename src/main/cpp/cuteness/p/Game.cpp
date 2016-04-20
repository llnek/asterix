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

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _enemies)
  __decl_ptr(ecs::Node, _shared)
  __decl_ptr(ecs::Node, _planet)
  __decl_ptr(c::Label, _tutInfo)

  __decl_create_layer(GLayer)
  __decl_get_iid(2)
  __decl_deco_ui()

  void startGame();
  void showTitle();
  void spawnEnemy();
  void hitPlanet(ecs::Node*);
  void shipSmash(ecs::Node*);
  void planetDamage();

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
  _planet= _engine->getNodes("n/Planet")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto p= CC_GEC(Planet,_planet,"f/CPixie");
  p->runAction(c::FadeIn::create(2));
  p->runAction(
      c::Sequence::create(
        c::MoveTo::create(2,CCT_PT(wb.cx, wb.cy)),
        c::CallFunc::create([=](){
           this->showTitle();
           this->startGame();
          }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startGame() {
    auto wb=cx::visBox();

  _tutInfo = cx::reifyBmfLabel("dft",
      "Tap here to start.\nProtect the planet!" );
  _tutInfo->setColor(c::Color3B(255, 254, 185));
  XCFG()->scaleBmfont(_tutInfo,24);
  CC_ANCHOR(_tutInfo, 0.5,1);
  CC_POS2(_tutInfo,wb.cx, wb.top - 30);
  addItem(_tutInfo);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showTitle() {
  auto t = cx::reifySprite("gametitle.png");
  t->setOpacity(0);
  t->setScale(4);
  t->runAction(c::Spawn::create(
        c::FadeIn::create(500) ,
        c::ScaleTo::create(500,1),
        CC_NIL
        ));
}

static s_arr<sstr,3> ENEMYPICS = {
  "beetleship.png",
  "octopus.png", "rocketship.png"};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnEnemy() {
  auto sq= CC_GEC(EnemySquad,_enemies,"n/ESquad");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
    auto wb= cx::visBox();
  auto po= sq->randGet();
    auto e= (ecs::Node*)po->take(true);
  auto sp= CC_GEC(Enemy,e,"f/CPixie");
  float y = cx::randInt(wb.top);
  float x= cx::randInt(100) * cx::randSign();
  if (x > 0) {
    x += wb.right;
  }
  cx::resurrect(e, x, y);
  auto t= c::random(2500-ss->speedBump, 4500-ss->speedBump) / 1000;
  sp->runAction(
      c::MoveTo::create(t, CCT_PT(wb.cx, wb.cy)));
  ss->speedBump += 50;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::hitPlanet(ecs::Node *obj) {
  cx::hibernate(obj);
    planetDamage();
  cx::sfxPlay("sndBlast");
  auto h=CC_GEC(f::CHealth,_planet,"f/CHealth");
  if (h->alive()) {
    spawnEnemy();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::shipSmash(ecs::Node *obj) {
  cx::hibernate(obj);
  cx::sfxPlay("sndKill");
  getHUD()->updateScore(28);
    spawnEnemy();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::planetDamage() {
  auto h= CC_GEC(f::CHealth,_planet,"f/CHealth");
  auto p= CC_GEC(Planet,_planet,"f/CPixie");
  h->hurt(2);
  p->setOpacity(255 * h->ratio());
  auto co= p->getOpacity();
  if (h->curHP < 2) {
    p->setOpacity(0);
    cx::sfxPlay("sndLose");
  } else {
    p->runAction(c::Spawn::create(
          c::FadeTo::create(200,1),
          c::Sequence::create(
            c::ScaleTo::create(200, 1.2),
            c::CallFunc::create([=](){
              p->setOpacity(co);
              p->setScale(1);
              }),
            CC_NIL
            ),
          CC_NIL
          ));
  }
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
  if (_tutInfo) {
    auto h=CC_GEC(f::CHealth,_planet,"f/CHealth");
    auto p=CC_GEC(Planet,_planet,"f/CPixie");
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
    p->setOpacity(255);
    h->setHealth(10);
    ss->speedBump=0;
    _tutInfo->removeFromParent();
    _tutInfo=CC_NIL;
    spawnEnemy();
    getHUD()->ready();
    return false;
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
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  centerImage("game.bg")->runAction(c::FadeIn::create(2));
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




