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

#if !defined(__XSYSTEM_H__)
#define __XSYSTEM_H__

#include "x2d/GameScene.h"
#include "ash/System.h"
#include "core/JSON.h"
#include "Factory.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename F> class CC_DLL XSystem : public a::System {
protected:

  XSystem(not_null<F*>, not_null<c::Dictionary*>);

  void sendEx(const MsgTopic &t, j::json m) {
    MGMS()->sendMsgEx(t, &m);
  }

  void send(const MsgTopic &t) {
    MGMS()->sendMsg(t);
  }

  c::Dictionary *state=nullptr;
  F *factory=nullptr;
  bool inited=false;

  NOCPYASS(XSystem)
  NODFT(XSystem)

public:

  virtual ~XSystem();
};

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
XSystem<F>::XSystem(not_null<F*> f, not_null<c::Dictionary*> d) {

  inited=false;
  CC_KEEP(d)
  factory=f;
  state=d;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
XSystem<F>::~XSystem() {
  CC_DROP(state)
}

NS_END(fusii)
#endif


