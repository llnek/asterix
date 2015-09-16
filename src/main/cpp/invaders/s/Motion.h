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
#include "ash/Ash.h"
NS_ALIAS(f, fusilli)
NS_ALIAS(a, ash)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public BaseSystem {
private:

  DISALLOW_COPYASSIGN(Motions)
  Motions();

public:

  static Motions* Create(Factory*, cc::Dictionary*);
  virtual ~Motions();

  virtual void RemoveFromEngine(a::Engine*);
  virutal void AddToEngine(a::Engine*);
  virtual bool Update(float dt);

  void ControlCannon(a::Node*, float dt);
  void FireMissile(a::Node*, float dt);
  void ScanInput(a::Node*, float dt);
  void ProcessAlienMotions(a::Node*,float dt);

  virtual int Priority() { return Motion; }

};



NS_END(invaders)


