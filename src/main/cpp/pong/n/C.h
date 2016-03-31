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

#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::CPixie {
  Ball(not_null<c::Node*> s)
    : CPixie(s) {
  }
  __decl_comp_tpid( "n/Ball")
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Paddle : public f::CPixie {

  const s_arr<KEYCODE,2> p2Keys() {
    return s_arr<KEYCODE,2> {KEYCODE::KEY_A , KEYCODE::KEY_D };
  }

  const s_arr<KEYCODE,2> p1Keys() {
    return s_arr<KEYCODE,2> {
      KEYCODE::KEY_LEFT_ARROW ,
      KEYCODE::KEY_RIGHT_ARROW };
  }

public:

  Paddle(not_null<c::Node*> s, int pnum)
    : CPixie(s) {
    this->kcodes = pnum == 1 ? p1Keys() : p2Keys();
    this->snd = pnum == 1 ? "x_hit" : "o_hit";
    this->pnum= pnum;
  }

  __decl_comp_tpid( "n/Paddle")
  s_arr<KEYCODE,2> kcodes;
  __decl_iz(pnum)
  __decl_md(sstr, snd)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::CStats {

  Player(int category, int value, int pnum, const sstr &color)
    : CStats(value) {
    this->category= category;
    this->color= color;
    this->pnum=pnum;
  }

  void setName(const sstr &id, const sstr &name) {
    this->pname=name;
    this->pid=id;
  }

  Player() {}

  __decl_comp_tpid( "n/Player")
  __decl_iz(category)
  __decl_iz(pnum)
  __decl_md(sstr, color)
  __decl_md(sstr, pid)
  __decl_md(sstr, pname)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public ecs::Component {
  __decl_comp_tpid( "n/Players")
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Faux : public f::CAutoma {
  __decl_comp_tpid("n/Faux")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Position : public ecs::Component {

  __decl_comp_tpid( "n/Position")

  Position(float lp) {
    lastP= lp;
  }

  __decl_iz(lastDir )
  __decl_fz(lastP)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_md(c::Vec2, p2p)
  __decl_md(c::Vec2, p1p)
  __decl_md(c::Size, pz)
  __decl_md(c::Size, bz)
  __decl_md(c::Vec2, bp)
  __decl_bf(poked)
  __decl_iz(pnum )
  __decl_comp_tpid( "n/GVars")
};


NS_END


