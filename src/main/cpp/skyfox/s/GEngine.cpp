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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto p= MGMS()->reifyPool("Meteors");

  p->preset([=]() {
    auto s= cx::reifySprite("meteor.png");
    MGML()->addAtlasItem("game-pics",s, kMiddleground, kSpriteMeteor);
    return mc_new1(Meteor,s);
  }, 50);

  p= MGMS()->reifyPool("Healths");
  p->preset([=]() {
    auto s= cx::reifySprite("health.png");
    s->setAnchorPoint(c::Vec2(0.5f, 0.8f));
    MGML()->addAtlasItem("game-pics",s, kMiddleground, kSpriteHealth);
    return mc_new1(Health,s);
  }, 20);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

}


NS_END

