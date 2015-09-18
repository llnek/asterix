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
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Resolve : public BaseSystem {
protected:
  a::NodeList* aliens;
  a::NodeList* ships;

private:

  NO__COPYASSIGN(Resolve)
  Resolve();

public:

  static Resolve* Create(Factory*, c::Dictionary*);
  virtual ~Resolve();

  virtual void RemoveFromEngine(a::Engine*);
  virtual void AddToEngine(a::Engine*);
  virtual bool Update(float dt);

  void CheckMissiles();
  void CheckBombs();
  void CheckAliens(a::Node*);
  void CheckShip(a::Node*);

  virtual int Priority() { return Resolve; }

};



NS_END(invaders)


