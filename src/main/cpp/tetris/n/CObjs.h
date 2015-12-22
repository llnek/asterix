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

//typedef int DIM4x4[4][4];
//typedef int DIM3x3[3][3];
//typedef int DIM2x2[2][2];

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BModel {
protected:
  int dim=0;
public:
  virtual bool test(int rID, int row, int col) = 0;
  virtual int size() = 0;
  virtual ~BModel() {}
  int square() { return dim * dim; }
  int getDim() { return dim; }
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

  virtual ~Brick() {}
  Brick() {}

  sstr frame1= "0.png";
  sstr frame0;
  c::Vec2 startPos;
};

typedef fusii::FPtrs<Brick> FArrBrick;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeInfo {
  ShapeInfo(BModel *m, int rot, const sstr &p) {
    this->rot=rot;
    model=m;
    png=p;
  }
  ShapeInfo() {}
  BModel *model = nullptr;
  int rot= 0;
  sstr png;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  Shape(const ShapeInfo& si) {
    info=si;
  }
  s_vec<Brick*> bricks;
  float x= 0.0f;
  float y=0.0f;
  ShapeInfo info;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell : public a::Component {
  virtual const a::COMType typeId() { return "n/ShapeShell"; }
  Shape *shape;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad : public a::Component {
  virtual const a::COMType typeId() { return "n/CtrlPad"; }
  s::map<sstr,f::Box4> hotspots;
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridBox : public a::Component {
  virtual const a::COMType typeId() { return "n/GridBox"; }
  f::Box4 box;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BlockGrid  : public a::Component {
  virtual const a::COMType typeId() { return "n/BlockGrid"; }
  s_vec<FArrBrick> grid;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxModel : public BModel {
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

public:

  virtual int size() { return 4; }

  virtual ~BoxModel() {}

  BoxModel() { dim=2; }

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElModel : public BModel {
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

public:

  virtual int size() { return 4; }

  virtual ~ElModel() {}

  ElModel() { dim=3; }

  virtual bool test(int rID, int row, int col) {
     return layout[rID * square() + row * dim + col] == 1;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElxModel : public BModel {
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
public:
  virtual int size() { return 4; }

  virtual ~ElxModel() {}

  ElxModel() { dim=3; }

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL LineModel : public BModel {
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

public:

  virtual int size() { return 4; }
  virtual ~LineModel() {}
  LineModel() { dim=4; }

  virtual bool test(int rID, int row, int col) {
    return layout[rID*square() + row * dim + col] == 1;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NubModel : public BModel {
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

public:

  virtual int size() { return 4; }

  virtual ~NubModel() {}
  NubModel() { dim=3; }

  virtual bool test(int rID, int row, int col) {
     return layout[rID * square() + dim * row + col] == 1 ;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StModel : public BModel {
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

public:

  virtual int size() { return 4; }

  virtual ~StModel() {}
  StModel() { dim=3; }

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + row * dim + col] == 1;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StxModel : public BModel {
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

public:

  virtual int size() { return 4; }

  virtual ~StxModel() {}
  StxModel() { dim=3; }

  virtual bool test(int rID, int row, int col) {
    return layout[rID * square() + dim * row + col] == 1;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Dropper : public a::Component {
  virtual const a::COMType typeId() { return "n/Dropper"; }
  float dropSpeed = CC_CSV(c::Float, "DROPSPEED");
  float dropRate= 80 + 700.0f/1.0f ;
  c::DelayTime *timer=nullptr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FilledLines : public a::Component {
  virtual const a::COMType typeId() { return "n/FilledLines"; }
  s_vec<int> lines;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {
  virtual const a::COMType typeId() { return "n/Motion"; }
  bool right=false;
  bool left=false;
  bool rotr= false;
  bool rotl= false;
  bool down=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Pauser  : public a::Component {
  virtual const a::COMType typeId() { return "n/Pauser"; }
  c::DelayTime *timer=nullptr;
  bool pauseToClear=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TileGrid  : public a::Component {
  virtual const a::COMType typeId() { return "n/TileGrid"; }
  s_vec<f::FArrInt> tiles;
};


NS_END(tetris)
#endif

