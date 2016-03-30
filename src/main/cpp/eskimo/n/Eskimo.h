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
#include "b2Sprite.h"

NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL EskimoSprite : public b2Sprite {
friend class Eskimo;

  static EskimoSprite* create(GVars*);
  void bind(GVars*);

  void makeCircleShape();
  void makeBoxShape();

public:

  __decl_bf(_switchShape)
  __decl_iz(_state)
  __decl_ptr(GVars,ss)

  virtual void update();
  virtual void reset();

};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Eskimo : public f::CPixie {

  Eskimo(not_null<c::Node*> n)
  : CPixie(n) {
    MGML()->addAtlasItem("game-pics", node, kMiddleground);
  }

public:

  static owner<Eskimo*> create(not_null<GVars*> ss) {
    return new Eskimo(EskimoSprite::create(ss));
  }

};




NS_END



