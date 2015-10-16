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

#include "core/Primitives.h"
#include "Factory.h"
#include "ash/Ash.h"
NS_ALIAS(c, cocos2d)
NS_ALIAS(a,ash)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL BaseSystem : public a::System {
protected:
  c::Dictionary* state;
  Factory* factory;

  void Set(not_null<Factory*>, not_null<c::Dictionary*>);
  BaseSystem();

  virtual bool OnUpdate(float) = 0;

private:

  NO__CPYASS(BaseSystem)

public:

  virtual bool Update(float time);
  virtual ~BaseSystem();

};






NS_END(fusii)
#endif


