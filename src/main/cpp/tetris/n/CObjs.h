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
  int shape;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Shape {
  model= options.model;
  rot= options.rot;
  sstr png;
  float x;
  float y;
  s_vec<> bricks; //=[];
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL CtrlPad {
  hotspots= {};
};

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridBox {
  box= {};
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BlockGrid {
  grid=[];
};

class CC_DLL BModel {
protected:
  virtual void* getLayout(int) = 0;
  int getDim() { return dim; }
  int size() { return sz; }
  int dim=0;
  int sz=4;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxModel : public BModel {

  virtual void* getLayout(int pos) {
    return &layout[pos];
  }
  BoxModel() { dim=2;  }

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
};

struct CC_DLL ElModel : public BModel {

  virtual void* getLayout(int pos) {
    return &layout[pos];
  }
  ElModel() { dim=3; }

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
};

struct CC_DLL ElxModel : public BModel {

  virtual void* getLayout(int pos) { return &layout[pos]; }
  ElxModel() { dim=3; }

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
};

struct CC_DLL LineModel : public BModel {
  virtual void* getLayout(int pos) { return &layout[pos]; }
  LineModel() { dim=4; }
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
};

struct CC_DLL NubModel : public BModel {
  virtual void* getLayout(int pos) { return &layout[pos]; }
  NubModel() { dim=3; }
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
};

struct CC_DLL StModel : public BModel {
  virtual void* getLayout(int pos) { return &layout[pos]; }
  StModel() { dim=3; }

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
};

struct CC_DLL StxModel : public BModel {
  virtual void* getLayout(int pos) { return &layout[pos]; }
  StxModel() { dim=3; }
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
    this->removeFromParent();
  }

  static Brick * reify(float x, float y, const sstr& f0) {
    Brick *sprite = new (std::nothrow) Brick();
    if (NNP(sprite) && sprite->init()) {
      sprite->setAnchorPoint(cx::anchorTL());
      sprite->startPos = c::ccp(x,y);
      sprite->frame0=fo;
      sprite->setPosition(x,y);
      sprite->autorelease();
      sprite->show();
      return sprite;
    }
    CC_SAFE_DELETE(sprite);
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
  float dropRate= 80 + 700/1 ;
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
s_vec<Model*> ListOfModels = {
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

