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

#include "core/CCSX.h"
#include "core/COMP.h"
#include "lib.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Gem : public f::CPixie {

  Gem( int t) {
    node= cx::reifySprite(getGemPng(t));
    type=t;
  }

  DECL_TV(int,type,-1)
  DECL_BF(selected)

public:

  static Gem* create() {
    return mc_new1(Gem,TYPE_GEM_BLUE);
  }

  void deselect() {selected=false; }
  void select() { selected=true; }
  void reset() {
    node->setScale(1, 1);
    node->setRotation(0);
  }
  int getType() { return type; }
  void setType(int t) {
    SCAST(c::Sprite*,node)->setSpriteFrame(getGemPng(t));
    type = t;
  }

};


NS_END


