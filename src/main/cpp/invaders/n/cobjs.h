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

#include "2d/ComObj.h"
NS_ALIAS(c, cocos2d)
NS_ALIAS(f, fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public f::ComObj {
private:

  NO__COPYASSIGN(Alien)
  Alien();

  int rank;

public:

  Alien(c::Sprite*, int value, int rank);
  virtual ~Alien();

};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public a::Component {
private:
  NO__COPYASSIGN(AlienSquad)
  AlienSquad();
public:

  AlienSquad(const s::vector<Alien>& aliens, int step);
  virtual ~AlienSquad();
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
private:
  NO__COPYASSIGN(Bomb)
  Bomb();
  float x;
  float y;
public:

  Bomb(c::Sprite*);
  virtual ~Bomb();

};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public a::Component {
private:
  NO__COPYASSIGN(Cannon)
  Cannon();
  float coolDownWindow;
  bool hasAmmo;
public:

  Cannon(float coolDownWindow);
  virtual ~Cannon();

};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public f::ComObj {
private:
  NO__COPYASSIGN(Explosion)
  Explosion();
public:

  virtual void Inflate(c::Dictionary* );
  virtual ~Explosion();
  Explosion(c::Sprite* );

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Looper : public a::Component {
private:
  NO__COPYASSIGNDFT(Looper);
  Looper();
public:

  Looper(int count);
  virtual ~Looper();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
private:
  NO__COPYASSIGN(Missile)
  Missile();
  float x;
  float y;
public:
  Missile(c::Sprite*);
  virtual ~Missile();
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public a::Component {
private:
  NO__COPYASSIGN(Motion)
  bool right;
  bool left;
public:
  virtual ~Motion();
  Motion();
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public f::ComObj {
private:
  NO__COPYASSIGN(Ship)
  Ship();
public:
  Ship(c::Sprite*, const stdstr&, const stdstr&);
  virtual ~Ship();
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Velocity : public a::Component {
private:
  NO__COPYASSIGNDFT(Velocity)
  Velocity();
  float x;
  float y;
public:
  Velocity(float vx, float vy);
  virtual ~Velocity();
};



NS_END(invaders)


