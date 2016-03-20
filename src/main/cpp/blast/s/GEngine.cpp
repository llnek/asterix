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
#include "n/Player.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto ent= this->reifyNode("Shared",true);
  auto sp = Player::create();
  auto ss= mc_new(GVars);
    auto wb= cx::visBox();
  init(ss);
  ent->checkin(ss);

  SCAST(c::Node*,sp)->setPosition(wb.cx, wb.cy);
  MGML()->addItem(sp, E_LAYER_PLAYER);

  ent= this->reifyNode("Player",true);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(sp);
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(f::CMove));

  addEnemyFormation(ss, sp);
  addPowerUp(ss, sp);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}


NS_END

