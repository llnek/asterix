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
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Stager : public f::BaseSystem<EFactory> {
protected:

  virtual bool onUpdate(float);
  void sharedExplosion();
  void initBackSkies();
  void onceOnly();

public:

  virtual const a::SystemType typeId() { return "n/Stager"; }

  virtual int priority() { return a::PreUpdate; }

  Stager(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);
  virtual ~Stager() {}

  NOCPYASS(Stager)
  NODFT(Stager)

    bool inited;
    a::NodeList* ships;
};


NS_END(terra)
#endif

