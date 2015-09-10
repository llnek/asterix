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

#if !defined(__PRIMITIVES_H__)
#define __PRIMITIVES_H__

#include "core/fusilli.h"
#include "base/CCRef.h"
#include "base/CCConsole.h"
#include "base/CCDataVisitor.h"
#include "platform/CCCommon.h"
NS_USING(cocos2d)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* DictVal(Dictionary* d, const string& key) {
  auto v= d->objectForKey(key);
  if (NNP(v)) {
    return static_cast<T*>(v);
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Box4 {
  Box4(float t, float r, float b, float l)
    : top(t), right(r), bottom(b), left(l)
  {}
  ~Box4()
  {}
  Box4(const Box4& b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
  }
  Box4& operator=(const Box4& b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
    return *this;
  }
  float top;
  float right;
  float bottom;
  float left;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Val2 : public Ref, public Clonable {
public:

  static Val2* create(float x, float y) {
    Val2* pRet = new Val2(x,y);
    pRet->autorelease();
    return pRet;
  }

  Val2(float x, float y)
  : _x(x), _y(y)
  {}

  Vec2 getValue() const {return Vec2(_x,_y); }

  virtual ~Val2() {
    CCLOGINFO("deallocing ~Val2: %p", this);
  }

  virtual void acceptVisitor(DataVisitor &visitor) { }

  virtual Val2* clone() const override {
    return Val2::create(_x,_y);
  }

private:
  float _x;
  float _y;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Size2 : public Ref, public Clonable {
public:

  static Size2* create(float w, float h) {
    Size2* pRet = new Size2(w,h);
    pRet->autorelease();
    return pRet;
  }

  Size2(float w, float h)
  : _w(w), _h(h)
  {}

  Size getValue() const {return Size(_w,_h); }

  virtual ~Size2() {
    CCLOGINFO("deallocing ~Size2: %p", this);
  }

  virtual void acceptVisitor(DataVisitor &visitor) { }

  virtual Size2* clone() const override {
    return Size2::create(_w,_h);
  }

private:
  float _w;
  float _h;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C3B : public Ref, public Clonable {
public:

  static C3B* create(const Color3B& c) {
    C3B* pRet = new C3B(c);
    pRet->autorelease();
    return pRet;
  }

  C3B(const Color3B& c)
  : _c(c)
  {}

  Color3B getValue() const {return Color3B(_c); }

  virtual ~C3B() {
    CCLOGINFO("deallocing ~C3B: %p", this);
  }

  virtual void acceptVisitor(DataVisitor &visitor) {  }

  virtual C3B* clone() const override {
    return C3B::create(_c);
  }

private:
  Color3B _c;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C4B : public Ref, public Clonable {
public:

  static C4B* create(const Color4B& c) {
    C4B* pRet = new C4B(c);
    pRet->autorelease();
    return pRet;
  }

  C4B(const Color4B& c)
  : _c(c)
  {}

  Color4B getValue() const {return Color4B(_c); }

  virtual ~C4B() {
    CCLOGINFO("deallocing ~C4B: %p", this);
  }

  virtual void acceptVisitor(DataVisitor &visitor) {  }

  virtual C4B* clone() const override {
    return C4B::create(_c);
  }

private:
  Color4B _c;
};







NS_END(fusilli)
#endif
