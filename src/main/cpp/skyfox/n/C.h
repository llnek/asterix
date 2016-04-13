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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(skyfox)

enum {
  kSpriteBomb,
  kSpriteShockwave,
  kSpriteMeteor,
  kSpriteHealth,
  kSpriteHalo,
  kSpriteSparkle,
  kSpriteUfo,
  kSpriteRay
};

enum {
  kBackground,
  kMiddleground,
  kForeground
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cloud : public f::CPixie {

  Cloud() {}

public:

  static owner<Cloud*> create() {
    auto z= mc_new(Cloud);
    z->initWithSpriteFrameName("cloud.png");
    z->autorelease();
    return z;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Meteor : public f::CPixie {

  Meteor() {}

public:

  static owner<Meteor*> create() {
    auto z= mc_new(Meteor);
    z->initWithSpriteFrameName("meteor.png");
    z->autorelease();
    return z;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Health : public f::CPixie {

  Health() {}

public:

  static owner<Health*> create() {
    auto z= mc_new(Health);
    z->initWithSpriteFrameName("health.png");
    z->autorelease();
    return z;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::CPixie {

  Bomb() {}

public:

  static owner<Bomb*> create() {
    auto sparkle = cx::reifySprite("sparkle.png");
    auto halo = cx::reifySprite("halo.png");
    auto sp = mc_new(Bomb);
    sp->initWithSpriteFrameName("bomb.png");
    sp->getTexture()->generateMipmap();

    auto sz = CC_CSIZE(sp);
    CC_POS2(sparkle, CC_ZW(sz) * 0.72, CC_ZH(sz) * 0.72);
    CC_POS2(halo, CC_ZW(sz) * 0.4, CC_ZH(sz) * 0.4);
    sp->addChild(sparkle, kMiddleground, kSpriteSparkle);
    sp->addChild(halo, 1, kSpriteHalo);
    sp->autorelease();
    CC_HIDE(sp);

    return sp;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ufo : public f::CPixie {

  Ufo() {}

public:

  static owner<Ufo*> create() {
    auto ray = cx::reifySprite("ray.png");
    auto sp= mc_new(Ufo);
    sp->initWithSpriteFrameName("ufo_1.png");
    auto sz= CC_CSIZE(sp);
    ray->setAnchorPoint(cx::anchorT());
    CC_POS2(ray, CC_ZW(sz) * 0.52, CC_ZH(sz) * 0.5);
    sp->addChild(ray, -1, kSpriteRay);
    CC_HIDE(sp);
    sp->autorelease();

    return sp;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {

  __decl_map(ecs::Node*,ecs::Node*, fallingObjects)
  __decl_comp_tpid( "n/GVars" )

  __decl_fz(difficultyInterval)
  __decl_fz(difficultyTimer)

  __decl_fz(meteorInterval)
  __decl_fz(meteorTimer)
  __decl_fz(meteorSpeed)

  __decl_fz(healthInterval)
  __decl_fz(healthTimer)
  __decl_fz(healthSpeed)

  __decl_fz(ufoInterval)
  __decl_fz(ufoTimer)
  __decl_bf(ufoKilled)

  __decl_iz(shockwaveHits)
  __decl_fz(energy)

  __decl_ptr(c::RepeatForever,rotateSprite)
  __decl_ptr(c::RepeatForever,swingHealth)
  __decl_ptr(c::RepeatForever,blinkRay)
  __decl_ptr(c::Sequence,shockwaveSequence)
  __decl_ptr(c::Sequence,groundHit)
  __decl_ptr(c::Sequence,explosion)
  __decl_ptr(c::Sprite,shockWave)
  __decl_ptr(c::ScaleTo,growBomb)
  __decl_ptr(c::Animate,ufoAnimation)

};



NS_END




