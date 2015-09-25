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
NS_ALIAS(f, fusilli)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public f::ComObj {
private:

  NO__CPYASS(Alien)
  Alien();

public:

  virtual const a::COMType TypeId() { return "n/Alien"; }

  Alien(not_null<c::Sprite*>, int value, int rank);
  virtual ~Alien();

  int rank;

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public a::Component {
private:

  NO__CPYASS(AlienSquad)
  AlienSquad();

public:

  virtual const a::COMType TypeId() { return "n/AlienSquad"; }

  AlienSquad(not_null<f::XPool*> aliens, int step);
  virtual ~AlienSquad();

  const s::vector<f::ComObj*>& Elements() {
    return aliens->Elements(); }

  int Size() { return aliens->Size(); }

  f::XPool* aliens;
  int stepx;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
private:
  NO__CPYASS(Bomb)
  Bomb();
public:

  virtual const a::COMType TypeId() { return "n/Bomb"; }

  Bomb(not_null<c::Sprite*>);
  virtual ~Bomb();

  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public a::Component {
private:
  NO__CPYASS(Cannon)
  Cannon();
public:

  virtual const a::COMType TypeId() { return "n/Cannon"; }

  Cannon(float coolDownWindow);
  virtual ~Cannon();

  float coolDownWindow;
  bool hasAmmo;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public f::ComObj {
private:
  NO__CPYASS(Explosion)
  Explosion();
public:

  virtual const a::COMType TypeId() { return "n/Explosion"; }

  //virtual void Inflate(not_null<c::Dictionary*> );
  virtual void Inflate(float x, float y);

  virtual ~Explosion();
  Explosion(not_null<c::Sprite*> );

  float frameTime;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Looper : public a::Component {
private:
  NO__CPYASS(Looper);

public:

  virtual const a::COMType TypeId() { return "n/Looper"; }

  virtual ~Looper();
  Looper();

  c::DelayTime* timer0;
  c::DelayTime* timer1;

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
private:
  NO__CPYASS(Missile)
  Missile();
public:

  virtual const a::COMType TypeId() { return "n/Missile"; }

  Missile(not_null<c::Sprite*>);
  virtual ~Missile();

  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public a::Component {
private:
  NO__CPYASS(Motion)
public:

  virtual const a::COMType TypeId() { return "n/Motion"; }
  virtual ~Motion();
  Motion();

  bool right;
  bool left;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public f::ComObj {
private:
  NO__CPYASS(Ship)
  Ship();
public:

  virtual const a::COMType TypeId() { return "n/Ship"; }

  Ship(not_null<c::Sprite*>, const stdstr&, const stdstr&);
  virtual ~Ship();

  stdstr frame0;
  stdstr frame1;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Velocity : public a::Component {
private:
  NO__CPYASS(Velocity)
  Velocity();
public:

  virtual const a::COMType TypeId() { return "n/Velocity"; }

  Velocity(float vx, float vy);
  virtual ~Velocity();

  float x;
  float y;
};



NS_END(invaders)
#endif

