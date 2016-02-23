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

#include "2d/CCSprite.h"
#include "n/lib.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class GVars;
class CC_DLL Igloo : public c::Sprite {

  const sstr fmtGPng(int g) { return "block_large_" + s::to_string(g) + ".png"; }
  DECL_PTR(c::Sprite, _block)
  void addBlock();

public:

  void initIgloo(int gravity, const c::Vec2&);
  static Igloo* create(GVars*);

  virtual ~Igloo();
  Igloo(GVars*);

};




NS_END


