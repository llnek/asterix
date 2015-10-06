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
protected:
  virtual bool OnUpdate(float dt);

private:

  NO__CPYASS(Aliens)
  Aliens()=delete ;

  a::NodeList* baddies;

public:

  Aliens(not_null<Factory*>, not_null<c::Dictionary*>);

  virtual const a::SystemType TypeId() {
    return "n/Aliens";
  }

  virtual ~Aliens();

  virtual void RemoveFromEngine(not_null<a::Engine*>);
  virtual void AddToEngine(not_null<a::Engine*>);

  void ProcessMovement(not_null<a::Node*>, float dt);
  void ProcessBombs(not_null<a::Node*>, float dt);

  void CheckBomb(not_null<AlienSquad*> );
  void DropBomb(float x, float y);

  void MaybeShuffleAliens(not_null<AlienSquad*> );
  bool TestDirX(not_null<f::ComObj*> b, int stepx);

  void ForwardOneAlien(not_null<f::ComObj*> a, float delta);
  void ShuffleOneAlien(not_null<f::ComObj*> a, int stepx);

  bool DoShuffle(not_null<AlienSquad*> sqad);
  bool DoForward(not_null<AlienSquad*> sqad);

  f::ComObj* FindMinX(not_null<AlienSquad*> sqad);
  f::ComObj* FindMaxX(not_null<AlienSquad*> sqad);

  virtual int Priority() { return a::AI; }

};


NS_END(invaders)
#endif

