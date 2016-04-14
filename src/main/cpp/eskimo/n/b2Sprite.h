// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "2d/CCSprite.h"
#include "C.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL b2Sprite : public f::CPixie  {

  __decl_ptr(b2Body, _body)
  b2Sprite() {}

public:

  virtual void setPosition (const CCT_PT&);
  virtual void update();
  virtual void hide();
  virtual void reset();

  __decl_getr(b2Body,_body,Body)

};


NS_END


