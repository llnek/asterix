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
#include "BackDrop.h"
//#include "MMenu.h"
#include "n/lib.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)
BEGIN_NS_UNAMED


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  virtual void onMouseMotion(f::ComObj*, const c::Vec2&);
  virtual void postReify();

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  void onAlienKilled(Alien*);
  void onPlayerKilled();
  void onEnd();

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  DECL_PTR(a::NodeList,aliens)
  DECL_PTR(a::NodeList,ships)
};

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(f::ComObj *c, const c::Vec2 &loc) {
  auto bx= MGMS()->getEnclosureBox();
  auto pos= c->pos();
  c->setPos(cx::clamp(loc, c->csize(), bx).x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {

  auto death = c::Spawn::createWithTwoActions(
      c::FadeOut::create(0.5f),
      c::ScaleTo::create(0.5f, 1.5f));
  auto ship= CC_GNLF(Ship,ships,"ship");
  auto d1=c::CallFunc::create([=]() {
        ship->deflate();
    });
  auto pos= ship->pos();
  c::CallFunc *d2;

  if (getHUD()->reduceLives(1)) {
    d2=c::CallFunc::create([=]() {
        this->onEnd();
    });
  } else  {
    d2= c::CallFunc::create([=]() {
        spawnPlayer(ship);
    });
  }

  ship->sprite->runAction(
      c::Sequence::create( death, d1,d2,nullptr));

  auto explosion = c::ParticleSystemQuad::create("pics/explosion.plist");
  explosion->setAutoRemoveOnFinish(true);
  explosion->setPosition(pos);
  MGML()->addItem(explosion);

  cx::sfxPlay("blast_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAlienKilled(Alien *obj) {

  obj->sprite->stopAllActions();

  auto blast = c::ScaleTo::create(0.25f, 0.0f);
  auto remove =c::CallFunc::create([=]() {
      obj->deflate();
  });
  obj->sprite->runAction(
      c::Sequence::createWithTwoActions(blast, remove));

  auto exp = c::ParticleSystemQuad::create("pics/explosion.plist");
  auto pc= c::ccc4f(0.9569, 0.2471, 0.3373, 1);
  exp->setStartColor(pc);
  exp->setEndColor(pc);
  exp->setAutoRemoveOnFinish(true);
  exp->setPosition(obj->pos());
  MGML()->addItem(exp);

  getHUD()->updateScore(obj->score);
  cx::sfxPlay("blast_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  auto c= loadLevel(MGMS()->getLevel());
  regoAtlas("game-pics");
  engine= mc_new1(GEngine,c);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  aliens = engine->getNodeList(AlienNode().typeId());
  ships = engine->getNodeList(ShipNode().typeId());
  auto lpr2= CC_GNLF(Looper,aliens,"looper");
  auto lpr1= CC_GNLF(Looper,ships,"looper");
  auto s= CC_GNLF(Ship,ships,"ship");

  this->motionees.push_back(s);
  spawnPlayer(s);

  // timers for bullets
  lpr2->timer = cx::reifyTimer(MGML(), 1.0f);
  lpr1->timer = cx::reifyTimer(MGML(), 1.0f);
}




END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {

  auto y = (GLayer*) getLayer(2);

  if ("/game/player/killed" == topic) {
    y->onPlayerKilled();
  }
  else
  if ("/game/alien/killed" == topic) {
    y->onAlienKilled((Alien*) m);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}



NS_END



