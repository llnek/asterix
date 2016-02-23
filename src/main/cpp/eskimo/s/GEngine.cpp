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
#include "n/Platform.h"
#include "n/GSwitch.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto pp= MGMS()->reifyPool("Platforms");
  auto ps= MGMS()->reifyPool("Switches");
  auto ent= this->reifyEntity();
  auto ss= mc_new(GVars);
  ss->world=this->world;
  ent->checkin(ss);

  pp->preset([=]() -> f::Poolable* {
    auto p= mc_new1(Platform,ss);
    MGML()->addAtlasItem("game-pics",p->node);
    return p;
  }, 50);

  ps->preset([=]() -> f::Poolable* {
    auto s= GSwitch::create(ss);
    MGML()->addAtlasItem("game-pics", s->node);
    return s;
  }, 30);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

}


NS_END

