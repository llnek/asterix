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
NS_ALIAS(f, fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public f::ComObj {
private:

  DISALLOW_COPYASSIGN(Alien)
  Alien();

  int rank;

public:

  Alien(cc::Sprite*, float value, int rank);
  virtual ~Alien();

};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public a::Component {
public:

  AlienSquad(const vector<Alien>& aliens, int step);
  virtual ~AlienSquad();

private:
  DISALLOW_COPYASSIGNDFT(AlienSquad)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public ComObj {
public:

  virtual ~Bomb();
  Bomb(Sprite*);

private:
  DISALLOW_COPYASSIGNDFT(Bomb)
  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Cannon : public Component {
public:

  Cannon(float coolDownWindow);
  virtual ~Cannon();

private:

  DISALLOW_COPYASSIGN(Cannon)
  float coolDownWindow;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public ComObj {
public:

  virtual ~Explosion();
  Explosion(Sprite* );

  virtual void Inflate(Dictionary* dict) override;

private:
  DISALLOW_COPYASSIGN(Explosion)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Looper : public Component {
public:

  virtual ~Looper();
  Looper(int count);

private:
  DISALLOW_COPYASSIGNDFT(Looper);
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public ComObj {
public:
  virtual ~Missile();
  Missile(Sprite*);
private:
  DISALLOW_COPYASSIGNDFT(Missile)
  float x;
  float y;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public Component {
public:
  virtual ~Motion();
  Motion();
private:
  DISALLOW_COPYASSIGN(Motion)
  bool right;
  bool left;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public ComObj {
public:
  virtual ~Ship();
  Ship(Sprite*,frames);
private:
  DISALLOW_COPYASSIGNDFT(Ship)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Velocity : public Component {
public:
  virtual ~Velocity();
  Velocity(float vx, float vy);
private:
  DISALLOW_COPYASSIGNDFT(Velocity)
  float x;
  float y;
};



NS_END(invaders)


