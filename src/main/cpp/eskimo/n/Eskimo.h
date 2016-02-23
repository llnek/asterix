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

#include "lib.h"
#include "b2Sprite.h"

NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL EskimoSprite : public b2Sprite {

  void makeCircleShape();
  void makeBoxShape();

public:

  static EskimoSprite* create(not_null<GVars*>);

  DECL_BF(_switchShape)
  DECL_IZ(_state)
  DECL_PTR(GVars,ss)

  EskimoSprite(not_null<GVars*>);
  virtual ~EskimoSprite() {}

  virtual void update();
  virtual void reset();

};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Eskimo : public f::ComObj {

  MDECL_COMP_TPID("n/Eskimo")

  Eskimo(not_null<GVars*> g) {
    node= EskimoSprite::create(g);
    MGML()->addAtlasItem("game-pics", node, kMiddleground);
  }

};




NS_END



