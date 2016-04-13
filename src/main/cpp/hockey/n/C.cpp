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

#include "C.h"
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void setPos(not_null<ecs::Node*> node, float x, float y) {

  auto p= CC_GEC(f::CPixie,node, "f/CPixie");
  auto mv= CC_GEC(f::CMove,node,"f/CMove");
  auto pos= CCT_PT(x,y);

  if (!mv->moveTarget.equals(pos)) {
    mv->moveTarget= pos;
  }

  CC_POS2(p, x,y);

}

//////////////////////////////////////////////////////////////////////////////
//
void readyPt(const s_vec<ecs::Node*>& mallets, ecs::Node *ball) {

  auto pv= CC_GEC(f::CMove,ball,"f/CMove");
  auto puck= CC_GEC(Puck,ball,"f/CPixie");
  auto wb=cx::visBox();

  F__LOOP(it,mallets) {
    auto &node= *it;
    auto mv=CC_GEC(f::CMove,node,"f/CMove");
    auto p=CC_GEC(Player,node,"f/CStats");
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



