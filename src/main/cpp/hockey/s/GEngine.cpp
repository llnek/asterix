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
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyNode("Mallet", true);
  auto sp= Mallet::create();
  // player1
  MGML()->addAtlasItem("game-pics", sp);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(sp);
  ent->checkin(mc_new1(Player,1));
  ent->checkin(mc_new(f::CMove));

  // player2
  ent= this->reifyNode("Mallet", true);
  sp= Mallet::create();
  MGML()->addAtlasItem("game-pics", sp);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(sp);
  ent->checkin(mc_new1(Player,2));
  ent->checkin(mc_new(f::CMove));

  // the ball
  ent= this->reifyNode("Puck", true);
  auto psp= Puck::create();
  MGML()->addAtlasItem("game-pics", psp);
  ent->checkin(mc_new(f::CAutoma));
  ent->checkin(mc_new(f::CMove));
  ent->checkin(psp);

  ent= this->reifyNode("Shared", true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(Resolve,this));
}




NS_END

