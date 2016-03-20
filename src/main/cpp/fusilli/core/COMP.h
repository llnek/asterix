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
class CC_DLL UIComObj : public ecs::Component {
protected:
  void bindNode(c::Node* n) { _node=n; _auto=true; }
  virtual bool isAuto() { return _auto; }
  DECL_BF(_auto)
  void flipAuto(bool b) { _auto=b; }
public:

  MDECL_COMP_TPID("f/CPixie")
  DECL_PTR(c::Node, _node)

  UIComObj(c::Node *n) { _node=n; }
  UIComObj() {}
  virtual ~UIComObj() {
    CCLOG("Poo! I am dead %p", this);
  }

  virtual void hide() { if (_node) _node->setVisible(false); }
  virtual void show() { if (_node) _node->setVisible(true); }
  virtual void inflate(float x, float y) {
    if (_node) {
      _node->setPosition(x,y);
      show();
    }
  }
  virtual void inflate() { show(); }
  virtual void deflate() {
    if (_node) {
      _node->unscheduleAllCallbacks();
      _node->stopAllActions();
      hide();
    }
  }
  virtual float height() {
    return _node ? _node->boundingBox().size.height : 0; }
  virtual float width() {
    return _node ? _node->boundingBox().size.width : 0; }
  virtual const c::Rect bbox() {
    return _node ? _node->boundingBox() : CC_ZRT; }
  virtual void setPos(float x, float y) { if (_node) _node->setPosition(x,y); }
  virtual const c::Vec2 pos() {
    return _node ? _node->getPosition() : CC_ZPT; }
  virtual const c::Size csize() {
    return _node ? CC_CSIZE(_node) : CC_ZSZ; }
  virtual float circum() {
    return _node ? CC_CSIZE(_node).width : 0; }
  virtual float radius() {
    return _node ? CC_CSIZE(_node).width * 0.5 : 0; }
  virtual int tag() { return _node ? _node->getTag() : 0; }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CPixie : public c::Sprite, public UIComObj {

  CPixie() { bindNode(this); }

public:

  static owner<CPixie*> reifyFrame(const sstr &n) {
    auto z= new CPixie();
    z->initWithSpriteFrameName(n);
    z->autorelease();
    return z;
  }

  static owner<CPixie*> reifyFile(const sstr &n) {
    auto z= new CPixie();
    z->initWithFile(n);
    z->autorelease();
    return z;
  }

  virtual ~CPixie() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CDrawNode : public c::DrawNode, public UIComObj {

  CDrawNode() {
    bindNode(this);
  }

public:

  static owner<CDrawNode*> reify() {
    auto z= new CDrawNode();
    z->init();
    z->autorelease();
    return z;
  }

  virtual ~CDrawNode() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C2DNode : public c::Node, public UIComObj {

  C2DNode() {
    bindNode(this);
  }

public:

  static owner<C2DNode*> reify() {
    auto z= new C2DNode();
    z->init();
    z->autorelease();
    return z;
  }

  virtual ~C2DNode() {}
};


NS_END



