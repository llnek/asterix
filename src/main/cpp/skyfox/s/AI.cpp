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
  shared = engine->getNodeList(SharedNode().typeId());
  bombs = engine->getNodeList(BombNode().typeId());
  ufos = engine->getNodeList(UfoNode().typeId());
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

  auto ss=CC_GNLF(GVars,shared,"slots");
  auto bomb=CC_GNLF(Bomb,bombs,"bomb");
  auto ufo=CC_GNLF(Ufo,ufos,"ufo");
  auto wb=cx::visBox();

  ss->meteorTimer += dt;
  if (ss->meteorTimer > ss->meteorInterval) {
    ss->meteorTimer = 0;
    resetMeteor();
  }

  ss->ufoTimer += dt;
  if (ss->ufoTimer > ss->ufoInterval) {
    ss->ufoTimer = 0;
    resetUfo(ufo);
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

  if (bomb->sprite->isVisible()) {
    if (bomb->sprite->getScale() > 0.3f) {
      if (bomb->sprite->getOpacity() != 255) {
        bomb->sprite->setOpacity(255);
      }
    }
  }

  auto ray = ufo->sprite->getChildByTag(kSpriteRay);
  auto pos=ufo->pos();
  if (ufo->sprite->isVisible() &&
      ray->isVisible()) {
    if (pos.x > wb.right * 0.1f &&
        pos.x <= wb.right * 0.9f) {
      changeEnergy(-0.25);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetMeteor() {
  auto wb=cx::visBox();
  auto mtx = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto mx = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto ss = CC_GNLF(GVars,shared,"slots");
  auto p= MGMS()->getPool("Meteors");

  if (ss->fallingObjects.size() > 30) {
    return;
  }

  auto meteor = p->get();
  if (ENP(meteor)) {
    SCAST(GEngine*,engine)->createMeteors();
    meteor=p->get();
  }
  auto sz= meteor->csize();

  meteor->sprite->stopAllActions();
  meteor->sprite->setPosition(mx, wb.top + HHZ(sz));

  auto rr = c::RepeatForever::create( c::RotateBy::create(0.5f , -90));
  auto seq= c::Sequence::create(
      c::MoveTo::create(ss->meteorSpeed,
        c::Vec2(mtx, wb.top * 0.15f)),
      c::CallFunc::create(
        [=]() { this->fallingObjectDone(meteor); }),
      CC_NIL);

  meteor->show();
  meteor->sprite->runAction(rr);
  meteor->sprite->runAction(seq);
  ss->fallingObjects.insert(S__PAIR(f::ComObj*,f::ComObj*,meteor,meteor));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetUfo(Ufo *ufo) {

  auto UFO_SPEED = CC_CSV(c::Integer,"UFO+SPEED");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto wb= cx::visBox();
  auto sp= ufo->sprite;
  auto newY = cx::randFloat(wb.top * 0.3f) + wb.top * 0.3f;
  auto newX= cx::randSign() > 0 ? 0 : wb.right;

  if (sp->isVisible()) { return; }

  if (newY > wb.top * 0.7f) {
    newY = wb.top * 0.7f;
  }

  sp->stopAllActions();
  ufo->setPos(newX, newY);
  sp->runAction(ss->ufoAnimation->clone());

  auto ray = sp->getChildByTag(kSpriteRay);
  CC_HIDE(ray);
  ray->stopAllActions();
  ray->runAction(ss->blinkRay->clone());

  c::FiniteTimeAction *seq;
  if ((int)newX == 0) {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(wb.right * 1.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  } else {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(-wb.right * 0.1, newY)),
        c::CallFuncN::create(
          [](c::Node *p) { CC_HIDE(p); }),
        CC_NIL);
  }

  CC_SHOW(sp);
  sp->runAction(seq);
  ss->ufoKilled = false;
  cx::sfxPlay("pew");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetHealth() {

  auto ss= CC_GNLF(GVars,shared,"slots");
  auto hp= MGMS()->getPool("Healths");
  auto wb= cx::visBox();
  auto health = hp->get();

  if (ss->fallingObjects.size() > 30) { return; }
  if (ENP(health)) {
    SCAST(GEngine*,engine)->createHealths();
    health=hp->get();
  }

  auto htx = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto hx = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto sp=health->sprite;
  auto sz= health->csize();

  sp->stopAllActions();
  sp->setPosition(hx, wb.top + HHZ(sz));

  auto seq = c::Sequence::create(
         c::MoveTo::create(ss->healthSpeed,
           c::Vec2(htx, wb.top * 0.15f)),
         c::CallFunc::create(
           [=]() { this->fallingObjectDone(health); }),
         CC_NIL);

  CC_SHOW(sp);
  sp->runAction( ss->swingHealth->clone());
  sp->runAction(seq);
    ss->fallingObjects.insert(S__PAIR(f::ComObj*,f::ComObj*,health,health));
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::changeEnergy(float value) {
  auto ss= CC_GNLF(GVars,shared,"slots");
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
  auto ss=CC_GNLF(GVars,shared,"slots");

  ss->meteorInterval -= 0.15f;
  if (ss->meteorInterval < 0.25f) {
    ss->meteorInterval = 0.25f;
  }

  ss->meteorSpeed -= 1;
  if (ss->meteorSpeed < 4) {
    ss->meteorSpeed = 4;
  }

  ss->ufoInterval -= 0.1f;
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
void AI::fallingObjectDone(f::ComObj *pSender) {

  auto ss= CC_GNLF(GVars,shared,"slots");

  ss->fallingObjects.erase(pSender);
  pSender->sprite->stopAllActions();
  pSender->sprite->setRotation(0);

  if (pSender->sprite->getTag() == kSpriteMeteor) {
    changeEnergy(-15);
    pSender->sprite->runAction(ss->groundHit->clone() );
    //play explosion sound
    cx::sfxPlay("boom");
  } else {
    CC_HIDE(pSender->sprite);
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
  pSender->deflate();
}




NS_END



