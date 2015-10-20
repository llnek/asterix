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

#if !defined(__MOVE_H__)
#define __MOVE_H__

#include "core/BaseSystem.h"
#include "EFactory.h"
#include "n/gnodes.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Move : public f::BaseSystem<EFactory> {
protected:
  virtual bool OnUpdate(float dt);
private:

  NO__CPYASS(Move)
  Move()=delete;

  a::NodeList* ships;

public:

  Move(not_null<EFactory*>, not_null<c::Dictionary*> );

  virtual const a::SystemType TypeId() {
    return "s/Move";
  }

  virtual ~Move() {}

  virtual void AddToEngine(not_null<a::Engine*>);

  void ProcessShipMotions(not_null<a::Node*>, float dt);
  void Clamp(not_null<Ship*> ship);

  void MoveBombs(float dt);
  void MoveMissiles(float dt);

  virtual int Priority() { return a::Move; }

};




NS_END(invaders)
#endif



