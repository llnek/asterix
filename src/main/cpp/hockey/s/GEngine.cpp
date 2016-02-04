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
  auto sp= cx::loadSprite("mallet.png");
  auto ent= this->reifyEntity();
  ent->checkin( mc_new1(Mallet,sp,1));
  ent->checkin(mc_new(Gesture));

  sp= cx::loadSprite("mallet.png");
  ent= this->reifyEntity();
  ent->checkin( mc_new1(Mallet,sp,2));
  ent->checkin(mc_new(Gesture));

  sp= cx::loadSprite("puck.png");
  ent= this->reifyEntity();
  ent->checkin(mc_new1(Puck,sp));

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
void GEngine::readyPoint(a::NodeList *mallets, a::Node *ball) {
  auto puck= CC_GNF(Puck,ball,"puck");
  auto wb=cx::visbox();
  for (auto node=mallets->head;node;node=node->next) {
    auto m=CC_GNF(Mallet,node,"mallet");
    if (m->pnum == 2) {
      m->setPos(wb.cx, wb.top - m->circum());
    } else {
      m->setPos(wb.cx, m->circum());
    }
  }
  puck->setPos(wb.cx, wb.cy - puck->circum());
}




NS_END

