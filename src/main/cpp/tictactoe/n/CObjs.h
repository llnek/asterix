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

#include "nlohmann/json.hpp"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "ash/Ash.h"
#include "s/utils.h"
#include "Board.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_ALIAS(j, nlohmann)
NS_ALIAS(f, fusii)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SmartAlgo : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/SmartAlgo"; }

  SmartAlgo(not_null<Board*> b) {
    this->board= b;
  }

  virtual ~SmartAlgo() {}

  NOCPYASS(SmartAlgo)
  SmartAlgo() = delete;

  Board* board;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Grid : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Grid"; }

  Grid(const ArrCells& seed) {
    s::copy(s::begin(seed),
        s::end(seed), s::begin(values));
    this->GOALS= mapGoalSpace();
  }

  virtual ~Grid() {}

  NOCPYASS(Grid);
  Grid() = delete;

  s::vector<ArrDim> GOALS;
  ArrCells values;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayView : public a::Component {
public:
  virtual const a::COMType typeId() { return "n/PlayView"; }

  PlayView(not_null<f::XLayer*> layer) {
    this->size = cx::calcSize("z.png");
      this->boxes= mapGridPos(1.0f);
    this->cells.fill(nullptr);
    this->layer= layer;
  }

  virtual ~PlayView() {}

  NOCPYASS(PlayView)
  PlayView() = delete;

  s::array<c::Sprite*, GD_SZ> cells;
  s::array<f::Box4, GD_SZ> boxes;
  f::XLayer* layer;
  c::Size size;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Player"; }

    Player(int category, int value,
      int id,
      const stdstr& color) {

    this->offset = id == 1 ? 0 : 1;
    this->category= category;
    this->color= color;
    this->pnum=id;
    this->value= value;
  }

  virtual ~Player() {}

  Player& operator=(const Player&  other) {
    category = other.category;
    pidlong= other.pidlong;
    pid = other.pid;
    color = other.color;
    pnum= other.pnum;
    value= other.value;
    offset= other.offset;
    return *this;
  }

  Player(const Player& other) {
    category = other.category;
    pidlong= other.pidlong;
    pid = other.pid;
    color = other.color;
    pnum= other.pnum;
    value= other.value;
    offset= other.offset;
  }

  Player() {
    category= (int) f::GMode::NICHTS;
    pnum= -1;
    offset=0;
    value=0;
  }

  stdstr pidlong;
  stdstr pid;
  stdstr color;
  int category;
  int pnum;
  int value;
  int offset;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Players : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Players"; }

  virtual ~Players() {}

  NOCPYASS(Players)

  Players() {
  }

  s::array<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UISelection : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/UISelection"; }

  UISelection() {
    this->cell = -1;
    this->px = -1;
    this->py = -1;
  }

  virtual ~UISelection() {}

  NOCPYASS(UISelection)

  int cell;
  int px;
  int py;
};



NS_END(tttoe)
#endif



