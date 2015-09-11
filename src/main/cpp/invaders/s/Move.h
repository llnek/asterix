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
class CC_DLL Move : public System {
public:

  virtual ~Move();
  Move(options);

  virtual void RemoveFromEngine(Engine*);
  virtual void AddToEngine(Engine*);
  virtual bool Update(float dt);

  void ProcessShipMotions(Node*, float dt);
  void clamp(ComObj* ship);
  void MoveBombs(float dt);
  void MoveMissiles(float dt);

  virtual int Priority() override { return Move; }

private:
  DISALLOW_COPYASSIGN(Move)
  Move() {}
};




NS_END(invaders)



