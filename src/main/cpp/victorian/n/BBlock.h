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

#include "C.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_BEGIN(victorian)

enum {
  kWallTile,
  kRoofTile,
  kChimney
};

enum {
  kBlockGap,
  kBlock1,
  kBlock2,
  kBlock3,
  kBlock4
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public c::Sprite {
protected:

  virtual bool initWithSpriteFrameName(const sstr&);

  __decl_bf(_puffing)
  __decl_iz(_type)

  c::Vector<c::Sprite*> _chimneys;
  Block() {}

  c::Vector<c::Sprite*> _wallTiles;
  c::Vector<c::Sprite*> _roofTiles;

  __decl_ptr(c::SpriteFrame, _roof2)
  __decl_ptr(c::SpriteFrame, _roof1)

  __decl_ptr(c::SpriteFrame,_tile4)
  __decl_ptr(c::SpriteFrame,_tile3)
  __decl_ptr(c::SpriteFrame,_tile2)
  __decl_ptr(c::SpriteFrame,_tile1)

  __decl_ptr(c::Action, _puffAnimation)
  __decl_ptr(c::Action, _puffSpawn)
  __decl_ptr(c::Action, _puffMove)
  __decl_ptr(c::Action, _puffFade)
  __decl_ptr(c::Action, _puffScale)

  __decl_iz(_puffIndex)

public:

  bool isPuffing() { return _puffing; }
  void setPuffing(bool value);
  void createPuff();

  void setup(const CCT_SZ&, int type);
  static owner<Block*> create();

  __decl_gsms(int,_type,Type)
  virtual ~Block();

};


NS_END



