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

  auto bomb=CC_GNLF(Bomb,bombs,"bomb");
  auto ufo=CC_GNLF(Ufo,ufos,"ufo");
  auto wb=cx::visBox();

  meteorTimer += dt;
  if (meteorTimer > _meteorInterval) {
    meteorTimer = 0;
    resetMeteor();
  }

  ufoTimer += dt;
  if (ufoTimer > _ufoInterval) {
    ufoTimer = 0;
    resetUfo();
  }

  healthTimer += dt;
  if (healthTimer > _healthInterval) {
    healthTimer = 0;
    resetHealth();
  }

  difficultyTimer += dt;
  if (difficultyTimer > _difficultyInterval) {
    difficultyTimer = 0;
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

  auto ss = CC_GNLF(GVars,shared,"slots");
  auto p= MGMS()->getPool("Meteors");
  auto wb = cx::visBox();

  if (ss->fallingObjects.size() > 30) { return; }

  auto meteor = p->get();
  auto meteor_x = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto meteor_target_x = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  auto mz= meteor->csize();

  meteor->stopAllActions();
  meteor->setPosition(meteor_x, wb.top + HHZ(mz));

  auto rotate = c::RotateBy::create(0.5f ,  -90);
  auto repeatRotate = c::RepeatForever::create(rotate);
  auto seq= c::Sequence::create (
      c::MoveTo::create(ss->meteorSpeed,
        c::Vec2(meteor_target_x, wb.top * 0.15f)),
      c::CallFunc::create([=]() {
        this->fallingObjectDone(meteor);
        }),
      CC_NIL);

  meteor->setVisible (true);
  meteor->runAction(repeatRotate);
  meteor->runAction(sequence);
  ss->fallingObjects.push_back(meteor);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetUfo(Ufo *ufo) {

  if (ufo->sprite->isVisible()) { return; }

  auto UFO_SPEED = CC_CSV(c::Integer,"UFO+SPEED");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto wb= cx::visBox();
  auto newY = cx::randFloat(wb.top * 0.3f) + wb.top * 0.3f;
  auto newX= cx::randSign() > 0 ? 0 : wb.right;

  if (newY > wb.top * 0.7f) {
    newY = wb.top * 0.7f;
  }

  ufo->sprite->stopAllActions();
  ufo->setPos(newX, newY);
  ufo->sprite->runAction(ss->ufoAnimation->clone());

  auto ray = ufo->sprite->getChildByTag(kSpriteRay);
  ray->sprite->setVisible(false);
  ray->sprite->stopAllActions();
  ray->sprite->runAction(ss->blinkRay->clone());

  FiniteTimeAction *  sequence;
  if ((int)newX == 0) {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(wb.right * 1.1, newY)),
        c::CallFunc::create([](c::Node *p) {
          p->setVisible(false);
          }),
        CC_NIL);
  } else {
    seq= c::Sequence::create(
        c::MoveTo::create(
          UFO_SPEED,
          c::Vec2(-wb.right * 0.1, newY)),
        c::CallFunc::create([](c::Node *p) {
          p->setVisible(false);
          }),
        CC_NIL);
  }

  ufo->sprite->setVisible(true);
  ufo->sprite->runAction(sequence);
  ss->ufoKilled = false;
  cx::sfxPlay("pew");
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetHealth() {

  if (ss->fallingObjects.size() > 30) { return; }

  auto ss= CC_GNLF(GVars,shared,"slots");
  auto hp= MGMS()->getPool("Healths");
  auto health = hp->get();
  if (ENP(health)) {
    //?
  }

	int health_x = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;
  int health_target_x = cx::randFloat(wb.right * 0.8f) + wb.right * 0.1f;

  health->stopAllActions();
  health->setPosition(health_x,
      wb.top + health->getBoundingBox().size.height * 0.5);

  auto seq = c::Sequence::create(
         c::MoveTo::create(ss->healthSpeed,
           c::Vec2(health_target_x, wb.top * 0.15f)),
         c::CallFunc::create([=]() {
           this->fallingObjectDone(health);
           }),
         CC_NIL);

  health->inflate();
  health->runAction( ss->swingHealth->clone());
  health->runAction(seq);
  ss->fallingObjects.push_back(health);
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

  SENDMSG("/game/hud/updateEnergy");
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
void AI::fallingObjectDone(c::Node *pSender) {

  //_fallingObjects.erase(_fallingObjects.find( (Sprite *)pSender));
  pSender->stopAllActions();
  pSender->setRotation(0);

  if (pSender->getTag() == kSpriteMeteor) {
    changeEnergy(-15);
    pSender->runAction(ss->groundHit->clone() );
    //play explosion sound
    cx::sfxPlay("boom");
  } else {
    pSender->setVisible(false);
    if (ss->energy == 100) {
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
}




NS_END



