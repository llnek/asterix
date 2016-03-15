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

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onInited();

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  void onAlienKilled(ecs::Node*);
  void onPlayerKilled();
  void onEnd();

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  DECL_PTR(ecs::Node, _aliens)
  DECL_PTR(ecs::Node,_ship)
  DECL_PTR(ecs::Node, _shared)

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &tap) {
  auto ui= CC_GEC(Ship,_ship,"f/CPixie");
  auto bx= MGMS()->getEnclosureBox();
  auto pos= ui->pos();
  auto loc= cx::clamp(tap, ui->csize(), bx);
  ui->setPos(loc.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  this->setOpacity(0.1 * 255);
  MGMS()->stop();
  surcease();
  //Ende::reify(MGMS(),4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {

  auto sp= CC_GEC(Ship,_ship,"f/CPixie");
  auto pos= sp->pos();
  c::CallFunc *d2;

  if (getHUD()->reduceLives(1)) {
    d2=c::CallFunc::create([=]() {
        this->onEnd();
    });
  } else {
    d2= c::CallFunc::create([=]() {
        spawnPlayer(_ship);
    });
  }

  sp->node->runAction(
      c::Sequence::create(
        c::Spawn::create(
          c::FadeOut::create(0.5),
          c::ScaleTo::create(0.5, 1.5),CC_NIL),
        c::CallFunc::create(
          [=]() { sp->deflate(); }),
        d2,
        CC_NIL));

  auto explosion =
    c::ParticleSystemQuad::create(
        XCFG()->getAtlas("explosions"));
  explosion->setAutoRemoveOnFinish(true);
  explosion->setPosition(pos);
  MGML()->addItem(explosion);

  cx::sfxPlay("blast_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAlienKilled(ecs::Node *node) {

  auto cs=CC_GEC(AlienType,node,"f/CStats");
  auto ui=CC_GEC(Alien,node,"f/CPixie");

  ui->node->stopAllActions();
  ui->node->runAction(
      c::Sequence::create(
        c::ScaleTo::create(0.25, 0.0),
        c::CallFunc::create(
          [=]() {
            ui->deflate();
            node->yield(); }),
        CC_NIL));

  auto exp = c::ParticleSystemQuad::create( XCFG()->getAtlas("explosion"));
  auto pc= c::ccc4f(0.9569, 0.2471, 0.3373, 1);
  exp->setAutoRemoveOnFinish(true);
  exp->setPosition(ui->pos());
  exp->setStartColor(pc);
  exp->setEndColor(pc);
  MGML()->addItem(exp);

  getHUD()->updateScore(cs->value);
  cx::sfxPlay("blast_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {
  auto c= loadLevel(MGMS()->getLevel());
  regoAtlas("game-pics");
  _engine= mc_new1(GEngine,c);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  spawnPlayer(_ship);

  auto lpr= CC_GEC(Loopers,_shared,"f/Loopers");
  // timers for bullets
  lpr->timers[0]= cx::reifyTimer(MGML(), 1000);
  lpr->timers[1]= cx::reifyTimer(MGML(), 1000);
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
void Game::decoUI() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  play();
}


NS_END



