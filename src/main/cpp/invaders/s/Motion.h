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

#if !defined(__MOTION_H__)
#define __MOTION_H__


#include "BaseSystem.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public BaseSystem {
private:

  NO__COPYASSIGN(Motion)
  Motion();

  a::NodeList* cannons;
  a::NodeList* ships;
  a::NodeList* aliens;

public:

  static Motion* Create(Factory*, c::Dictionary*);
  virtual ~Motion();

  virtual const a::SystemType TypeId() { return "n/Motion"; }

  virtual void RemoveFromEngine(a::Engine*);
  virtual void AddToEngine(a::Engine*);
  virtual bool Update(float dt);

  void ProcessAlienMotions(a::Node*,float dt);
  void ControlCannon(a::Node*, float dt);
  void FireMissile(a::Node*, float dt);
  void ScanInput(a::Node*, float dt);

  virtual int Priority() { return a::Motion; }

};



NS_END(invaders)
#endif


