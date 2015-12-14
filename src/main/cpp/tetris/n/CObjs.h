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

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShapeShell {
  Shape *shape;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  Shape(BModel *m, int rot, const sstr& p) {
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
  s_vec<> grid;
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL BModel {
protected:

  int dim=0;
  int sz=4;

public:

  virtual void * getLayout(int) = 0;
  int getDim() { return dim; }
  int size() { return sz; }
  int getLayouts() { return 4; }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL BoxModel : public BModel {
protected:

  s_arr<DIM2X2,4> layout {
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

  virtual void * getLayout(int pos) { return &layout[pos]; }

  virtual ~BoxModel() {}
  BoxModel() { dim=2;}
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL ElModel : public BModel {
protected:

  s_arr<DIM3X3,4> layout {
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

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~ElModel() {}
  ElModel() { dim=3; }

};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL ElxModel : public BModel {
protected:

  s_arr<DIM3X3, 4> layout {
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

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~ElxModel() {}

  ElxModel() { dim=3; }
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL LineModel : public BModel {
protected:

  s_arr<DIM4x4, 4> layout {
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

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~LineModel() {}
  LineModel() { dim=4; }
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL NubModel : public BModel {
protected:

  s_arr<DIM3x3, 4> layout {
      0,0,0,
      0,1,0,
      1,1,1 ,

      1,0,0,
      1,1,0,
      1,0,0 ,

      1,1,1,
      0,1,0,
      0,0,0 ,

      0,0,1,
      0,1,1,
      0,0,1
  };

public:

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~NubModel() {}
  NubModel() { dim=3; }

};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL StModel : public BModel {
protected:

  s_arr<DIM3x3, 4> layout {
      0,1,0,
      0,1,1,
      0,0,1 ,

      0,0,0,
      0,1,1,
      1,1,0 ,

      1,0,0,
      1,1,0,
      0,1,0 ,

      0,1,1,
      1,1,0,
      0,0,0
  };

public:

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~StModel() {}
  StModel() { dim=3; }

};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL StxModel : public BModel {
protected:

  s_arr<DIM3x3, 4> layout  {
      0,1,0,
      1,1,0,
      1,0,0 ,

      1,1,0,
      0,1,1,
      0,0,0 ,

      0,0,1,
      0,1,1,
      0,1,0 ,

      0,0,0,
      1,1,0,
      0,1,1
  };

public:

  virtual void * getLayout(int pos) { return &layout[pos]; }
  virtual ~StxModel() {}
  StxModel() { dim=3; }

};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Brick : public c::Sprite {

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

  static Brick * reify(const c::Vec2& pos, const sstr& f0) {
    Brick * b = mc_new(Brick);
    if (NNP(sprite) &&
        b->init()) {
      b->setAnchorPoint(cx::anchorTL());
      b->startPos = pos;
      b->frame0=fo;
      b->setPosition(pos.x, pos.y);
      b->autorelease();
      b->show();
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

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Dropper {
  float dropSpeed = CC_CSV(c::Float, "DROPSPEED");
  float dropRate= 80 + 700.0f/1.0f ;
  c::DelayTime* timer=nullptr;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL FilledLines {
  s_vec<> lines;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion {
  bool right=false;
  bool left=false;
  bool rotr= false;
  bool rotl= false;
  bool down=false;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Pauser {
  c::DelayTime* timer=nullptr;
  bool pauseToClear=false;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL TileGrid {
  s_vec<> tiles;
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

//////////////////////////////////////////////////////////////////////////
//
typedef f::FArray<Brick> FArrBrick;





NS_END(tetris)
#endif

