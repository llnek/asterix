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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "2d/CCSprite.h"
#include "Primitives.h"
#include "ecs/Node.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CGesture : public ecs::Component {
  bool hasMoved() { return down || up || right || left; }
  void reset() { down=up=right=left=false; }
  __decl_comp_tpid("f/CGesture")
  __decl_bf(down)
  __decl_bf(up)
  __decl_bf(right)
  __decl_bf(left)
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CHealth : public ecs::Component {
  virtual bool alive() { return curHP > 0; }
  __decl_comp_tpid("f/CHealth")
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
  void setHealth(int h) {
    origHP=h;
    curHP=h;
  }
  float percent() { return 100.0f * curHP/origHP; }
  float ratio() { return (float)curHP/(float)origHP; }
  bool isGod() { return godMode; }
  __decl_iz(origHP)
  __decl_iz(curHP)
  __decl_bf(godMode)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Looper : public ecs::Component {
  virtual ~Looper() { CC_DROP(tm.timer); }
  Looper() {}
  __decl_comp_tpid("f/Looper")
  __decl_md(DLTimer, tm)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Loopers : public ecs::Component {
  __decl_comp_tpid("f/Loopers")
  virtual ~Loopers() {
    F__LOOP(it,tms) {
      auto &z= *it;
      CC_DROP(z.timer);
    }
  }
  Loopers() {}
  __decl_vec(DLTimer,tms)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CAutoma : public ecs::Component {
  __decl_comp_tpid("f/CAutoma")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CHuman : public ecs::Component {
  __decl_comp_tpid("f/CHuman")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CTarget : public ecs::Component {
  __decl_comp_tpid("f/CTarget")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CStats : public ecs::Component {
  __decl_comp_tpid("f/CStats")
  __decl_iz(state)
  __decl_iz(value)
  CStats(int v) { value=v;}
  CStats() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMelee : public ecs::Component {
  __decl_comp_tpid("f/CMelee")
  long long lastDamageTime;
  __decl_bf(selfDie)
  __decl_fz(damage)
  __decl_fz(damageRate)
  CMelee() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CMove : public ecs::Component {
  __decl_comp_tpid("f/CMove")

  __decl_md(CCT_PT, moveTarget)
  __decl_md(CCT_PT, lastPos)
  __decl_md(CCT_PT, nextPos)

  __decl_md(CCT_V2, maxSpeed)
  __decl_md(CCT_V2, maxAccel)
  __decl_md(CCT_V2, speed)
  __decl_md(CCT_V2, accel)
  __decl_md(CCT_V2, vel)
  __decl_md(CCT_V2, acc)

  __decl_fz(interval)//millis
  __decl_fz(power)
  __decl_fz(angle)

  CMove(const CCT_PT &t, float ms, float ma)
  : CMove(ms, ma) {
    moveTarget=t;
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
class CC_DLL UICObj : public ecs::Component {
protected:

  virtual bool isAuto() { return _auto; }
  void flipAuto(bool b) { _auto=b; }
  void bindNode(c::Node *n) {
    _auto=true; _node=dynamic_cast<c::Node*>(n);
    //CCLOG("ecs called, node= %p", _node);
  }
  __decl_bf(_auto)

public:

  __decl_comp_tpid("f/CPixie")
  __decl_ptr(c::Node, _node)

  UICObj(c::Node *n) { _node=n; }
  UICObj() {
    //CCLOG("inside UICObj ctor");
  }
  virtual ~UICObj() {
    //CCLOG("Poo! I am dead %p", this);
  }

  virtual void hide() { CC_HIDE(_node); }
  virtual void show() { CC_SHOW(_node); }
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
  virtual const CCT_PT pos() {
    return _node ? _node->getPosition() : CC_ZPT; }
  virtual const CCT_SZ csize() {
    return _node ? CC_CSIZE(_node) : CC_ZSZ; }
  virtual float circum() {
    return _node ? CC_CSIZE(_node).width : 0; }
  virtual float radius() {
    return _node ? CC_CSIZE(_node).width * 0.5 : 0; }
  virtual int tag() { return _node ? _node->getTag() : 0; }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CPixie : public c::Sprite, public UICObj {
protected:

  CPixie() { bindNode(this); }

public:

  static owner<CPixie*> reifyFrame(const sstr &n) {
    auto z= mc_new(CPixie);
    z->initWithSpriteFrameName(n);
    z->autorelease();
    return z;
  }

  static owner<CPixie*> reifyFile(const sstr &n) {
    auto z= mc_new(CPixie);
    z->initWithFile(n);
    z->autorelease();
    return z;
  }

  virtual ~CPixie() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CDrawNode : public c::DrawNode, public UICObj {
protected:

  CDrawNode() { bindNode(this); }

public:

  static owner<CDrawNode*> reify() {
    auto z= mc_new(CDrawNode);
    z->init();
    z->autorelease();
    return z;
  }

  virtual ~CDrawNode() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CNode : public c::Node, public UICObj {
protected:
  CNode() { bindNode(this); }
public:
  virtual ~CNode() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL CLayer : public c::Layer, public UICObj {
protected:
  CLayer() { bindNode(this); }
public:
  virtual ~CLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ColorLayer : public c::LayerColor, public UICObj {
protected:
  ColorLayer() { bindNode(this); }
public:
  virtual ~ColorLayer() {}
};






NS_END



