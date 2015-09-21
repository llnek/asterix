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

  NO__COPYASSIGN(Alien)
  Alien();

public:

  Alien(c::Sprite*, int value, int rank);
  virtual ~Alien();
  virtual const a::COMType TypeId() { return "n/Alien"; }
  int rank;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public a::Component {
private:
  NO__COPYASSIGN(AlienSquad)
  AlienSquad();
public:

  virtual const a::COMType TypeId() { return "n/AlienSquad"; }
  AlienSquad(f::XPool* aliens, int step);
  virtual ~AlienSquad();
  const s::vector<f::ComObj*>& Elements() { return aliens->Elements(); }
  int Size() { return aliens->Size(); }

  f::XPool* aliens;
  int stepx;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
private:
  NO__COPYASSIGN(Bomb)
  Bomb();
public:

  virtual const a::COMType TypeId() { return "n/Bomb"; }

  Bomb(c::Sprite*);
  virtual ~Bomb();

  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public a::Component {
private:
  NO__COPYASSIGN(Cannon)
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
  NO__COPYASSIGN(Explosion)
  Explosion();
public:

  virtual const a::COMType TypeId() { return "n/Explosion"; }

  virtual void Inflate(c::Dictionary* );
  virtual ~Explosion();
  Explosion(c::Sprite* );

  float frameTime;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Looper : public a::Component {
private:
  NO__COPYASSIGN(Looper);
  Looper();
public:

  virtual const a::COMType TypeId() { return "n/Looper"; }

  Looper(int count);
  virtual ~Looper();

  c::DelayTime* timer0;
  c::DelayTime* timer1;

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
private:
  NO__COPYASSIGN(Missile)
  Missile();
public:

  virtual const a::COMType TypeId() { return "n/Missile"; }

  Missile(c::Sprite*);
  virtual ~Missile();
  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public a::Component {
private:
  NO__COPYASSIGN(Motion)
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
  NO__COPYASSIGN(Ship)
  Ship();
public:

  virtual const a::COMType TypeId() { return "n/Ship"; }

  Ship(c::Sprite*, const stdstr&, const stdstr&);
  virtual ~Ship();
  stdstr frame0;
  stdstr frame1;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Velocity : public a::Component {
private:
  NO__COPYASSIGN(Velocity)
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

