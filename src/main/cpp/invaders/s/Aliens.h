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

#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/GNodes.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Aliens : public f::BaseSystem<EFactory> {
protected:

  void forwardOneAlien(f::ComObj* a, float delta);
  void shuffleOneAlien(f::ComObj* a, int stepx);

  void processMovement(a::Node*, float dt);
  void processBombs(a::Node*, float dt);

  void dropBomb(float x, float y);
  void checkBomb(AlienSquad* );

  bool testDirX(f::ComObj* b, int stepx);
  void maybeShuffleAliens(AlienSquad* );

  bool doShuffle(AlienSquad* sqad);
  bool doForward(AlienSquad* sqad);

  f::ComObj* findMinX(AlienSquad* sqad);
  f::ComObj* findMaxX(AlienSquad* sqad);

  virtual bool onUpdate(float);

  a::NodeList* baddies;

public:

  virtual const a::SystemType typeId() { return "n/Aliens"; }
  Aliens(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual int priority() { return a::AI; }
  virtual ~Aliens() {}

  NOCPYASS(Aliens)
  NODFT(Aliens)

};


NS_END(invaders)
#endif

