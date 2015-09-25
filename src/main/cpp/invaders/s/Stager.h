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

#include "BaseSystem.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Stager : public BaseSystem {
protected:
  void Fire(const stdstr& t, void* evt);

  void InitAlienSize();
  void InitShipSize();

  void OnceOnly();

  a::NodeList* ships;
  bool inited;

private:

  NO__COPYASSIGN(Stager)
  Stager();
  void Init();

public:

  Stager(not_null<Factory*>, not_null<c::Dictionary*>);

  virtual const a::SystemType TypeId() {
    return "s/Stager"; }

  virtual void RemoveFromEngine(not_null<a::Engine*>);
  virtual void AddToEngine(not_null<a::Engine*>);

  virtual bool Update(float dt);

  virtual int Priority() { return a::PreUpdate; }

  virtual ~Stager();

};



NS_END(invaders)
#endif

