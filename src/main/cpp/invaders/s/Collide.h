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

#if !defined(__COLLIDE_H__)
#define __COLLIDE_H__


#include "BaseSystem.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Collide : public BaseSystem {
private:

  NO__CPYASS(Collide)
  Collide()=delete;

  a::NodeList* aliens;
  a::NodeList* ships;
  a::Engine* engine;

public:

  Collide(not_null<Factory*>, not_null<c::Dictionary*>);

  virtual const a::SystemType TypeId() {
    return "n/Collide";
  }

  virtual ~Collide();

  virtual void RemoveFromEngine(not_null<a::Engine*>);
  virtual void AddToEngine(not_null<a::Engine*> );
  virtual bool Update(float dt);

  bool MaybeCollide(not_null<f::ComObj*>, not_null<f::ComObj*>);
  void CheckMissilesBombs();
  void CheckMissilesAliens(not_null<a::Node*>);
  void CheckShipBombs(not_null<a::Node*>);
  void CheckShipAliens(not_null<a::Node*>, not_null<a::Node*>);

  virtual int Priority() { return a::Collide; }

};



NS_END(invaders)
#endif

