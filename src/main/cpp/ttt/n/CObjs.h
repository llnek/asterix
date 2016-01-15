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
struct CC_DLL Robot : public a::Component {

  MDECL_COMP_TPID( "n/Robot" )
  DECL_PTR(TTToe,board)

  Robot(not_null<TTToe*> b) {
    this->board= b;
  }

  virtual ~Robot() {
    mc_del_ptr(board)
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Grid : public a::Component {

  Grid(const ArrCells &seed) {
    s::copy(s::begin(seed),
        s::end(seed), s::begin(values));
    GOALS= mapGoalSpace();
  }

  MDECL_COMP_TPID("n/Grid")
  DECL_TD(ArrCells, values)
  // winning combo(s)
  s_vec<ArrDim> GOALS;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PlayView : public a::Component {

  PlayView(not_null<f::XNode*> node) {
    this->layer= node;
  }

  DECL_PTR(f::XNode, layer)
  MDECL_COMP_TPID( "n/PlayView")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public a::Component {

  MDECL_COMP_TPID( "n/Player")

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

  Player() {}

  DECL_TV(int, category, f::GMode::NICHTS)
  DECL_TV(int, pnum,  -1)
  DECL_IZ(value)
  DECL_TD(sstr, pidlong)
  DECL_TD(sstr, pid)
  DECL_TD(sstr, color)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public a::Component {
  MDECL_COMP_TPID("n/Players")
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CellSquare  : public f::ComObj {
  CellSquare(not_null<c::Sprite*> s, int cell, int value)
  : ComObj(s) {
    this->value=value;
    this->cell=cell;
  }
  DECL_IZ(value)
  DECL_IZ(cell)
  MDECL_COMP_TPID( "n/CellSquare" )
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



