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

#include "ash/Ash.h"
NS_USING(ash)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public System {
public:

  Motions(options);

  virtual void RemoveFromEngine(Engine*) override;
  virutal void AddToEngine(Engine*) override;
  virtual void Update(float dt) override;

  void ControlCannon(Node*, float dt);
  void FireMissile(Node*, float dt);
  void ScanInput(Node*, float dt);
  void ProcessAlienMotions(Node*,float dt);

  virtual int Priority() override { return Motion; }

private:

  DISALLOW_COPYASSIGN(Motions)
  Motions() {}
};


NS_END(invaders)


