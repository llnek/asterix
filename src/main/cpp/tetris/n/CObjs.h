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

#include "core/XConfig.h"
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BModel {
  virtual bool test(int rID, int row, int col) = 0;
  int square() { return dim * dim; }
  int getDim() { return dim; }
  virtual int size() = 0;
  virtual ~BModel() {}
protected:
  DECL_IZ(dim)
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick : public c::Sprite {

  static Brick* reify(const c::Vec2 &pos,
      const sstr &f0) {
    Brick *b = mc_new(Brick);
    if (NNP(b) &&
        b->init()) {
      b->setAnchorPoint(cx::anchorTL());
      b->setPosition(pos);
      b->startPos = pos;
      b->frame0=f0;
      b->show();
      b->autorelease();
      return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
  }

  void blink() {
    setSpriteFrame( cx::getSpriteFrame(frame1));
  }

  void show() {
    setSpriteFrame( cx::getSpriteFrame(frame0));
  }

  void dispose() {
    removeFromParent();
  }

  DECL_TV(sstr, frame1, "0.png")
  DECL_TD(c::Vec2, startPos)
  DECL_TD(sstr, frame0)
};

typedef fusii::FArrayPtr<Brick> FArrBrick;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeInfo {
  ShapeInfo(BModel *m, int rot, const sstr &p) {
    this->rot=rot;
    model=m;
    png=p;
  }
  DECL_PTR(BModel, model)
  DECL_TD(sstr, png)
  DECL_IZ(rot)
  ShapeInfo() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  Shape(const ShapeInfo& si) {
    info=si;
  }
  DECL_TD(ShapeInfo , info)
  s_vec<Brick*> bricks;
  DECL_FZ(x)
  DECL_FZ(y)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell : public a::Component {
  MDECL_COMP_TPID( "n/ShapeShell")
  DECL_PTR(Shape, shape)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad : public a::Component {
  MDECL_COMP_TPID( "n/CtrlPad")
  s::map<sstr,f::Box4> hotspots;
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridBox : public a::Component {
  MDECL_COMP_TPID( "n/GridBox")
  DECL_TD(f::Box4, box)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BlockGrid  : public a::Component {
  MDECL_COMP_TPID( "n/BlockGrid")
  s_vec<FArrBrick> grid;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxModel : public BModel {

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }
  virtual int size() { return 4; }
  BoxModel() { dim=2; }

private:
  s_arr<int, 2*2*4> layout {
     1,1,
     1,1,

     1,1,
     1,1,

     1,1,
     1,1,

     1,1,
     1,1
  };
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElModel : public BModel {

  virtual bool test(int rID, int row, int col) {
     return layout[rID * square() + row * dim + col] == 1;
  }
  virtual int size() { return 4; }
  ElModel() { dim=3; }

private:
  s_arr<int, 4 * 3 * 3> layout {
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
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElxModel : public BModel {

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }
  virtual int size() { return 4; }
  ElxModel() { dim=3; }

private:
  s_arr<int, 4*3*3> layout {
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
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL LineModel : public BModel {

  virtual bool test(int rID, int row, int col) {
    return layout[rID*square() + row * dim + col] == 1;
  }
  virtual int size() { return 4; }
  LineModel() { dim=4; }

private:
  s_arr<int, 4*4*4> layout {
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
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NubModel : public BModel {

  virtual bool test(int rID, int row, int col) {
     return layout[rID * square() + dim * row + col] == 1 ;
  }
  virtual int size() { return 4; }
  NubModel() { dim=3; }

private:
  s_arr<int, 4 * 3 * 3> layout {
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
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StModel : public BModel {

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }
  virtual int size() { return 4; }
  StModel() { dim=3; }

private:
  s_arr<int,4*3*3> layout {
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
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StxModel : public BModel {

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + dim * row + col] == 1;
  }
  virtual int size() { return 4; }
  StxModel() { dim=3; }

private:
  s_arr<int,4*3*3> layout {
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
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Dropper : public a::Component {
  MDECL_COMP_TPID( "n/Dropper")
  DECL_FZ(dropSpeed)
  DECL_FZ(dropRate)
  DECL_PTR(c::DelayTime, timer)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FilledLines : public a::Component {
  MDECL_COMP_TPID( "n/FilledLines")
  s_vec<int> lines;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {
  MDECL_COMP_TPID( "n/Motion")
  DECL_BF(right)
  DECL_BF(left)
  DECL_BF(rotr)
  DECL_BF(rotl)
  DECL_BF(down)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Pauser  : public a::Component {
  DECL_PTR(c::DelayTime, timer)
  MDECL_COMP_TPID("n/Pauser")
  DECL_BF(pauseToClear)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TileGrid  : public a::Component {
  MDECL_COMP_TPID("n/TileGrid")
  s_vec<f::FArrInt> tiles;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars  : public a::Component {
  MDECL_COMP_TPID("n/GVars")

};

NS_END(tetris)
#endif

