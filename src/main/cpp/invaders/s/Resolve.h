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

#if !defined(__RESOLVE_H__)
#define __RESOLVE_H__


#include "BaseSystem.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Resolve : public BaseSystem {
private:

  NO__CPYASS(Resolve)
  Resolve();

  void Init();

  a::NodeList* aliens;
  a::NodeList* ships;

public:

  Resolve(not_null<Factory*>, not_null<c::Dictionary*>);
  virtual ~Resolve();

  virtual const a::SystemType TypeId() {
    return "s/Resolve"; }

  virtual void RemoveFromEngine(not_null<a::Engine*> );
  virtual void AddToEngine(not_null<a::Engine*> );
  virtual bool Update(float dt);

  void CheckMissiles();
  void CheckBombs();
  void CheckAliens(not_null<a::Node*> );
  void CheckShip(not_null<a::Node*> );

  virtual int Priority() { return a::Resolve; }

};



NS_END(invaders)
#endif


