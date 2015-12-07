// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "Motions.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<a::Engine*> e, not_null<c::Dictionary*> f)
  : f::BaseSystem<EFactory>(e, f) {

  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine> e) {
  ShipMotionNode n;
  ships= e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::OnUpdate(float dt) {
  //const evt = this.evQ.length > 0 ? this.evQ.shift() : undef,
  auto node = ships->head;
  if (NNP(node)) {
    doIt(node, dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::doIt(a::Node* node, float dt) {
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onGui(a::Node* node, float dt) {
  auto ship = CC_GNF(Ship, node, "ship");
  auto bx= MGMS()->getEnclosureBox();
  auto loc = ship->pos();
  auto wc= cx::visRect();
  auto cur= ccpAdd(loc, c::Vec2()); //evt.delta);
  cur= ccpClamp(cur, ccp(bx.left, bx.bottom),
                 ccp(bx.right, bx.top));
  ship->setPos(cur.x, cur.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::onKey(a::Node* node, float dt) {
  auto motion= CC_GNF(Motion, node, "motion");
  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT)) {
    motion->right = true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT)) {
    motion->left= true;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_DOWN)) {
    motion->down = true;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_UP)) {
    motion->up= true;
  }
}



NS_END(terra)


