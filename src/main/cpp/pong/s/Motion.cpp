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

#include "x2d/GameScene.h"
#include "Motion.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<EFactory*> f, not_null<c::Dictionary*> o)
  : XSystem<EFactory>(f,o) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine*> e) {
  PaddleNode n;
  nodeList= e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {
  if (MGMS()->isLive()) {
    for (auto node= nodeList->head; node; node=node->next) {
      doit(node, dt);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::doit(a::Node *node, float dt) {
  ongui(node,dt);
  onkey(node, dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void ongui(a::Node *node, float dt) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::onkey(a::Node *node, float dt) {
  auto p= node->paddle;
  auto m= node->motion;
  auto &cs = p->kcodes;

  if (MGMS()->keyPoll(cs[0])) {
    m->left=true;
  }

  if (MGMS()->keyPoll(cs[1])) {
    m->right=true;
  }

}


NS_END(pong)


