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
#include "ecs/Node.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CGesture : public ecs::Component {
  MDECL_COMP_TPID("f/CGesture")
  DECL_BF(down)
  DECL_BF(up)
  DECL_BF(right)
  DECL_BF(left)
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CHealth : public ecs::Component {
  virtual bool alive() { return curHP > 0; }
  MDECL_COMP_TPID("f/CHealth")
  CHealth(int h, int c) {
    origHP=h;
    curHP=c;
    assert(c <= h);
  }
  CHealth(int h=1) {
    origHP=h;
    curHP=h;
  }
  virtual void hurt(int damage=1) {
    if (!godMode) {
      curHP -= damage;
    }
  }
  virtual void reset() {
    godMode=false;
    curHP=origHP;
  }

  void enterGodMode() { godMode=true; }
  void exitGodMode() { godMode=false; }
  bool isGod() { return godMode; }

  DECL_IZ(origHP)
  DECL_IZ(curHP)
  DECL_BF(godMode)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Looper : public ecs::Component {
  virtual ~Looper() { CC_DROP(tm.timer); }
  Looper() {}
  MDECL_COMP_TPID("f/Looper")
  DECL_TD(DLTimer, tm)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Loopers : public ecs::Component {
  MDECL_COMP_TPID("f/Loopers")
  virtual ~Loopers() {
    F__LOOP(it,tms) {
      auto &z= *it;
      CC_DROP(z.timer);
    }
  }
  Loopers() {}
  s_vec<DLTimer> tms;
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
struct CC_DLL CStats : public ecs::Component {
  MDECL_COMP_TPID("f/CStats")
  DECL_FZ(value)
  CStats(int v) { value=v;}
  CStats() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMelee : public ecs::Component {
  MDECL_COMP_TPID("f/CMelee")
  long long lastDamageTime;
  DECL_BF(selfDie)
  DECL_FZ(damage)
  DECL_FZ(damageRate)
  CMelee() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMove : public ecs::Component {
  MDECL_COMP_TPID("f/CMove")

  DECL_TD(c::Vec2, moveTarget)
  DECL_TD(c::Vec2, lastPos)
  DECL_TD(c::Vec2, nextPos)

  DECL_TD(c::Vec2, maxSpeed)
  DECL_TD(c::Vec2, maxAccel)
  DECL_TD(c::Vec2, speed)
  DECL_TD(c::Vec2, accel)
  DECL_TD(c::Vec2, vel)
  DECL_TD(c::Vec2, acc)

  DECL_FZ(interval)//millis
  DECL_FZ(power)
  DECL_FZ(angle)

  CMove(const c::Vec2 &t, float ms, float ma) {
    moveTarget=t;
    maxSpeed.y=ms;
    maxSpeed.x=ms;
    maxAccel.y=ma;
    maxAccel.x=ma;
  }
  CMove(float ms, float ma) {
    maxSpeed.y=ms;
    maxSpeed.x=ms;
    maxAccel.y=ma;
    maxAccel.x=ma;
  }
  CMove() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CPixie : public ecs::Component {
  MDECL_COMP_TPID("f/CPixie")
  DECL_PTR(c::Node,node)
  CPixie(not_null<c::Node*> n) { node=n; }
  CPixie() {}
  virtual void hide() { if (node) node->setVisible(false); }
  virtual void show() { if (node) node->setVisible(true); }
  virtual void inflate(float x, float y) {
    if (node) { node->setPosition(x,y); show(); }
  }
  virtual void ejectNode() { node=nullptr; }
  virtual void removeNode() {
    if (node) {node->removeFromParent();}
    node=nullptr;
  }
  virtual void dispose() { removeNode(); }
  virtual void inflate() { show(); }
  virtual void deflate() {
    if (node) {
      node->unscheduleAllCallbacks();
      node->stopAllActions();
      hide();
    }
  }
  virtual float height() { return node ? node->boundingBox().size.height : 0; }
  virtual float width() { return node ? node->boundingBox().size.width : 0; }
  virtual const c::Rect bbox() { return node ? node->boundingBox() : c::Rect(0,0,0,0); }
  virtual void setPos(float x, float y) { if (node) node->setPosition(x,y); }
  virtual const c::Vec2 pos() { return node ? node->getPosition() : c::Vec2(0,0); }
  virtual const c::Size csize() { return node ? CC_CSIZE(node) : c::Size(0,0); }
  virtual float circum() { return node ? CC_CSIZE(node).width : 0; }
  virtual float radius() { return node ? CC_CSIZE(node).width * 0.5 : 0; }
  virtual int tag() { return node ? node->getTag() : 0; }
  virtual ~CPixie() {
    //if (node) { node->removeFromParent(); node=nullptr; }
  }

};





NS_END



