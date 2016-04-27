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

#include "aeon/fusilli.h"
#include "JSON.h"
#include "cocos2d.h"
#include "Macros.h"
NS_ALIAS(c, cocos2d)
NS_BEGIN(fusii)

typedef FArrayPtr<c::Sprite> SpritePtrArr;
typedef FArrayPtr<c::Node> NodePtrArr;

//////////////////////////////////////////////////////////////////////////////
//
template<typename T> class CC_DLL Maybe {

  __decl_md(T, _value)
  __decl_bf(_isset)

public:
  explicit Maybe(T t) { _value=t; _isset=true; }
  Maybe<T>& operator=(const Maybe<T> &arg) {
    _value=arg._value;
    _isset=arg._isset;
    return *this;
  }
  Maybe(const Maybe<T> &arg) {
    _value=arg._value;
    _isset=arg._isset;
  }
  Maybe<T>& operator=(Maybe<T> &&arg) {
    _value=arg._value;
    _isset=arg._isset;
    return *this;
  }
  Maybe(Maybe<T> &&arg) {
    _value=arg._value;
    _isset=arg._isset;
  }
  ~Maybe() {}
  Maybe() {}
  T get() const { return _value; }
  bool isNone() const { return !_isset; }
};

typedef Maybe<float> MaybeFloat;
typedef Maybe<int> MaybeInt;

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* dictVal(not_null<c::Dictionary*> d, const sstr &key) {
  auto v= d->objectForKey(key);
  if (N_NIL(v)) {
    return static_cast<T*>(v);
  } else {
    return CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* reifyRefType() {
  T *p = mc_new(T) ;
  if (N_NIL(p) && p->init()) {
    p->autorelease();
  } else {
      mc_del_ptr(p);
  }
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void emptyQueue(s_que<T> &q) {
  while (! q.empty()) {
    q.pop();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
struct DLTimer {
  __decl_ptr(c::DelayTime, timer)
  __decl_fz(duration)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Box4 {
  Box4(float t, float r, float b, float l)
    : top(t), right(r), bottom(b), left(l) {
    cy= (top-bottom) * 0.5 + bottom;
    cx= (right-left) * 0.5 + left;
  }
  ~Box4() {}
  Box4() {}
  Box4(const Box4 &b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
    cy= (top-bottom) * 0.5 + bottom;
    cx= (right-left) * 0.5 + left;
  }
  Box4& operator=(const Box4 &b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
    cy= (top-bottom) * 0.5 + bottom;
    cx= (right-left) * 0.5 + left;
    return *this;
  }
  __decl_fz(bottom)
  __decl_fz(top)
  __decl_fz(cx)
  __decl_fz(cy)
  __decl_fz(right)
  __decl_fz(left)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Val2 : public c::Ref, public c::Clonable {

  __decl_fz(_x)
  __decl_fz(_y)

public:

  static Val2* create(float x, float y) {
    Val2 *pRet = new Val2(x,y);
    pRet->autorelease();
    return pRet;
  }

  Val2(float x, float y)
  : _x(x), _y(y)
  {}

  CCT_PT getValue() const { return CCT_PT(_x,_y); }

  virtual ~Val2() {
    CCLOGINFO("deallocing ~Val2: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) { }

  virtual Val2* clone() const override {
    return Val2::create(_x,_y);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Size2 : public c::Ref, public c::Clonable {

  __decl_fz(_w)
  __decl_fz(_h)

public:

  static Size2* create(float w, float h) {
    Size2 *pRet = new Size2(w,h);
    pRet->autorelease();
    return pRet;
  }

  static Size2* create(const c::Size z) {
    return create(z.width, z.height);
  }

  Size2(float w, float h)
  : _w(w), _h(h)
  {}

  c::Size getValue() const {return c::Size(_w,_h); }

  virtual ~Size2() {
    CCLOGINFO("deallocing ~Size2: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) { }

  virtual Size2* clone() const override {
    return Size2::create(_w,_h);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Box4R : public c::Ref, public c::Clonable {

  __decl_fz(_lf)
  __decl_fz(_t)
  __decl_fz(_r)
  __decl_fz(_b)

public:

  static Box4R* create(float t, float r, float b, float lf) {
    Box4R *pRet = new Box4R(t,r,b,lf);
    pRet->autorelease();
    return pRet;
  }

  static Box4R* create(const Box4& x) {
    Box4R *pRet = new Box4R(x);
    pRet->autorelease();
    return pRet;
  }

  Box4R(float t, float r, float b, float lf)
  : _t(t), _r(r), _b(b), _lf(lf)
  {}

  Box4R(const Box4& x) {
    _t= x.top;
    _r= x.right;
    _b= x.bottom;
    _lf= x.left;
  }

  Box4 getValue() const { return Box4(_t,_r,_b,_lf); }

  virtual ~Box4R() {
    CCLOGINFO("deallocing ~Box4R: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) { }

  virtual Box4R* clone() const override {
    return Box4R::create(_t,_r,_b,_lf);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL JsonObj : public c::Ref, public c::Clonable {

  __decl_md(j::json, _obj)

public:

  static JsonObj* create(const j::json& c) {
    JsonObj *pRet = new JsonObj(c);
    pRet->autorelease();
    return pRet;
  }

  static JsonObj* create() {
    JsonObj *pRet = new JsonObj();
    pRet->autorelease();
    return pRet;
  }

  JsonObj(const j::json &c)
  : _obj(c)
  {}

  JsonObj()
  {}

  j::json getValue() const { return j::json(_obj); }

  void setValue(j::json n) { _obj=n; }

  virtual ~JsonObj() {
    CCLOGINFO("deallocing ~JsonObj: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) {  }

  virtual JsonObj* clone() const override {
    return JsonObj::create(_obj);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C3B : public c::Ref, public c::Clonable {

  __decl_md(c::Color3B, _c)

public:

  static C3B* create(const c::Color3B& c) {
    C3B *pRet = new C3B(c);
    pRet->autorelease();
    return pRet;
  }

  C3B(const c::Color3B &c)
  : _c(c)
  {}

  c::Color3B getValue() const {return c::Color3B(_c); }

  virtual ~C3B() {
    CCLOGINFO("deallocing ~C3B: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) {  }

  virtual C3B* clone() const override {
    return C3B::create(_c);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C4B : public c::Ref, public c::Clonable {

  __decl_md(c::Color4B, _c)

public:

  static C4B* create(const c::Color4B &c) {
    C4B *pRet = new C4B(c);
    pRet->autorelease();
    return pRet;
  }

  C4B(const c::Color4B &c)
  : _c(c)
  {}

  c::Color4B getValue() const {return c::Color4B(_c); }

  virtual ~C4B() {
    CCLOGINFO("deallocing ~C4B: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) {  }

  virtual C4B* clone() const override {
    return C4B::create(_c);
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cell2D {
  Cell2D (int r, int c) { row=r; col=c; }
  Cell2D() {}
  __decl_iz(row)
  __decl_iz(col)
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cell2I {
  Cell2I (int x, int y) { this->x=x; this->y=y; }
  Cell2I() {}
  __decl_iz(x)
  __decl_iz(y)
};




NS_END




