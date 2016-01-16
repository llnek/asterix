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
#pragma once

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "core/JSON.h"
#include "s/utils.h"
#include "TTToe.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL SmartAI : public a::Component {
  MDECL_COMP_TPID( "n/SmartAI" )
  DECL_PTR(TTToe,ai)
  SmartAI(not_null<TTToe*> b) {
    this->ai= b;
  }
  virtual ~SmartAI() {
    mc_del_ptr(ai)
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Grid : public a::Component {
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
struct CC_DLL Player : public a::Component {
  Player(int category, int value, int id) {
    this->category= category;
    this->pnum=id;
    this->value= value;
  }
  Player& operator=(const Player &other) {
    category= other.category;
    pidlong= other.pidlong;
    pnum= other.pnum;
    value= other.value;
    pid = other.pid;
    return *this;
  }
  Player(const Player &other) {
    category= other.category;
    pidlong= other.pidlong;
    pid = other.pid;
    pnum= other.pnum;
    value= other.value;
  }
  Player(int pnum) { this->pnum= pnum; }
  Player() {}
  DECL_TV(int, category, f::GMode::NICHTS)
  DECL_TV(int, pnum,  -1)
  DECL_IZ(value)
  DECL_TD(sstr, pidlong)
  DECL_TD(sstr, pid)
  DECL_TD(sstr, color)
  MDECL_COMP_TPID( "n/Player")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public a::Component {
  MDECL_COMP_TPID("n/Players")
  //~owner
  s_arr<Player*,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CSquare  {
  CSquare(int cell) {
    this->value = CC_CSV(c::Integer,"CV_Z");
    this->cell=cell;
    this->png= "z";
    sprite= cx::reifySprite(png+ ".png");
  }
  void toggle(int nv) {
    auto x= CC_CSV(c::Integer,"CV_X");
    auto o= CC_CSV(c::Integer,"CV_O");
    auto z= CC_CSV(c::Integer,"CV_Z");
    if (value == z) {
      if (nv == x) { png= "x"; }
      if (nv == o) { png= "o"; }
      value=nv;
      sprite->setSpriteFrame(png + ".png");
    }
  }
  void flip() {
    sprite->setSpriteFrame(png + "_i.png");
  }
  DECL_PTR(c::Sprite,sprite)
  DECL_IZ(value)
  DECL_IZ(cell)
  DECL_TD(sstr,png)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CSquares  : public a::Component {
  MDECL_COMP_TPID( "n/CSquares" )
  s_arr<CSquare*,GD_SZ> sqs;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CellPos  : public a::Component {
  MDECL_COMP_TPID( "n/CellPos" )
  DECL_TV(int, cell,  -1)
  DECL_TV(int, px, -1)
  DECL_TV(int, py, -1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID( "n/GVars" )
  DECL_IZ(pnum)
  DECL_IZ(lastWinner)
};


NS_END(tttoe)



