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

#include "core/CCSX.h"
#include "ash/Ash.h"
#include "s/utils.h"
#define BD_SZ 3

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(a,ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct ViewData {
  ViewData(c::Sprite* s, float x, float y, int v) {
    sprite=s;
    this->x=x;
    this->y=y;
    value=v;
  }
  c::Sprite* sprite;
  float x;
  float y;
  int value;
};

struct HUDUpdate {
  HUDUpdate(bool r, int p) { running=r; pnum=p; }
  bool running;
  int pnum;
};

struct ScoreUpdate {
  ScoreUpdate(const stdstr& c, int n) { color=c; score=n; }
  stdstr color;
  int score;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL SmartAlgo : public a::Component {
public:
  SmartAlgo(not_null<TTTBoard*> b) {
    this->board= b;
  }

  virtual const a::COMType TypeId() { return "n/SmartAlgo"; }
  virtual ~SmartAlgo() {}

  SmartAlgo() = delete;
  NO__CPYASS(SmartAlgo)

  TTTBoard* board;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Board : public a::Component {
public:

  Board() {
    this->GOALS= MapGoalSpace();
    this->size=BD_SZ;
  }

  virtual const a::COMType TypeId() { return "n/Board"; }
  virtual ~Board() {}

  NO__CPYASS(Board);
  Board() = delete;

  s::vector<s::array<int,BD_SZ>> GOALS;
  int size;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Grid : public a::Component {
public:

  Grid(const s::array<int,BD_SZ * BD_SZ>& seed) {
    s::copy(s::begin(seed), s::end(seed), s::begin(values));
  }

  virtual const a::COMType TypeId() { return "n/Grid"; }
  virtual ~Grid() {}

  NO__CPYASS(Grid);
  Grid() = delete;

  s::array<int, BD_SZ * BD_SZ> values;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL PlayView : public a::Component {

  PlayView(not_null<f::XLayer*> layer) {
    auto sp = cx::ReifySprite("z.png");
    auto sz= sp->getContentSize();
    this->layer= layer;
    this->size = sz;
    this->boxes= MapGridPos(1.0f);
  }

  virtual const a::COMType TypeId() { return "n/PlayView"; }
  virtual ~PlayView() {}

  NO__CPYASS(PlayView)
  PlayView() = delete;

  s::array<f::Box4,BD_SZ * BD_SZ> boxes;
  f::XLayer* layer;
  c::Size size;
  s::array<ViewData,BD_SZ * BD_SZ> cells;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL NetPlay : public a::Component {
public:

  NetPlay() { SNPTR(event) }

  virtual const a::COMType TypeId() { return "n/NetPlay"; }
  virtual ~NetPlay() {}
  NO__CPYASS(NetPlay)

  void* event;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Player : public a::Component {
public:

  Player(int category, int value, int id, const stdstr& color) {
    this->color= color;
    this->pnum=id;
    this->category= category;
    this->value= value;
    this->offset = id == 1 ? 0 : 1;
  }

  virtual const a::COMType TypeId() { return "n/Player"; }
  virtual ~Player() {}
  NO__CPYASS(Player)
  Player() = delete;

  stdstr color;
  int pnum;
  int category;
  int value;
  int offset;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Players : public a::Component {
public:

  Players() {}

  virtual const a::COMType TypeId() { return "n/Players"; }
  virtual ~Players() {}
  NO__CPYASS(Players)

  s::array<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL UISelection : public a::Component {
public:

  UISelection() {
    this->cell = -1;
    this->px = -1;
    this->py = -1;
  }

  virtual const a::COMType TypeId() { return "n/UISelection"; }
  virtual ~UISelection() {}
  NO__CPYASS(UISelection)

  int cell;
  int px;
  int py;
};



NS_END(tttoe)
#endif



