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

#include "core/ComObj.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::ComObj {
  virtual const a::COMType typeId() { return "n/Ball"; }
  Ball(c::Sprite *s, float v)
    : ComObj(s) {
    speed=v;
  }
  float speed=0;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {
  virtual const a::COMType typeId() { return "n/Motion"; }
  bool right = false;
  bool left = false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Paddle : public f::ComObj {
private:

  const s_arr<int,2> p1Keys() {
    return cx::isPortrait()
      ? s_arr<int,2> {KEYCODE::KEY_LEFT_ARROW , KEYCODE::KEY_RIGHT_ALLOW}
      : s_arr<int,2> {KEYCODE::KEY_DOWN_ARROW,,KEYCODE::KEY_UP_ARROW };
  }

  const s_arr<int,2> p2Keys() {
    return cx::isPortrait()
      ? s_arr<int,2> {KEYCODE::KEY_A , KEYCODE::KEY_D }
      : s_arr<int,2> {KEYCODE::KEY_S, KEYCODE::KEY_W  };
  }

  void onColor(const s_arr<int,2> &cs, const sstr &snd) {
    s::copy(cs.begin(), cs.end(), this->kcodes.begin());
    this->snd= snd;
  }

public:

  Paddle(c::Sprite *s, int pnum, float v) : ComObj(s) {

    if (pnum == 1) {
      onColor(p1Keys(), "x_hit" );
    } else {
      onColor(p2Keys(), "o_hit");
    }

    this->pnum= pnum;
    this->speed=v;
  }

  virtual const a::COMType typeId() { return "n/Paddle"; }

  s_arr<int,2> kcodes;
  float speed=0;
  int pnum=0;
  sstr snd;

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public a::Component {

  Player(int category, int value, int pnum, const sstr &color) {
    this->category= category;
    this->color= color;
    this->pnum=pnum;
    this->value= value;
  }

  virtual const a::COMType typeId() { return "n/Player"; }

  setName(const sstr &id, const sstr &name) {
    this->pname=name;
    this->pid=id;
  }

  int category=0;
  int pnum=0;
  int value=0;
  sstr color;
  sstr pid
  sstr pname;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public a::Component {
  virtual const a::COMType typeId() { return "n/Players"; }
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Faux : public a::Component {
  virtual const a::COMType typeId() { return "n/Faux"; }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Position : public a::Component {

  virtual const a::COMType typeId() { return "n/Position"; }

  Position(float lp) {
    lastP= lp;
  }

  int lastDir = 0;
  float lastP= 0;

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Slots : public a::Component {
  bool poked=false;
  int pnum = 0;
  c::Vec2 p2p;
  c::Vec2 p1p;
  c::Size pz;
  c::Size bz;
  c::Vec2 bp;
  virtual const a::COMType typeId() { return "n/Slots"; }
}



NS_END(pong)
#endif


