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
class CC_DLL PlatformSprite : public b2Sprite {
friend class Platform;

  static PlatformSprite* create(GVars*);
  s_vec<c::Sprite*> _tiles;
  DECL_PTR(GVars, ss)

  void switchTexture();
  void createTiles();
    void bind(GVars*);
public:

  void initPlatform(int width, float angle, const c::Vec2&);

  virtual ~PlatformSprite();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Platform : public f::CPixie {

  Platform(not_null<c::Node*> n)
    : CPixie(n) {
  }

public:

  static owner<Platform*> create(not_null<GVars*> ss) {
    return new Platform(PlatformSprite::create(ss));
  }

};




NS_END

