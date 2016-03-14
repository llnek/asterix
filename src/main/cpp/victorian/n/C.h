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
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
enum {
    kBackground,
    kMiddleground,
    kForeground
};

enum {
    kSpritePlayer,
    kSpriteTerrain,
    kSpriteBlock,
    kSpriteChimney,
    kSpritePuff
};

enum GameState {
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGameTutorial,
    kGameTutorialJump,
    kGameTutorialFloat,
    kGameTutorialDrop
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Widget : public f::CPixie {

  virtual float radius() { return HTV(_width); }
  virtual float height() { return _height; }
  virtual float width() { return _width; }

  Widget(not_null<c::Node*> s) : CPixie(s) {}
  void setSize() {
    _height = CC_CSIZE(node).height;
    _width = CC_CSIZE(node).width;
  }

  DECL_TD(c::Vec2, nextPos)
  DECL_TD(c::Vec2, vel)
  DECL_FZ(maxSpeed)
  DECL_FZ(speed)
  DECL_FZ(_height)
  DECL_FZ(_width)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  DECL_PTR(c::Animate,jamAnimate)
  DECL_PTR(c::MoveTo,jamMove)
  DECL_PTR(c::Sprite,jam)
  DECL_PTR(c::Sprite,hat)

  DECL_PTR(c::Sprite,background)
  DECL_PTR(c::Sprite,foreground)
  DECL_IZ(state)

  DECL_IZ(speedIncreaseInterval)
  DECL_IZ(speedIncreaseTimer)

  MDECL_COMP_TPID( "n/GVars" )
};



NS_END


