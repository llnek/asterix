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

#include "b2Sprite.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
class GVars;
class CC_DLL Platform : public b2Sprite {

  s_vec<c::Sprite*> _tiles;

  void switchTexture();
  void createTiles();

public:

  void initPlatform(int width, float angle, const c::Vec2&);
  virtual ~Platform();
  Platform(GVars*);
  static Platform* create(GVars*);
};


NS_END

