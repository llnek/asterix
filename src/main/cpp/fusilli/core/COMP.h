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

#pragma once

#include "2d/CCSprite.h"
#include "Primitives.h"
#include "ecs/Ecs.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CGesture : public ecs::Component {
  MDECL_COMP_TPID("f/CGesture")
  DECL_BF(bottom)
  DECL_BF(top)
  DECL_BF(right)
  DECL_BF(left)
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CHealth : public ecs::Component {
  MDECL_COMP_TPID("f/CHealth")
  CHealth(float h) {
    alive=true;
    origHP=h;
    curHP=h;
  }
  DECL_FZ(origHP)
  DECL_FZ(curHP)
  DECL_BF(alive)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CAutoma : public ecs::Component {
  MDECL_COMP_TPID("f/CAutoma")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CHuman : public ecs::Component {
  MDECL_COMP_TPID("f/CHuman")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CStash : public ecs::Component {
  MDECL_COMP_TPID("f/CStash")
  DECL_FZ(score)
  CStash(float v) { score=v;}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMelee : public ecs::Component {
  MDECL_COMP_TPID("f/CMelee")
  DECL_BF(attacking)
  DECL_FZ(hurt)
  DECL_BF(selfDie)
  DECL_FZ(hurtRate)
  DECL_FZ(lastHurt)
  CMelee() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMove : public ecs::Component {
  MDECL_COMP_TPID("f/CMove")

  DECL_TD(c::Vec2, moveTarget)
  DECL_TD(c::Vec2, lastPos)
  DECL_TD(c::Vec2, nextPos)

  DECL_TD(c::Vec2, vel)
  DECL_TD(c::Vec2, acc)
  DECL_FZ(maxSpeed)
  DECL_FZ(maxAccel)
  DECL_FZ(speed)
  DECL_FZ(accel)

  CMove() {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CDraw : public ecs::Component {
  MDECL_COMP_TPID("f/CDraw")
  DECL_PTR(c::Node,node)
  CDraw(not_null<c::Node*> n) { node=n; }
  CDraw() {}
  void hide() { if (node) node->setVisible(false); }
  void show() { if (node) node->setVisible(true); }
  void inflate(float x, float y) {
    if (node) { node->setPosition(x,y); show(); }
  }
  void releaseInnerNode() { node=nullptr; }
  void removeInnerNode() {
    if (node) {node->removeFromParent();}
    node=nullptr;
  }
  void inflate() { show(); }
  void deflate() {
    if (node) {
      node->unscheduleAllCallbacks();
      node->stopAllActions();
      hide();
    }
  }
  float height() { return node ? node->boundingBox().size.height : 0; }
  float width() { return node ? node->boundingBox().size.width : 0; }
  const c::Rect bbox() { return node ? node->boundingBox() : c::Rect(0,0,0,0); }
  void setPos(float x, float y) { if (node) node->setPosition(x,y); }
  const c::Vec2 pos() { return node ? node->getPosition() : c::Vec2(0,0); }
  const c::Size csize() { return node ? CC_CSIZE(node) : c::Size(0,0); }
  float circum() { return node ? CC_CSIZE(node).width : 0; }
  float radius() { return node ? CC_CSIZE(node).width * 0.5 : 0; }
  int tag() { return node ? node->getTag() : 0; }
  virtual ~CDraw() {
     if (node) { node->removeFromParent(); node=nullptr; }
  }

};





NS_END



