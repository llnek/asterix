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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Motion.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : XSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////////
//
Motions::~Motions() {
  cx::undoTimer(timer);
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena= e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::onceOnly() {
  mkThrottler();
  inited=true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {
  auto n= arena->head;
  if (MGMS()->isLive()) {

    if (!this->inited) {
      onceOnly();
    } else {
      doit(n, dt);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::doit(a::Node *node, float dt) {
  onkey(node, dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::onkey(a::Node *node, float dt) {

  if (!cx::timerDone(timer)) {
    return;
  } else {
  }

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    sftRight(node, dt);
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    sftLeft(node, dt);
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    rotRight(node, dt);
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    rotLeft(node, dt);
  }
  if (MGML()->keyPoll(KEYCODE::KEY_SPACE)) {
    sftDown(node, dt);
  }

  mkThrottler();
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::mkThrottler() {
  auto w= CC_CSV(c::Float, "THROTTLEWAIT");
  cx::undoTimer(timer);
  timer= cx::reifyTimer(MGML(),w);
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::sftRight(a::Node *node, float dt) {
  CC_GNF(Motion, node, "motion")->right=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::sftLeft(a::Node *node, float dt) {
  CC_GNF(Motion, node, "motion")->left=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::sftDown(a::Node *node, float dt) {
  CC_GNF(Motion, node, "motion")->down=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::rotRight(a::Node *node, float dt) {
  CC_GNF(Motion, node, "motion")->rotr=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::rotLeft(a::Node *node, float dt) {
  CC_GNF(Motion, node, "motion")->rotl=true;
}


NS_END(tetris)


