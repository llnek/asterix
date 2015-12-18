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

#include "core/XSystem.h"
#include "EFactory.h"
#include "n/GNodes.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Aliens : public f::XSystem<EFactory> {
public:

  virtual const a::SystemType typeId() { return "n/Aliens"; }
  Aliens(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);

  virtual int priority() { return a::AI; }

  virtual bool update(float);

  void forwardOneAlien(f::ComObj*, float delta);
  void shuffleOneAlien(f::ComObj*, int stepx);

  void processMovement(a::Node*, float dt);
  void processBombs(a::Node*, float dt);

  void dropBomb(float x, float y);
  void checkBomb(AlienSquad* );

  bool testDirX(f::ComObj* , int stepx);
  void maybeShuffleAliens(AlienSquad* );

  bool doShuffle(AlienSquad* );
  bool doForward(AlienSquad* );

  f::ComObj* findMinX(AlienSquad* );
  f::ComObj* findMaxX(AlienSquad* );

  virtual ~Aliens() {}
  NODFT(Aliens)
  NOCPYASS(Aliens)

  a::NodeList *baddies=nullptr;
};


NS_END(invaders)
#endif

