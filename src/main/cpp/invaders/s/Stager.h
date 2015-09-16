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

#include "platform/CCCommon.h"
#include "ash/Ash.h"
#include "BaseSystem.h"
NS_ALIAS(a, ash)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Stager : public BaseSystem {
protected:

  Stager();

private:

  DISALLOW_COPYASSIGN(Stager)

public:

  static Stager* Create(Factory* f, cc::Dictionary* options);

  virtual void RemoveFromEngine(a::Engine*);

  virtual void AddToEngine(a::Engine*);

  void InitAlienSize();

  void InitShipSize();

  virtual bool Update(float dt);

  void OnceOnly();

  virtual int Priority() { return PreUpdate; }

  virtual ~Stager();

};



NS_END(invaders)

