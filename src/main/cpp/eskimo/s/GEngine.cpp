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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "n/Platform.h"
#include "n/Eskimo.h"
#include "n/GSwitch.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto ent= this->reifyNode("Shared", true);
  auto pp= MGMS()->reifyPool("Platforms");
  auto ps= MGMS()->reifyPool("Switches");
  auto ss= mc_new(GVars);
  ss->world= this->_world;
  ent->checkin(ss);

  pp->preset([=]() -> f::Poolable* {
    auto e= this->reifyNode("Platform");
    auto p= Platform::create(ss);
    MGML()->addAtlasItem("game-pics",p);
    e->checkin(p);
    return e;
  }, 50);

  ps->preset([=]() -> f::Poolable* {
    auto e= this->reifyNode("GSwitch");
    auto s= GSwitch::create(ss);
    MGML()->addAtlasItem("game-pics", s);
    e->checkin(s);
    return e;
  }, 30);

  ent= this->reifyNode("Eskimo", true);
  ent->checkin(Eskimo::create(ss));
  ent->checkin(mc_new(f::CGesture));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(AI,this));
}


NS_END

