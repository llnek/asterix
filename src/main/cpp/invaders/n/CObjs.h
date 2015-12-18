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
struct CC_DLL Alien : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Alien"; }
  Alien(not_null<c::Sprite*>, int value, int rank);

  virtual ~Alien() {}
  NODFT(Alien)
  NOCPYASS(Alien)

  int rank;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public a::Component {

  virtual const a::COMType typeId() { return "n/AlienSquad"; }
  const s_vec<f::ComObj*>& list() { return aliens->list(); }

  AlienSquad(not_null<f::XPool*> aliens, int step);
  int size() { return aliens->size(); }

  virtual ~AlienSquad() {}
  NODFT(AlienSquad)
  NOCPYASS(AlienSquad)

  f::XPool *aliens;
  int stepx;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Bomb"; }
  Bomb(not_null<c::Sprite*>);

  virtual ~Bomb() {}
  NODFT(Bomb)
  NOCPYASS(Bomb)
};

//////////////////////////////////////////////////////////////////////////////
struct CC_DLL Cannon : public a::Component {

  virtual const a::COMType typeId() { return "n/Cannon"; }

  Cannon(float coolDownWindow);

  virtual ~Cannon() {}
  NODFT(Cannon)
  NOCPYASS(Cannon)

  float coolDownWindow;
  bool hasAmmo;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::ComObj {

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
struct CC_DLL Looper : public a::Component {

  virtual const a::COMType typeId() { return "n/Looper"; }

  virtual ~Looper();
  Looper();
  NOCPYASS(Looper)

  c::DelayTime *timer7=nullptr;
  c::DelayTime *timer0=nullptr;
  c::DelayTime *timer1=nullptr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Missile"; }
  Missile(not_null<c::Sprite*>);

  virtual ~Missile() {}
  NOCPYASS(Missile)
  NODFT(Missile)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {

  virtual const a::COMType typeId() { return "n/Motion"; }

  virtual ~Motion() {}
  Motion() {}
  NOCPYASS(Motion)

  bool right=false;
  bool left=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Ship"; }
  Ship(not_null<c::Sprite*>, const sstr&, const sstr&);

  virtual ~Ship() {}
  NODFT(Ship)
  NOCPYASS(Ship)

  sstr frame0;
  sstr frame1;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Velocity : public a::Component {

  virtual const a::COMType typeId() { return "n/Velocity"; }
  Velocity(float vx, float vy) {
    x=vx; y=vy;
  }

  virtual ~Velocity() {}
  NODFT(Velocity)
  NOCPYASS(Velocity)

  float x=0;
  float y=0;
};

NS_END(invaders)
#endif

