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

/*
#include "platform/CCCommon.h"

#include "math/CCAffineTransform.h"
#include "math/CCGeometry.h"
#include "math/CCVertex.h"
#include "math/Mat4.h"
#include "math/MathUtil.h"
#include "math/Quaternion.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"

// Deprecated include
#include "deprecated/CCArray.h"
#include "deprecated/CCBool.h"
#include "deprecated/CCDictionary.h"
#include "deprecated/CCDouble.h"
#include "deprecated/CCFloat.h"
#include "deprecated/CCInteger.h"
#include "deprecated/CCNotificationCenter.h"
#include "deprecated/CCSet.h"
#include "deprecated/CCString.h"
// CCDeprecated.h must be included at the end
#include "deprecated/CCDeprecated.h"

#include "base/CCDataVisitor.h"
#include "base/CCConsole.h"
#include "base/CCRef.h"
*/

#include "aeon/fusilli.h"
#include "cocos2d.h"
#include "Macros.h"

NS_ALIAS(c, cocos2d)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
class CC_DLL Option {
private:
  NO__CPYASS(Option)
  bool isset;
  T value;
public:
  explicit Option(T t) { value=t; isset=true; }
  ~Option() {}
  Option() { isset=false; }
  T Get() { return value; }
  bool IsNone() { return isset; }
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* DictVal(not_null<c::Dictionary*> d, const stdstr& key) {
  auto v= d->objectForKey(key);
  if (NNP(v)) {
    return static_cast<T*>(v);
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* ReifyRefType() {
  T* pRet = new(std::nothrow) T();
  if (pRet && pRet->init()) {
    pRet->autorelease();
  } else {
    mc_del_ptr(pRet)
  }
  return pRet;
}


//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void EmptyQueue(s::queue<T>& q) {
  while (! q.empty()) {
    q.pop();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Box4 {
  Box4(float t, float r, float b, float l)
    : top(t), right(r), bottom(b), left(l)
  {}
  ~Box4() {}
  Box4(const Box4& b) {
    top=b.top;
    right=b.right;
    bottom=b.bottom;
    left=b.left;
  }
  Box4() {
    top= right= bottom= left=0;
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
class CC_DLL Val2 : public c::Ref, public c::Clonable {
public:

  static Val2* create(float x, float y) {
    Val2* pRet = new Val2(x,y);
    pRet->autorelease();
    return pRet;
  }

  Val2(float x, float y)
  : _x(x), _y(y)
  {}

  c::Vec2 getValue() const {return c::Vec2(_x,_y); }

  virtual ~Val2() {
    CCLOGINFO("deallocing ~Val2: %p", this);
  }

  virtual void acceptVisitor(c::DataVisitor &visitor) { }

  virtual Val2* clone() const override {
    return Val2::create(_x,_y);
  }

private:
  float _x;
  float _y;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Size2 : public c::Ref, public c::Clonable {
public:

  static Size2* create(float w, float h) {
    Size2* pRet = new Size2(w,h);
    pRet->autorelease();
    return pRet;
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

private:
  float _w;
  float _h;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C3B : public c::Ref, public c::Clonable {
public:

  static C3B* create(const c::Color3B& c) {
    C3B* pRet = new C3B(c);
    pRet->autorelease();
    return pRet;
  }

  C3B(const c::Color3B& c)
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

private:
  c::Color3B _c;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL C4B : public c::Ref, public c::Clonable {
public:

  static C4B* create(const c::Color4B& c) {
    C4B* pRet = new C4B(c);
    pRet->autorelease();
    return pRet;
  }

  C4B(const c::Color4B& c)
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

private:
  c::Color4B _c;
};







NS_END(fusii)
#endif

