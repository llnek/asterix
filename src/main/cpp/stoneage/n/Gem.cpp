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

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
Gem* Gem::create() {
  auto png= getGemPng(TYPE_GEM_BLUE);
  auto s= cx::loadSprite(png);
  return mc_new2(Gem,s,TYPE_GEM_BLUE);
}

//////////////////////////////////////////////////////////////////////////////
//
Gem::Gem(not_null<c::Node*> n, int t) : ComObj(n) {
  type = t;
}

//////////////////////////////////////////////////////////////////////////////
//
void Gem::reset() {
  node->setScale(1, 1);
  node->setRotation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Gem::setType(int value) {
  auto tc = CC_DTOR()->getTextureCache();
  auto p=getGemPngPath(value);
  node->setTexture(tc->addImage(p));
  type = value;
}

NS_END



