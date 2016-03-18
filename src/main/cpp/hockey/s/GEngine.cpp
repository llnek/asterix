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
  auto sp= cx::reifySprite("mallet.png");
  // player1
  MGML()->addAtlasItem("game-pics", sp);
  ent->checkin(mc_new1(Mallet,sp));
  ent->checkin(mc_new1(Player,1));
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CMove));

  // player2
  ent= this->reifyNode("Mallet", true);
  sp= cx::reifySprite("mallet.png");
  MGML()->addAtlasItem("game-pics", sp);
  ent->checkin(mc_new1(Mallet,sp));
  ent->checkin(mc_new1(Player,2));
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CMove));

  // the ball
  ent= this->reifyNode("Puck", true);
  sp= cx::reifySprite("puck.png");
  MGML()->addAtlasItem("game-pics", sp);
  ent->checkin(mc_new(f::CAutoma));
  ent->checkin(mc_new(f::CMove));
  ent->checkin(mc_new1(Puck,sp));

  ent= this->reifyNode("Shared", true);
  ent->checkin(mc_new(GVars));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(Resolve,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::readyPt(
    const s_vec<ecs::Node*>& mallets, ecs::Node *ball) {

  auto pv= CC_GEC(f::CMove,ball,"f/CMove");
  auto puck= CC_GEC(Puck,ball,"f/CPixie");
  auto wb=cx::visBox();

  F__LOOP(it,mallets) {
    auto node= *it;
    auto mv=CC_GEC(f::CMove,node,"f/CMove");
    auto p=CC_GEC(Player,node,"n/Player");
    auto m=CC_GEC(Mallet,node,"f/CPixie");

    if (p->value == 2) {
      setPos(node, wb.cx, wb.top - m->circum());
    } else {
      setPos(node, wb.cx, m->circum());
    }
  }

  setPos(ball, wb.cx, wb.cy - puck->circum());
  pv->vel.x= -10;
  pv->vel.y= 10;
}




NS_END

