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

#include "core/XConfig.h"
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
// base class for different shape configurations
struct CC_DLL BModel {
  virtual bool test(int rID, int row, int col) = 0;
  int sq() { return _dim * _dim; }
  int dim() { return _dim; }
  virtual int size() = 0;
  virtual ~BModel() {}
protected:
  __decl_iz(_dim)
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick {

  static Brick* reify(const CCT_PT &pos, const sstr &f0) {
    auto b = mc_new1(Brick, f0);
    b->node->setAnchorPoint(cx::anchorTL());
    b->node->setPosition(pos);
    b->startPos=pos;
    b->show();
    return b;
  }

  Brick(bool used) { this->colliable=used; }

  Brick(const sstr& f0) {
    node= c::Sprite::create();
    frame0=f0;
    colliable=true;
  }

  Brick() {}

  void blink() {
    if (node) node->setSpriteFrame( frame1);
  }

  void show() {
    if (node) node->setSpriteFrame( frame0);
  }

  void dispose() {
    if (node) node->removeFromParent();
    colliable=false;
  }

  __decl_mv(sstr, frame1, "0.png")
  __decl_md(sstr, frame0)

  __decl_md(CCT_PT, startPos)
  __decl_ptr(c::Sprite, node)
  __decl_bf(colliable)

};

// a row of bricks
typedef fusii::FArrayPtr<Brick> FArrBrick;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeInfo {
  ShapeInfo(BModel *m, int rot, const sstr &p) {
    this->rot=rot;
    model=m;
    png=p;
  }
  __decl_ptr(BModel, model)
  __decl_md(sstr, png)
  __decl_iz(rot)
  ShapeInfo() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape : public ecs::Component {
  Shape(const ShapeInfo &si) { info=si; }
  __decl_md(ShapeInfo, info)
  s_vec<Brick*> bricks;
  __decl_fz(x)
  __decl_fz(y)
  __decl_comp_tpid( "n/Shape")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell : public ecs::Component {
  __decl_comp_tpid( "n/ShapeShell")
  __decl_ptr(Shape, shape)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad : public ecs::Component {
  __decl_map(sstr,f::Box4, hotspots)
  __decl_comp_tpid( "n/CtrlPad")
  __decl_md(CCT_RT, bbox)
};

//////////////////////////////////////////////////////////////////////////////
// the game area as a grid
struct CC_DLL BlockGrid  : public ecs::Component {
  __decl_comp_tpid( "n/BlockGrid")
  s_vec<FArrBrick> grid;
};

//////////////////////////////////////////////////////////////////////////////
// piece = O
class CC_DLL BoxModel : public BModel {

  s_arr<int, 2*2*4> _layout {
     1,1,
     1,1,

     1,1,
     1,1,

     1,1,
     1,1,

     1,1,
     1,1
  };

public:
  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + row * _dim + col] == 1;
  }
  virtual int size() { return 4; }
  BoxModel() { _dim=2; }
};

//////////////////////////////////////////////////////////////////////////
// piece = L
struct CC_DLL ElModel : public BModel {

  s_arr<int, 3*3*4> _layout {
      0,1,0,
      0,1,0,
      0,1,1,

      0,0,0,
      1,1,1,
      1,0,0,

      1,1,0,
      0,1,0,
      0,1,0,

      0,0,1,
      1,1,1,
      0,0,0
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + row * _dim + col] == 1;
  }
  virtual int size() { return 4; }
  ElModel() { _dim=3; }

};

//////////////////////////////////////////////////////////////////////////
// piece J
struct CC_DLL ElxModel : public BModel {

  s_arr<int, 3*3*4> _layout {
      0,1,0,
      0,1,0,
      1,1,0,

      1,0,0,
      1,1,1,
      0,0,0,

      0,1,1,
      0,1,0,
      0,1,0,

      0,0,0,
      1,1,1,
      0,0,1
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + row * _dim + col] == 1;
  }
  virtual int size() { return 4; }
  ElxModel() { _dim=3; }

};

//////////////////////////////////////////////////////////////////////////
// piece I
struct CC_DLL LineModel : public BModel {

  s_arr<int, 4*4*4> _layout {
      0,0,0,0,
      1,1,1,1,
      0,0,0,0,
      0,0,0,0,

      0,0,1,0,
      0,0,1,0,
      0,0,1,0,
      0,0,1,0,

      0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0,

      0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID*sq() + row * _dim + col] == 1;
  }
  virtual int size() { return 4; }
  LineModel() { _dim=4; }
};

//////////////////////////////////////////////////////////////////////////
// piece T
struct CC_DLL NubModel : public BModel {

  s_arr<int, 3*3*4> _layout {
      0,0,0,
      0,1,0,
      1,1,1,

      1,0,0,
      1,1,0,
      1,0,0,

      1,1,1,
      0,1,0,
      0,0,0,

      0,0,1,
      0,1,1,
      0,0,1
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + _dim * row + col] == 1 ;
  }
  virtual int size() { return 4; }
  NubModel() { _dim=3; }

};

//////////////////////////////////////////////////////////////////////////
// piece S
struct CC_DLL StModel : public BModel {

  s_arr<int,3*3*4> _layout {
      0,1,0,
      0,1,1,
      0,0,1,

      0,0,0,
      0,1,1,
      1,1,0,

      1,0,0,
      1,1,0,
      0,1,0,

      0,1,1,
      1,1,0,
      0,0,0
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + row * _dim + col] == 1;
  }
  virtual int size() { return 4; }
  StModel() { _dim=3; }

};

//////////////////////////////////////////////////////////////////////////
// piece Z
struct CC_DLL StxModel : public BModel {

  s_arr<int,3*3*4> _layout {
      0,1,0,
      1,1,0,
      1,0,0,

      1,1,0,
      0,1,1,
      0,0,0,

      0,0,1,
      0,1,1,
      0,1,0,

      0,0,0,
      1,1,0,
      0,1,1
  };

public:

  virtual bool test(int rID, int row, int col) {
    return _layout[rID * sq() + _dim * row + col] == 1;
  }
  virtual int size() { return 4; }
  StxModel() { _dim=3; }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Dropper : public ecs::Component {
  __decl_comp_tpid( "n/Dropper")
  __decl_fz(dropSpeed)
  __decl_fz(dropRate)
  __decl_ptr(c::DelayTime, timer)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FilledLines : public ecs::Component {
  __decl_comp_tpid( "n/FilledLines")
  __decl_vec(int, lines)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public f::CGesture {
  void reset() {
    right=false;
    left=false;
    rotr=false;
    rotl=false;
    down=false;
  }
  __decl_bf(rotr)
  __decl_bf(rotl)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Pauser  : public ecs::Component {
  __decl_ptr(c::DelayTime, timer)
  __decl_comp_tpid("n/Pauser")
  __decl_bf(pauseToClear)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars  : public ecs::Component {
  __decl_comp_tpid("n/GVars")
  __decl_md(f::Box4,cbox)
  __decl_iz(hTiles)
};

//////////////////////////////////////////////////////////////////////////////
//
owner<Shape*> previewShape(const f::Box4&, const ShapeInfo&, float x, float y);

owner<Shape*> reifyShape(const f::Box4&, s_vec<FArrBrick>& ,
    float x, float y,
    const ShapeInfo&);

int topLine(not_null<ecs::Node*>);

void clearOldBricks(s_vec<Brick*>&);
void disposeShape(Shape*);

const s_vec<CCT_PT>
findBBox(const f::Box4&,s_vec<FArrBrick>&, BModel*,
    float px, float py, int rID, bool skipCollide = false);

bool maybeCollide(const f::Box4&, s_vec<FArrBrick>&, float tl_x, float tl_y );

const f::Cell2D xrefTile(const f::Box4&, float x, float y);
const f::Cell2D xrefTile(const f::Box4&, const CCT_PT&);

void initDropper(Dropper*);

void setDropper(not_null<c::Node*>, Dropper*, float r, float s);

void lock(const f::Box4&, not_null<ecs::Node*>, Shape*);

bool testFilledRow(s_vec<FArrBrick>&, int r);

void flashFilled(s_vec<FArrBrick>&,
    FilledLines*,
    const s_vec<int>& lines);

void pauseForClearance(not_null<ecs::Node*>, bool b, float delay);

bool moveDown(const f::Box4&, s_vec<FArrBrick>&, Shape*);

bool shiftRight(const f::Box4&, s_vec<FArrBrick>&, Shape*);
bool shiftLeft(const f::Box4&, s_vec<FArrBrick>&, Shape*);

bool rotateRight(const f::Box4&, s_vec<FArrBrick>&, Shape*);
bool rotateLeft(const f::Box4&, s_vec<FArrBrick>&, Shape*);



NS_END

