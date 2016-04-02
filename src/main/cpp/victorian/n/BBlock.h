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

#include "core/COMP.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_BEGIN(victorian)

enum {
  kWallTile,
  kRoofTile,
  kChimney
};

enum  {
  kBlockGap,
  kBlock1,
  kBlock2,
  kBlock3,
  kBlock4
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public c::Sprite {

  virtual bool initWithSpriteFrameName(const sstr&);
  void createPuff();
  Block() {}

  __decl_ptr(c::Action, _puffAnimation)
  __decl_ptr(c::Action, _puffSpawn)
  __decl_ptr(c::Action, _puffMove)
  __decl_ptr(c::Action, _puffFade)
  __decl_ptr(c::Action, _puffScale)

  __decl_ptr(c::SpriteFrame, _tile4)
  __decl_ptr(c::SpriteFrame, _tile3)
  __decl_ptr(c::SpriteFrame, _tile2)
  __decl_ptr(c::SpriteFrame, _tile1)
  __decl_ptr(c::SpriteFrame, _roof2)
  __decl_ptr(c::SpriteFrame, _roof1)

  c::Vector<c::Sprite*> _wallTiles;
  c::Vector<c::Sprite*> _roofTiles;
  c::Vector<c::Sprite*> _chimneys;

  __decl_iz(_puffIndex)
  __decl_bf(_puffing)
  __decl_iz(_type)

  __decl_fz(_height)
  __decl_fz(_width)
  __decl_fz(_tileH)
  __decl_fz(_tileW)

public:

  void setup(float width, float height, int type);
  static owner<Block*> create();
  virtual ~Block();

  __declapi_setr(bool,_puffing,Puffing)
  __decl_gsms(int, _type, Type)
  __decl_ismx(_puffing,Puffing)

  __decl_getr(float,_height, Height)
  __decl_getr(float,_width, Width)

  float getBottom() { return 0; }
  float getTop() { return _height; }
  float getRight() { return getPositionX() + _width; }
  float getLeft() { return getPositionX(); }
};


NS_END



