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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
    ufos=engine->getNodeList(UfoNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {

  auto ss= CC_GNLF(GVars,shared,"slots");
  auto ufo= CC_GNLF(Ufo,ufos,"ufo");
  auto ray = ufo->sprite->getChildByTag(kSpriteRay);
  auto sz= CC_CSIZE(ss->shockWave);
  s_vec<f::ComObj*> dead;
  float dx,dy;

  if (!ss->shockWave->isVisible()) {
    return;
  }

  F__LOOP(it,ss->fallingObjects) {
    auto co= it->second;
    dx = ss->shockWave->getPositionX() - co->sprite->getPositionX();
    dy = ss->shockWave->getPositionY() - co->sprite->getPositionY();
    if (pow(dx, 2) + pow(dy, 2) <= pow(HWZ(sz), 2)) {
      co->sprite->stopAllActions();
      co->sprite->runAction( ss->explosion->clone());
      cx::sfxPlay("boom");
      if (co->sprite->getTag() == kSpriteMeteor) {
          ++ss->shockwaveHits;
          auto msg= j::json({
              "score" , ss->shockwaveHits * 13 + ss->shockwaveHits * 2 });
          SENDMSGEX("/game/player/earnscore", &msg);
      }
      dead.push_back(co);
    }
  }

  F__LOOP(it,dead) { ss->fallingObjects.erase(*it); }

  if (ufo->sprite->isVisible() && ! ss->ufoKilled) {
    dx = ss->shockWave->getPositionX() - ufo->sprite->getPositionX();
    dy = ss->shockWave->getPositionY() - ufo->sprite->getPositionY();
    if (pow(dx, 2) + pow(dy, 2) <= pow(CC_CSIZE(ss->shockWave).width * 0.6f, 2)) {
      ss->ufoKilled = true;
      cx::pauseEffects();
      ufo->sprite->stopAllActions();
      ray->stopAllActions();
      CC_HIDE(ray);
      ufo->sprite->runAction( ss->explosion->clone());
      cx::sfxPlay("boom");
      ++ss->shockwaveHits;
      auto msg = j::json({
          "score", ss->shockwaveHits * 13 + ss->shockwaveHits * 4
          });
      SENDMSGEX("/game/player/earnscore", &msg);
    }
  }
}


NS_END




