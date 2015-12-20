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

NS_BEGIN(tetris)

typedef int DIM4x4[4][4];
typedef int DIM3x3[3][3];
typedef int DIM2x2[2][2];

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick : public c::Sprite {

  void blink() {
    this->setSpriteFrame( cx::getSpriteFrame(frame1));
  }

  void show() {
    this->setSpriteFrame( cx::getSpriteFrame(frame0));
  }

  void dispose() {
    this->setVisible(false);
    this->removeFromParent();
  }

  static Brick* reify(const c::Vec2 &pos, const sstr &f0) {
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

  virtual ~Brick() {}
  Brick() {}

  sstr frame1= "0.png";
  sstr frame0;
  c::Vec2 startPos;
};

typedef fusii::FArray<Brick> FArrBrick;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell {
  Shape *shape;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  Shape(BModel *m, int rot, const sstr &p) {
    this->rot=rot;
    model=m;
    png=p;
  }
  BModel *model = nullptr;
  int rot= 0;
  sstr png;
  float x= 0.0f;
  float y=0.0f;
  s_vec<Brick*> bricks;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad {
  s::map<sstr,f::Box4> hotspots;
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridBox {
  f::Box4 box;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BlockGrid {
  s_vec<Brick*> grid;
};

//////////////////////////////////////////////////////////////////////////
//
template<typename T> class CC_DLL BModel {
protected:

  s_vec<T> layout;
  int dim=0;

public:

  /*
  T& getLayout(int pos) {
    assert(pos >=0 && pos < layout.size());
    return layout[pos];
  }
  */
  bool test(int rID, int row, int col) = 0;
  int size() { return layout.size(); }
  int getDim() { return dim; }

  virtual ~BModel() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxModel : public BModel<DIM2x2> {

  virtual ~BoxModel() {}
  BoxModel() {
    DIM2x2 a1
    {1,1,
     1,1};
    DIM2x2 a2
    {1,1,
     1,1};
    DIM2x2 a3
    {1,1,
     1,1};
    DIM2x2 a4
    {1,1,
     1,1};

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=2;
  }
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElModel : public BModel<DIM3x3> {

  virtual ~ElModel() {}

  ElModel() {
    DIM3x3 a1
    { 0,1,0,
      0,1,0,
      0,1,1};
    DIM3x3 a2
    { 0,0,0,
      1,1,1,
      1,0,0 };
    DIM3x3 a3
    { 1,1,0,
      0,1,0,
      0,1,0 };
    DIM3x3 a4
    { 0,0,1,
      1,1,1,
      0,0,0 };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=3;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ElxModel : public BModel<DIM3x3> {

  virtual ~ElxModel() {}

  ElxModel() {

    DIM3x3 a1 {
      0,1,0,
      0,1,0,
      1,1,0
    };
    DIM3x3 a2 {
      1,0,0,
      1,1,1,
      0,0,0
    };
    DIM3x3 a3 {
      0,1,1,
      0,1,0,
      0,1,0
    };
    DIM3x3 a4 {
      0,0,0,
      1,1,1,
      0,0,1
    };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=3;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL LineModel : public BModel<DIM4x4> {

  virtual ~LineModel() {}
  LineModel() {

    DIM4x4 a1 {
      0,0,0,0,
      1,1,1,1,
      0,0,0,0,
      0,0,0,0
    };
    DIM4x4 a2 {
      0,0,1,0,
      0,0,1,0,
      0,0,1,0,
      0,0,1,0
    };
    DIM4x4 a3 {
      0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0
    };
    DIM4x4 a4 {
      0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0
    };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=4;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL NubModel : public BModel<DIM3x3> {

  virtual ~NubModel() {}
  NubModel() {

    DIM3x3 a1= {
      0,0,0,
      0,1,0,
      1,1,1
    };
    DIM3x3 a2 {
      1,0,0,
      1,1,0,
      1,0,0
    };
    DIM3x3 a3 {
      1,1,1,
      0,1,0,
      0,0,0
    };
    DIM3x3 a4 {
      0,0,1,
      0,1,1,
      0,0,1
    };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=3;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StModel : public BModel<DIM3x3> {

  virtual ~StModel() {}
  StModel() {
    DIM3x3 a1 {
      0,1,0,
      0,1,1,
      0,0,1
    };
    DIM3x3 a2 {
      0,0,0,
      0,1,1,
      1,1,0
    };
    DIM3x3 a3 {
      1,0,0,
      1,1,0,
      0,1,0
    };
    DIM3x3 a4 {
      0,1,1,
      1,1,0,
      0,0,0
    };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=3;
  }

};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL StxModel : public BModel<DIM3x3> {

  virtual ~StxModel() {}
  StxModel() {
    DIM3x3 a1{
      0,1,0,
      1,1,0,
      1,0,0
    };
    DIM3x3 a2 {
      1,1,0,
      0,1,1,
      0,0,0
    };
    DIM3x3 a3 {
      0,0,1,
      0,1,1,
      0,1,0
    };
    DIM3x3 a4 {
      0,0,0,
      1,1,0,
      0,1,1
    };

    layout.push_back(a1);
    layout.push_back(a2);
    layout.push_back(a3);
    layout.push_back(a4);

    dim=3;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Dropper {
  float dropSpeed = CC_CSV(c::Float, "DROPSPEED");
  float dropRate= 80 + 700.0f/1.0f ;
  c::DelayTime *timer=nullptr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL FilledLines {
  s_vec<int> lines;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion {
  bool right=false;
  bool left=false;
  bool rotr= false;
  bool rotl= false;
  bool down=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Pauser {
  c::DelayTime *timer=nullptr;
  bool pauseToClear=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TileGrid {
  s_vec<FArrBrick*> tiles;
};

//////////////////////////////////////////////////////////////////////////////
//
s_arr<BModel*,7> ListOfModels = {
  mc_new(LineModel),
  mc_new(BoxModel),
  mc_new(StModel),
  mc_new(ElModel),
  mc_new(NubModel),
  mc_new(StxModel),
  mc_new(ElxModel)
};



NS_END(tetris)
#endif

