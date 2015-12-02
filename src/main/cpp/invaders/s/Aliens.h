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
  virtual bool onUpdate(float);
private:
  NOCPYASS(Aliens)
  Aliens()=delete ;
  a::NodeList* baddies;
public:

  virtual const a::SystemType typeId() { return "n/Aliens"; }
  Aliens(not_null<EFactory*>, not_null<c::Dictionary*>);
  virtual ~Aliens() {}

  virtual void addToEngine(not_null<a::Engine*>);

  void processMovement(not_null<a::Node*>, float dt);
  void processBombs(not_null<a::Node*>, float dt);

  void checkBomb(not_null<AlienSquad*> );
  void dropBomb(float x, float y);

  void maybeShuffleAliens(not_null<AlienSquad*> );
  bool testDirX(not_null<f::ComObj*> b, int stepx);

  void forwardOneAlien(not_null<f::ComObj*> a, float delta);
  void shuffleOneAlien(not_null<f::ComObj*> a, int stepx);

  bool doShuffle(not_null<AlienSquad*> sqad);
  bool doForward(not_null<AlienSquad*> sqad);

  f::ComObj* findMinX(not_null<AlienSquad*> sqad);
  f::ComObj* findMaxX(not_null<AlienSquad*> sqad);

  virtual int priority() { return a::AI; }
};


NS_END(invaders)
#endif

