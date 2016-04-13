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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared = _engine->getNodes("n/GVars")[0];
  _bomb = _engine->getNodes("f/CGesture")[0];
  _ufo = _engine->getNodes("f/CAutoma")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  auto bomb=CC_GEC(Bomb,_bomb,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto ufo=CC_GEC(Ufo,_ufo,"f/CPixie");
  auto wb=cx::visBox();

  ss->meteorTimer += dt;
  if (ss->meteorTimer > ss->meteorInterval) {
    ss->meteorTimer = 0;
    resetMeteor();
  }

  ss->ufoTimer += dt;
  if (ss->ufoTimer > ss->ufoInterval) {
    ss->ufoTimer = 0;
    resetUfo(_ufo);
  }

  ss->healthTimer += dt;
  if (ss->healthTimer > ss->healthInterval) {
    ss->healthTimer = 0;
    resetHealth();
  }

  ss->difficultyTimer += dt;
  if (ss->difficultyTimer > ss->difficultyInterval) {
    ss->difficultyTimer = 0;
    increaseDifficulty();
  }

  if (bomb->isVisible() &&
      bomb->getScale() > 0.3) {
    if ((int)bomb->getOpacity() != 255) {
      bomb->setOpacity(255);
    }
  }

  auto ray = CC_GCT(ufo,kSpriteRay);
  auto pos=ufo->getPosition();
  if (ufo->isVisible() &&
      ray->isVisible()) {
    if (pos.x > wb.right * 0.1 &&
        pos.x <= wb.right * 0.9) {
      changeEnergy(-0.25);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetMeteor() {
  auto wb=cx::visBox();
  auto mtx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto mx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto ss = CC_GEC(GVars,_shared,"n/GVars");

  if (ss->fallingObjects.size() > 30) {
  return; }

  auto po= MGMS()->getPool("Meteors");
  auto e = (ecs::Node*)po->take(true);
  auto meteor=CC_GEC(Meteor,e,"f/CPixie");
  auto sz= CC_CSIZE(meteor);

  CC_POS2(meteor, mx, wb.top + HHZ(sz));
  meteor->stopAllActions();

  auto rr = c::RepeatForever::create(
      c::RotateBy::create(0.5 , -90));
  auto seq= c::Sequence::create(
      c::MoveTo::create(ss->meteorSpeed,
        CCT_PT(mtx, wb.top * 0.15)),
      c::CallFunc::create(
        [=]() { this->fallingObjectDone(e); }),
      CC_NIL);

  CC_SHOW(meteor);
  meteor->runAction(rr);
  meteor->runAction(seq);

  ss->fallingObjects.insert(
      S__PAIR(ecs::Node*,ecs::Node*,e,e));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetUfo(ecs::Node *node) {

  auto UFO_SPEED = CC_CSV(c::Integer,"UFO+SPEED");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto ufo=CC_GEC(Ufo,node,"f/CPixie");
  auto wb= cx::visBox();
  auto newY = cx::randFloat(wb.top * 0.3) + wb.top * 0.3;
  auto newX= cx::randSign() > 0 ? 0 : wb.right;

  if (ufo->isVisible()) { return; }

  if (newY > wb.top * 0.7) {
    newY = wb.top * 0.7;
  }

  ufo->stopAllActions();
  CC_POS2(ufo, newX, newY);
  ufo->runAction(ss->ufoAnimation->clone());

  auto ray = CC_GCT(ufo,kSpriteRay);
  CC_HIDE(ray);
  ray->stopAllActions();
  ray->runAction(ss->blinkRay->clone());

  c::FiniteTimeAction *seq;
  if ((int)newX == 0) {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          CCT_PT(wb.right * 1.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  } else {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          CCT_PT(-wb.right * 0.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  }

  ss->ufoKilled = false;
  CC_SHOW(ufo);
  ufo->runAction(seq);
  cx::sfxPlay("pew");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetHealth() {

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();

  if (ss->fallingObjects.size() > 30) {
  return; }

  auto htx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto hx = cx::randFloat(wb.right * 0.8) + wb.right * 0.1;
  auto hp= MGMS()->getPool("Healths");
    auto e = (ecs::Node*)hp->take(true);
  auto health=CC_GEC(Health,e,"f/CPixie");
  auto sz= health->csize();

  CC_POS2(health, hx, wb.top + HHZ(sz));
  health->stopAllActions();

  auto seq = c::Sequence::create(
         c::MoveTo::create(ss->healthSpeed,
           CCT_PT(htx, wb.top * 0.15)),
         c::CallFunc::create(
           [=]() { this->fallingObjectDone(e); }),
         CC_NIL);

  CC_SHOW(health);
  health->runAction(ss->swingHealth->clone());
  health->runAction(seq);

  ss->fallingObjects.insert(
      S__PAIR(ecs::Node*,ecs::Node*,e,e));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::changeEnergy(float value) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  ss->energy += value;

  if (ss->energy <= 0) {
    ss->energy = 0;
    SENDMSG("/game/end");
  }

  if (ss->energy > 100) {
    ss->energy = 100;
  }

  SENDMSG("/game/hud/updateenergy");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::increaseDifficulty () {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  ss->meteorInterval -= 0.15;
  if (ss->meteorInterval < 0.25) {
    ss->meteorInterval = 0.25;
  }

  ss->meteorSpeed -= 1;
  if (ss->meteorSpeed < 4) {
    ss->meteorSpeed = 4;
  }

  ss->ufoInterval -= 0.1;
  if (ss->ufoInterval < 5) {
    ss->ufoInterval = 5;
  }

  ss->healthSpeed -= 1;
  if (ss->healthSpeed < 8) {
    ss->healthSpeed = 8;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::fallingObjectDone(ecs::Node *node) {

  auto ui= CC_GEC(f::CPixie,node,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  ss->fallingObjects.erase(node);

  ui->stopAllActions();
  ui->setRotation(0);

  if (ui->getTag() == kSpriteMeteor) {
    changeEnergy(-15);
    ui->runAction(ss->groundHit->clone() );
    //play explosion sound
    cx::sfxPlay("boom");
  } else {
    CC_HIDE(ui);
    if ((int)ss->energy == 100) {
      auto msg= j::json({
          {"score", 25}
          });
      SENDMSGEX("/game/player/earnscore", &msg);
    } else {
      changeEnergy(10);
    }
    //play health bonus sound
    cx::sfxPlay("health");
  }

  cx::hibernate(node);
}




NS_END



