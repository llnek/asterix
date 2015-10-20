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

#include "BaseSystem.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
BaseSystem<F>::BaseSystem(not_null<F*> f, not_null<c::Dictionary*> d)
  : a::System(a::Error) {
  factory=f;
  state=d;
  state->retain();
}

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
BaseSystem<F>::~BaseSystem() {
  if (NNP(state)) { state->release(); }
}

//////////////////////////////////////////////////////////////////////////
//
template<typename F>
bool BaseSystem<F>::Update(float time) {
  //CCLOG("update called on system: %s", this->TypeId().c_str());
  return OnUpdate(time);
}

NS_END(fusii)


