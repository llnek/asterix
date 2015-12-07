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

#if !defined(__COBJS_H__)
#define __COBJS_H__

#include "2d/CCActionInterval.h"
#include "core/ComObj.h"
#include "core/XPool.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Alien"; }
  Alien(not_null<c::Sprite*>, int value, int rank);

  virtual ~Alien() {}

  NOCPYASS(Alien)
  NODFT(Alien)

  int rank;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/AlienSquad"; }
  const s_vec<f::ComObj*>& list() { return aliens->list(); }

  AlienSquad(not_null<f::XPool*> aliens, int step);
  int size() { return aliens->size(); }

  virtual ~AlienSquad() {}

  NOCPYASS(AlienSquad)
  NODFT(AlienSquad)

  f::XPool* aliens;
  int stepx;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Bomb"; }
  Bomb(not_null<c::Sprite*>);

  virtual ~Bomb() {}

  NOCPYASS(Bomb)
  NODFT(Bomb)

};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Cannon"; }

  Cannon(float coolDownWindow);

  virtual ~Cannon() {}

  NOCPYASS(Cannon)
  NODFT(Cannon)

  float coolDownWindow;
  bool hasAmmo;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Explosion"; }
  virtual void inflate(float x, float y);

  Explosion(not_null<c::Sprite*> );

  virtual ~Explosion() {}

  NOCPYASS(Explosion)
  NODFT(Explosion)

  float frameTime;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Looper : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Looper"; }

  NOCPYASS(Looper)
  DECLCZ(Looper)

  c::DelayTime* timer0;
  c::DelayTime* timer1;
  c::DelayTime* timer7;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Missile"; }
  Missile(not_null<c::Sprite*>);

  NOCPYASS(Missile)
  NODFT(Missile)

  virtual ~Missile() {}
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Motion"; }
  virtual ~Motion() {}

  NOCPYASS(Motion)

  Motion();

  bool right;
  bool left;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Ship"; }
  Ship(not_null<c::Sprite*>, const sstr&, const sstr&);

  virtual ~Ship() {}

  NOCPYASS(Ship)
  NODFT(Ship)

  sstr frame0;
  sstr frame1;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Velocity : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Velocity"; }
  Velocity(float vx, float vy);

  virtual ~Velocity() {}

  NOCPYASS(Velocity)
  NODFT(Velocity)

  float x;
  float y;
};



NS_END(invaders)
#endif

