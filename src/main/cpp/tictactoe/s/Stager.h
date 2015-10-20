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

#if !defined(__STAGER_H__)
#define __STAGER_H__

#include "core/BaseSystem.h"
#include "EFactory.h"

NS_ALIAS(f,fusii)
NS_ALIAS(a,ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Stager : public f::BaseSystem<EFactory> {
protected:

  virtual bool OnUpdate(float);

  void OnceOnly(a::Node*, float);
  void ShowGrid(a::Node*);
  void DoIt(a::Node*, float);

  NO__CPYASS(Stager)

public:

  Stager(not_null<a::Engine*> engine,
      not_null<c::Dictionary*> options);

  virtual void removeFromEngine(not_null<a::Engine*>);
  virtual void addToEngine(not_null<a::Engine*>);
//Priority: xcfg.ftypes.PreUpdate

  virtual ~Stager();
};


NS_END(tttoe)
#endif

