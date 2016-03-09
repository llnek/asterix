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
//
#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "TTToe.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// stores the grid values and the set of winning goals
struct CC_DLL Grid : public ecs::Component {
  Grid(const ArrCells &seed) {
    GOALS= mapGoalSpace();
    S__COPY(seed,vals);
  }
  MDECL_COMP_TPID("n/Grid")
  DECL_TD(ArrCells, vals)
  s_vec<ArrDim> GOALS;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::CPlayer {
  Player(int category, int value, int id)
    : CPlayer(value) {
    this->category= category;
    this->pnum=id;
  }
  Player& operator=(const Player &other) {
    category= other.category;
    pidlong= other.pidlong;
    pnum= other.pnum;
    pid = other.pid;
    CPlayer::operator=(other);
    return *this;
  }
  Player(const Player &other)
    : CPlayer(other){

    category= other.category;
    pidlong= other.pidlong;
    pid = other.pid;
    pnum= other.pnum;
  }
  Player(int pnum) { this->pnum= pnum; }
  Player() {}
  DECL_TV(int, category, 0)
  DECL_TV(int, pnum,  -1)
  DECL_TD(sstr, pidlong)
  DECL_TD(sstr, pid)
  DECL_TD(sstr, color)
  MDECL_COMP_TPID( "n/Player")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public ecs::Component {
  MDECL_COMP_TPID("n/Players")
  //~owner
  s_arr<Player*,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CSquare : public f::CDraw {
  CSquare(int cell)
    : CDraw( cx::reifySprite("z.png")) {
    this->cell=cell;
    this->png= "z";
  }
  void toggle(int nv) {
    auto x= CC_CSV(c::Integer,"CV_X");
    auto o= CC_CSV(c::Integer,"CV_O");
    if (value == 0) {
      if (nv == x) { png= "x"; }
      if (nv == o) { png= "o"; }
      value=nv;
        SCAST(c::Sprite*,node)->setSpriteFrame(png + ".png");
    }
  }
  void flip() {
    SCAST(c::Sprite*,node)->setSpriteFrame(png + ".i.png");
  }
  DECL_TD(sstr,png)
  DECL_IZ(value)
  DECL_IZ(cell)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CSquares  : public ecs::Component {
  MDECL_COMP_TPID( "n/CSquares" )
  s_arr<f::Box4,GD_SZ> boxes;
  s_arr<CSquare*,GD_SZ> sqs;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CellPos  : public ecs::Component {
  MDECL_COMP_TPID( "n/CellPos" )
  DECL_TV(int, cell,  -1)
  DECL_TV(int, px, -1)
  DECL_TV(int, py, -1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )
  DECL_IZ(pnum)
  DECL_IZ(lastWinner)
};


NS_END



