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

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(a,ash)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL SmartAlgo : public a::Component {
public:
  SmartAlgo(not_null<f::GameBoard*> b) {
    this->board= b;
  }

  virtual const COMType TypeId() { return "n/SmartAlgo"; }
  virtual ~SmartAlgo() {}

  SmartAlgo() = delete;
  NO__CPYASS(SmartAlgo)

  f::GameBoard* board;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Board : public a::Component {
public:

  Board(size_t size, const s::vector<f::FArray<int>>& goals) {
    this->GOALS= goals;
    this->size=size;
  }

  virtual const COMType TypeId() { return "n/Board"; }
  virtual ~Board() {}
  NO__CPYASS(Board);
  Board() = delete;

  s::vector<f::FArray<int>> GOALS;
  size_t size;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Grid : public a::Component {
public:

  Grid(size_t size, const f::FArray<int>& seed) {
    this->values.Clone(seed);
    this->size=size;
  }

  virtual const COMType TypeId() { return "n/Grid"; }
  virtual ~Grid() {}
  NO__CPYASS(Grid);
  Grid() = delete;

  f::FArray<int> values;
  size_t size;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL GridView : public a::Component {

  GridView(size_t size, not_null<c::Node*> layer) {
    auto sp = cx::CreateSprite("z.png");
    auto sz= sp->getContentSize();

    this->cells= sjs.makeArray(size * size, null);
    this->layer= layer;
    this->size = sz;
    this->url= "";

    this->gridMap= utils.mapGridPos();
  }

  virtual const COMType TypeId() { return "n/GridView"; }
  virtual ~GridView() {}
  NO__CPYASS(GridView)
  GridView() = delete;

  c::Node* layer;
  c::Size size;
  stdstr url;

};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL NetPlay : public a::Component {
public:

  NetPlay() { SNPTR(event) }

  virtual const COMType TypeId() { return "n/NetPlay"; }
  virtual ~NetPlay() {}
  NO__CPYASS(NetPlay)

  void* event;
};


//////////////////////////////////////////////////////////////////////////////
class CC_DLL Player : public a::Component {
public:

  Player(category,value,id,color) {
    this->color= color;
    this->pnum=id;
    this->category= category;
    this->value= value;
    this->offset = id == 1 ? 0 : 1;
  }

  virtual const COMType TypeId() { return "n/Player"; }
  virtual ~Player() {}
  NO__CPYASS(Player)
  Player() = delete;


};

//////////////////////////////////////////////////////////////////////////////
class CC_DLL UISelection : public a::Component {
public:

  UISelection() {
    this->cell = -1;
    this->px = -1;
    this->py = -1;
  }

  virtual const COMType TypeId() { return "n/UISelection"; }
  virtual ~UISelection() {}
  NO__CPYASS(UISelection)

  int cell;
  int px;
  int py;
};



NS_END(tttoe)
#endif



