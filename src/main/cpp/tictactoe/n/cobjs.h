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

#include "dbox/json11.hpp"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "ash/Ash.h"
#include "s/utils.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(j, json11)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SmartAlgo : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/SmartAlgo"; }

  SmartAlgo(not_null<TTTBoard*> b) {
    this->board= b;
  }

  virtual ~SmartAlgo() {}

  NO__CPYASS(SmartAlgo)
  SmartAlgo() = delete;

  TTTBoard* board;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Grid : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Grid"; }

  Grid(const ArrCells& seed, int sz = BD_SZ) {
    s::copy(s::begin(seed),
        s::end(seed), s::begin(values));
    this->GOALS= mapGoalSpace();
  }

  virtual ~Grid() {}

  NO__CPYASS(Grid);
  Grid() = delete;

  s::vector<ArrDim> GOALS;
  ArrCells values;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayView : public a::Component {

  virtual const a::COMType typeId() { return "n/PlayView"; }

  PlayView(not_null<f::XLayer*> layer) {
    auto sp = cx::reifySprite("z.png");
    auto sz= sp->getContentSize();
    this->layer= layer;
    this->size = sz;
    this->boxes= mapGridPos(1.0f);
    this->cells.fill(nullptr);
  }

  virtual ~PlayView() {}

  NO__CPYASS(PlayView)
  PlayView() = delete;

  s::array<f::Box4, GD_SZ> boxes;
  s::array<c::Sprite*, GD_SZ> cells;
  f::XLayer* layer;
  c::Size size;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL NetPlay : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/NetPlay"; }

  NetPlay(not_null<ws::OdinIO*> io) { odin = io; }

  virtual ~NetPlay() {}
  NO__CPYASS(NetPlay)

  ws::OdinIO* odin;
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

  NO__CPYASS(Player)
  Player() = delete;

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

  NO__CPYASS(Players)

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

  NO__CPYASS(UISelection)

  int cell;
  int px;
  int py;
};



NS_END(tttoe)
#endif



