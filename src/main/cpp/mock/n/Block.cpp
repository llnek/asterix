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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
owner<Block*> Block::create() {
  auto b= mc_new(Block);
  b->initWithFile("pics/blank.png");
  CC_HIDE(b);
  b->autorelease();
  return b;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setupBlock(const c::Rect &wz, const c::Size &sz, int type) {

  auto h= sz.height * wz.size.height / TILE_H_SIZE;
  auto w= sz.width * wz.size.width / TILE_W_SIZE;

  this->setTextureRect(c::Rect(0, 0, w, h));
  this->setAnchorPoint(cx::anchorBL());
  _type = type;

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



