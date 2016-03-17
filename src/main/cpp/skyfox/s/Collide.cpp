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
  _shared= _engine->getNodes("n/GVars")[0];
  _ufo= _engine->getNodes("f/CAutoma")[0];
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


  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto ufo= CC_GEC(Ufo,_ufo,"f/CPixie");
  auto ray = CC_GCT(ufo->node,kSpriteRay);
  auto sz= CC_CSIZE(ss->shockWave);
  s_vec<ecs::Node*> dead;
    if (!ss->shockWave->isVisible()) { return; }

  F__LOOP(it,ss->fallingObjects) {
    auto e= it->second;
    auto co=CC_GEC(f::CPixie,e,"f/CPixie");
    auto pos= co->pos();
    auto dx = ss->shockWave->getPositionX() - pos.x;
    auto dy = ss->shockWave->getPositionY() - pos.y;

    if (pow(dx, 2) + pow(dy, 2) <= pow(HWZ(sz), 2)) {
      co->node->stopAllActions();
      co->node->runAction( ss->explosion->clone());
      cx::sfxPlay("boom");
      if (co->node->getTag() == kSpriteMeteor) {
        ++ss->shockwaveHits;
        auto msg= j::json({
            {"score" , (int) (ss->shockwaveHits * 13 + ss->shockwaveHits * 2 )}});
        SENDMSGEX("/game/player/earnscore", &msg);
      }
      dead.push_back(e);
    }
  }

  F__LOOP(it,dead) {
    ss->fallingObjects.erase(*it);
    cx::hibernate(*it);
  }

  if (ufo->isOvert() && ! ss->ufoKilled) {
    auto pos= ufo->pos();
    auto dx = ss->shockWave->getPositionX() - pos.x;
    auto dy = ss->shockWave->getPositionY() - pos.y;
    if (pow(dx, 2) + pow(dy, 2) <= pow(CC_CSIZE(ss->shockWave).width * 0.6, 2)) {
      ss->ufoKilled = true;
      cx::pauseEffects();
      ufo->node->stopAllActions();
      ray->stopAllActions();
      CC_HIDE(ray);
      ufo->node->runAction( ss->explosion->clone());
      cx::sfxPlay("boom");
      ++ss->shockwaveHits;
      auto msg = j::json({
          {"score", (int)(ss->shockwaveHits * 13 + ss->shockwaveHits * 4) }
          });
      SENDMSGEX("/game/player/earnscore", &msg);
    }
  }
}


NS_END




