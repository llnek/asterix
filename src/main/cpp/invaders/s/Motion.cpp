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

#include "base/CCEventKeyboard.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "EFactory.h"
#include "Motion.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine*> e) {
  AlienMotionNode a;
  ShipMotionNode s;
  CannonCtrlNode c;

  aliens = e->getNodeList(a.typeId());
  ships = e->getNodeList(s.typeId());
  cannons = e->getNodeList(c.typeId());

  this->engine= (GEngine*)e.get();
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {
  auto enemy = aliens->head;
  auto ship=ships->head;
  auto cns= cannons->head;

  if (MGMS()->isLive()) {
    processAlienMotions(enemy,dt);
    controlCannon(cns,dt);
    scanInput(ship,dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::controlCannon(a::Node *node, float dt) {

  auto gun = CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  auto ship= CC_GNF(Ship, node, "ship");
  auto t= lpr->timer7;

  if (! gun->hasAmmo) {
    //throttle the cannon with timer
    if (cx::timerDone(t)) {
      ship->sprite->setSpriteFrame(ship->frame0);
      gun->hasAmmo=true;
      cx::undoTimer(t);
      SNPTR(lpr->timer7)
      fireMissile(node,dt);
    }
  } else {
    //TODO:
      if (MGML()->keyPoll(KEYCODE::KEY_SPACE)) {
      fireMissile(node,dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::fireMissile(a::Node *node, float dt) {

  auto gun= CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  auto ship= CC_GNF(Ship, node, "ship");

  auto cfg= MGMS()->getLCfg()->getValue();
  auto p= MGMS()->getPool("missiles");
  auto top= cx::getTop(ship->sprite);
  auto pos= ship->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    this->engine->reifyMissiles(36);
    ent= p->get();
  }

  ent->inflate(pos.x, top+4);

  lpr->timer7 = cx::reifyTimer( MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
  ship->sprite->setSpriteFrame(ship->frame1);

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::scanInput(a::Node *node, float dt) {

  auto m= CC_GNF(Motion, node, "motion");
  auto s= CC_GNF(Ship, node, "ship");

  m->right = MGML()->keyPoll( KEYCODE::KEY_RIGHT_ARROW);
  m->left = MGML()->keyPoll( KEYCODE::KEY_LEFT_ARROW);
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(a::Node *node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (ENP(lpr->timer0)) {
    lpr->timer0= cx::reifyTimer(MGML(), JS_FLOAT(cfg["marching"]));
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= cx::reifyTimer(MGML(), JS_FLOAT(cfg["bombing"]));
  }
}


NS_END(invaders)


