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

#if !defined(__ALIENS_H__)
#define __ALIENS_H__

#include "BaseSystem.h"
#include "n/gnodes.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Aliens : public BaseSystem {
private:

  NO__COPYASSIGN(Aliens)
  Aliens();

  a::NodeList* baddies;

public:

  virtual const a::SystemType TypeId() { return "n/Aliens"; }

  static Aliens* Create(Factory*, c::Dictionary*);
  virtual ~Aliens();

  virtual void RemoveFromEngine(a::Engine*);
  virtual void AddToEngine(a::Engine*);
  virtual bool Update(float dt);

  void ProcessMovement(a::Node*, float dt);
  void ProcessBombs(a::Node*, float dt);

  void DropBomb(float x, float y);
  void CheckBomb(AlienSquad* sqad);

  void MaybeShuffleAliens(AlienSquad* sqad);
  void TestDirX(f::ComObj* b, int stepx);

  void ForwardOneAlien(f::ComObj* a, float delta);
  void ShuffleOneAlien(f::ComObj* a, int stepx);

  void DoShuffle(AlienSquad* sqad);
  void DoForward(AlienSquad* sqad);

  f::ComObj* FindMinX(AlienSquad* sqad);
  f::ComObj* FindMaxX(AlienSquad* sqad);

  virtual int Priority() { return Motion; }

};


NS_END(invaders)
#endif

