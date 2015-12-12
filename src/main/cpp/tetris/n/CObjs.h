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

class CC_DLL ShapeDef {
public:
  virtual void* getLayout() = 0;
  int dim=0;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BoxShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  BoxShapeDef() { dim=2;  }
  s_vec<DIM2X2> layout = {
    {1,1,
     1,1},
    {1,1,
     1,1},
    {1,1,
     1,1},
    {1,1,
     1,1}
  };
};

struct CC_DLL ElShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  ElShapeDef() { dim=3; }
  s_vec<DIM3X3> layout = {
    { 0,1,0,
      0,1,0,
      0,1,1 },
    { 0,0,0,
      1,1,1,
      1,0,0 },
    { 1,1,0,
      0,1,0,
      0,1,0 },
    { 0,0,1,
      1,1,1,
      0,0,0  }
  };
};

struct CC_DLL ElxShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  ElxShapeDef() { dim=3; }
  s_vec<DIM3X3> layout = {
    { 0,1,0,
      0,1,0,
      1,1,0 },
    { 1,0,0,
      1,1,1,
      0,0,0 },
    { 0,1,1,
      0,1,0,
      0,1,0 },
    { 0,0,0,
      1,1,1,
      0,0,1 }
  };
};

struct CC_DLL LineShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  LineShapeDef() { dim=4; }
  s_vec<DIM4x4> layout = {
    { 0,0,0,0,
      1,1,1,1,
      0,0,0,0,
      0,0,0,0 },
    { 0,0,1,0,
      0,0,1,0,
      0,0,1,0,
      0,0,1,0 },
    { 0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0 },
    { 0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0 }
  };
};

struct CC_DLL NubShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  NubShapeDef() { dim=3; }
  s_vec<DIM3x3> layout= {
    { 0,0,0,
      0,1,0,
      1,1,1 },
    { 1,0,0,
      1,1,0,
      1,0,0 },
    { 1,1,1,
      0,1,0,
      0,0,0 },
    { 0,0,1,
      0,1,1,
      0,0,1 }
  };
};

struct CC_DLL StShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  StShapeDef() { dim=3; }
  s_vec<DIM3x3> layout= {
    { 0,1,0,
      0,1,1,
      0,0,1 },
    { 0,0,0,
      0,1,1,
      1,1,0 },
    { 1,0,0,
      1,1,0,
      0,1,0 },
    { 0,1,1,
      1,1,0,
      0,0,0 }
  };
};

struct CC_DLL StxShapeDef : public ShapeDef {
  virtual void* getLayout() { return &layout; }
  StxShapeDef() { dim=3; }
  s_vec<DIM3x3> layout = {
    { 0,1,0,
      1,1,0,
      1,0,0 },
    { 1,1,0,
      0,1,1,
      0,0,0 },
    { 0,0,1,
      0,1,1,
      0,1,0 },
    { 0,0,0,
      1,1,0,
      0,1,1 }
  };
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public c::Sprite {
  void blink() {
    setAnchorPoint(cx::anchorTL());
    setSpriteFrame(frame1);
  }
  void show() {
    setAnchorPoint(cx::anchorTL());
    setSpriteFrame(frame0);
  }
  Block(float x, float y, const sstr& f0, const sstr& f1) {
    //this.options = options;
    frame0 = cx::getSpriteFrame(f0);
    frame1 = cx::getSpriteFrame(f1);
    show();
    setPosition(x,y);
  }
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Brick {
  void blink() {
    if ( NNP(block)) {block->blink(); }
  }

  void dispose() {
    if (NNP(block)) {
      block->removeFromParent();
      SNPTR(block)
    }
  }

  Block* create() {
    return block = new Block(startPos.x, startPos.y, frame, "0.png");
  }

  Brick(float x, float y, const sstr& frame) {
    startPos = c::ccp(x,y);
    this->frame=frame;
    //this.options.frames= [ '' + options.frame + '.png', '0.png'];
  }

  virtual ~Brick() {}

  c::Vec2 startPos;
  sstr frame;
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
s_vec<ShapeDef*> ListOfShapeDefs = {
  mc_new(LineShapeDef),
  mc_new(BoxShapeDef),
  mc_new(StShapeDef),
  mc_new(ElShapeDef),
  mc_new(NubShapeDef),
  mc_new(StxShapeDef),
  mc_new(ElxShapeDef)
};


NS_END(tetris)
#endif

