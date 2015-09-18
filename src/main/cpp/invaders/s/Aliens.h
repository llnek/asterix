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
NS_ALAIS(c, cocos2d)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Aliens : public BaseSystem {
protected:
  NodeList* aliens;

private:

  NO__COPYASSIGN(Aliens)
  Aliens();

public:

  static Aliens* Create(Factory*, c::Dictionary*);
  virtual ~Aliens();

  virtual void RemoveFromEngine(a::Engine*);
  virtual void AddToEngine(a::Engine*);
  virtual bool Update(float dt);

  void ProcessMovement(a::Node*, float dt);
  void ProcessBombs(a::Node*, float dt);

  void DropBomb(float x, float y);
  void CheckBomb(a::Node* sqad);

  void MaybeShuffleAliens(a::Node* sqad);
  void TestDirX(f::ComObj* b, int stepx);

  void ForwardOneAlien(f::ComObj* a, float delta);
  void ShuffleOneAlien(f::ComObj* a, int stepx);

  void DoShuffle(a::Node* sqad);
  void DoForward(a::Node* sqad);

  float FindMinX(a::Node* sqad);
  float FindMaxX(a::Node* sqad);

  virtual int Priority() { return Motion; }

};


NS_END(invaders)


