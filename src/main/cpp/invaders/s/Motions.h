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

#if !defined(__MOTIONS_H__)
#define __MOTIONS_H__

#include "BaseSystem.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public BaseSystem {
private:

  NO__CPYASS(Motions)
  Motions()=delete;

  a::NodeList* cannons;
  a::NodeList* ships;
  a::NodeList* aliens;
  bool right;
  bool left;

public:

  Motions(not_null<Factory*>, not_null<c::Dictionary*>);

  virtual const a::SystemType TypeId() {
    return "n/Motions";
  }

  virtual void RemoveFromEngine(not_null<a::Engine*>);
  virtual void AddToEngine(not_null<a::Engine*>);
  virtual bool Update(float dt);

  void ProcessAlienMotions(not_null<a::Node*>,float dt);
  void ControlCannon(not_null<a::Node*>, float dt);
  void FireMissile(not_null<a::Node*>, float dt);
  void ScanInput(not_null<a::Node*>, float dt);

  virtual int Priority() { return a::Motion; }

  virtual ~Motions();
};



NS_END(invaders)
#endif


