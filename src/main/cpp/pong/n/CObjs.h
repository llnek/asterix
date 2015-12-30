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
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::ComObj {
  virtual const a::COMType typeId() { return "n/Ball"; }
  Ball(c::Sprite *s, float v)
    : ComObj(s) {
    speed=v;
  }
  DECL_FZ(speed)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {
  virtual const a::COMType typeId() { return "n/Motion"; }
  DECL_BF(right )
  DECL_BF(left )
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Paddle : public f::ComObj {
private:

  const s_arr<KEYCODE,2> p1Keys() {
    return cx::isPortrait()
      ? s_arr<KEYCODE,2> { KEYCODE::KEY_LEFT_ARROW , KEYCODE::KEY_RIGHT_ARROW}
      : s_arr<KEYCODE,2> {KEYCODE::KEY_DOWN_ARROW,KEYCODE::KEY_UP_ARROW };
  }

  const s_arr<KEYCODE,2> p2Keys() {
    return cx::isPortrait()
      ? s_arr<KEYCODE,2> {KEYCODE::KEY_A , KEYCODE::KEY_D }
      : s_arr<KEYCODE,2> {KEYCODE::KEY_S, KEYCODE::KEY_W  };
  }

  void onColor(const s_arr<KEYCODE,2> &cs, const sstr &snd) {
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

  s_arr<KEYCODE,2> kcodes;
  DECL_FZ(speed)
  DECL_IZ(pnum)
  DECL_TD(sstr, snd)

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

  Player() {}

  virtual const a::COMType typeId() { return "n/Player"; }

  void setName(const sstr &id, const sstr &name) {
    this->pname=name;
    this->pid=id;
  }

  DECL_IZ(category)
  DECL_IZ(pnum)
  DECL_IZ(value)
  DECL_TD(sstr, color)
  DECL_TD(sstr, pid)
  DECL_TD(sstr, pname)
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

  DECL_IZ(lastDir )
  DECL_FZ(lastP)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Slots : public a::Component {
  DECL_BF(poked)
  DECL_IZ(pnum )
  DECL_TD(c::Vec2, p2p)
  DECL_TD(c::Vec2, p1p)
  DECL_TD(c::Size, pz)
  DECL_TD(c::Size, bz)
  DECL_TD(c::Vec2, bp)
  virtual const a::COMType typeId() { return "n/Slots"; }
};



NS_END(pong)
#endif


