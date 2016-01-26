// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::ComObj {
  Ball(not_null<c::Sprite*> s, float v)
    : ComObj(s) {
    speed.x=v;
    speed.y=v;
  }
  MDECL_COMP_TPID( "n/Ball")
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Paddle : public f::ComObj {

  const s_arr<KEYCODE,2> p1Keys() {
    return s_arr<KEYCODE,2> {KEYCODE::KEY_LEFT_ARROW , KEYCODE::KEY_RIGHT_ARROW};
  }

  const s_arr<KEYCODE,2> p2Keys() {
    return s_arr<KEYCODE,2> {KEYCODE::KEY_A , KEYCODE::KEY_D };
  }

public:

  Paddle(not_null<c::Sprite*> s, int pnum, float v)
    : ComObj(s) {
    this->kcodes = pnum == 1 ? p1Keys() : p2Keys();
    this->snd = pnum == 1 ? "x_hit" : "o_hit";
    this->pnum= pnum;
    speed.x=v;
    speed.y=v;
  }

  MDECL_COMP_TPID( "n/Paddle")
  s_arr<KEYCODE,2> kcodes;
  DECL_IZ(pnum)
  DECL_TD(sstr, snd)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID( "n/Gesture")
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

  void setName(const sstr &id, const sstr &name) {
    this->pname=name;
    this->pid=id;
  }

  Player() {}

  MDECL_COMP_TPID( "n/Player")
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
  MDECL_COMP_TPID( "n/Players")
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Faux : public a::Component {
  MDECL_COMP_TPID("n/Faux")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Position : public a::Component {

  MDECL_COMP_TPID( "n/Position")

  Position(float lp) {
    lastP= lp;
  }

  DECL_IZ(lastDir )
  DECL_FZ(lastP)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  DECL_TD(c::Vec2, p2p)
  DECL_TD(c::Vec2, p1p)
  DECL_TD(c::Size, pz)
  DECL_TD(c::Size, bz)
  DECL_TD(c::Vec2, bp)
  DECL_BF(poked)
  DECL_IZ(pnum )
  MDECL_COMP_TPID( "n/GVars")
};


NS_END(pong)


