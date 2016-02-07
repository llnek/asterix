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


void GameLayer::resetMeteor(void) {

    if (_fallingObjects.size() > 30) return;

    auto meteor = _meteorPool.at(_meteorPoolIndex);
	_meteorPoolIndex++;
	if (_meteorPoolIndex == _meteorPool.size()) _meteorPoolIndex = 0;


	int meteor_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    int meteor_target_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;

    meteor->stopAllActions();
    meteor->setPosition(Vec2(meteor_x, _screenSize.height + meteor->getBoundingBox().size.height * 0.5));

    //create action
    auto  rotate = RotateBy::create(0.5f ,  -90);
    auto  repeatRotate = RepeatForever::create( rotate );
    auto  sequence = Sequence::create (
               MoveTo::create(_meteorSpeed, Vec2(meteor_target_x, _screenSize.height * 0.15f)),
               CallFunc::create(std::bind(&GameLayer::fallingObjectDone, this, meteor) ),
               nullptr);

    meteor->setVisible ( true );
    meteor->runAction(repeatRotate);
    meteor->runAction(sequence);
    _fallingObjects.pushBack(meteor);
}

void GameLayer::resetUfo(void) {

    if (_ufo->isVisible()) return;

    float newX;
    float newY;

    //pick side
    if (rand() % 100 < 50) {
        newX = 0.0;
    } else {
        newX = _screenSize.width;
    }

    //pick height
    newY = rand() % (int) (_screenSize.height * 0.3f) + _screenSize.height * 0.3f;
    if (newY > _screenSize.height * 0.7f) newY = _screenSize.height * 0.7f;

    _ufo->stopAllActions();
    _ufo->setPosition(Vec2(newX, newY));
    _ufo->runAction( _ufoAnimation->clone());

    //set ray
    auto ray = _ufo->getChildByTag(kSpriteRay);
    ray->setVisible(false);
    ray->stopAllActions();
    ray->runAction( _blinkRay->clone());

    FiniteTimeAction *  sequence;
    if (newX == 0.0) {
        sequence = Sequence::create ( MoveTo::create(UFO_SPEED, Vec2(_screenSize.width * 1.1, newY)),
                                       CallFunc::create(std::bind(&GameLayer::animationDone, this, _ufo) ),
                                       nullptr);
    } else {
        sequence = Sequence::create ( MoveTo::create(UFO_SPEED, Vec2(-_screenSize.width * 0.1, newY)),
                                     CallFunc::create(std::bind(&GameLayer::animationDone, this, _ufo) ),
                                    nullptr);
    }

    _ufo->setVisible(true);
    _ufo->runAction(sequence);
    _ufoKilled = false;
    SimpleAudioEngine::getInstance()->playEffect("pew.wav", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::resetHealth() {

  if (ss->fallingObjects.size() > 30) { return; }

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





NS_END



