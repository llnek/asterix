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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gem : public c::Sprite {

  static Gem* create() {
    auto g= new Gem();
    g->init();
    g->autorelease();
    return g;
  };

  Gem() {
    this->skin = cx::loadSprite("gem_blue.png");
    this->gemContainer = c::Node::create();
    this->type = TYPE_GEM_BLUE;
    this->selected = false;
  }

  DECL_BF(selected)

  int getType() { return type; }
  void deselect();
  void select();
  void reset();
};

//////////////////////////////////////////////////////////////////////////////
//
void Gem::select() {
  if (selected) {
  } else {
    selected = true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Gem::deselect() {
  if (!selected ) {
  } else {
    selected = false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Gem:reset() {
  gemContainer->setScale(1, 1);
  gemContainer->setRotation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Gem::setType(int value) {
  auto tc = CC_DTOR()->getTextureCache();
  this->type = value;
  skin->setTexture(tc->addImage("pics/" + getGemPng(type)));
}

bool Gem:init() {
  setVisible(false);
  self.gemContainer:addChild(self.skin)
  self:addChild(self.gemContainer)
}



NS_END




