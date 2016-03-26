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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Block.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
owner<Block*> Block::create(const c::Rect &frame) {

  auto b= mc_new(Block);
  b->_tileHeight = frame.size.height / TILE_H_SIZE;
  b->_tileWidth = frame.size.width / TILE_W_SIZE;
  b->initWithFile("pics/blank.png");
  CC_HIDE(b);
  b->autorelease();
  return b;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setupBlock(int width, int height, int type) {

  _height = height * _tileHeight;
  _width = width * _tileWidth;
  _type = type;

  this->setTextureRect(c::Rect(0, 0, _width, _height));
  this->setAnchorPoint(cx::anchorBL());

  switch (type) {

    case kBlockGap:
      CC_HIDE(this);
      return;

    case kBlock1:
      this->setColor(c::Color3B(200,200,200));
      break;

    case kBlock2:
      this->setColor(c::Color3B(150,150,150));
      break;

    case kBlock3:
      this->setColor(c::Color3B(100,100,100));
      break;

    case kBlock4:
      this->setColor(c::Color3B(50,50,50));
      break;
  }

  CC_SHOW(this);

}



NS_END


