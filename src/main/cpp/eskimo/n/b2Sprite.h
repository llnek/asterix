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

#include "x2d/GameScene.h"
#include "2d/CCSprite.h"
#include "Box2D/Box2D.h"
#include "lib.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL b2Sprite : public c::Sprite  {

  virtual void setSpritePosition (const c::Vec2& );
  virtual void update();
  virtual void hide();
  virtual void reset();

  DECL_PTR(b2Body, _body)

  b2Sprite() {}

};


NS_END


