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

#if !defined(__BASESYSTEM_H__)
#define __BASESYSTEM_H__

#include "ash/System.h"
#include "Factory.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename F> class CC_DLL BaseSystem : public a::System {
protected:

  BaseSystem(not_null<F*>, not_null<c::Dictionary*>);

  c::Dictionary *state=nullptr;
  F *factory=nullptr;
  bool inited=false;

  NOCPYASS(BaseSystem)
  NODFT(BaseSystem)

public:

  virtual ~BaseSystem();
};

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
BaseSystem<F>::BaseSystem(not_null<F*> f, not_null<c::Dictionary*> d)

  : System(a::Error) {

  inited=false;
  CC_KEEP(d)
  factory=f;
  state=d;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
BaseSystem<F>::~BaseSystem() {
  CC_DROP(state)
}

NS_END(fusii)
#endif


