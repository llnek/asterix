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
//

#include "platform/CCCommon.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLives : public c::Node {
protected:

  __decl_td(c::Size, _lifeSize)
  __decl_td(c::Vec2, _refPt)
  __decl_td(sstr, _frameId)

  s_vec<c::Node*> _icons;

  __decl_iz(_totalLives)
  __decl_iz(_curLives)
  __decl_tv(int, _dir, 1)
  __decl_tv(float, _scale, 1.0f)

public:

  virtual void decorate( const sstr &frame, int lives,
      float x, float y,
      float scale=1.0f, int dir= 1);

  bool isDead() { return _curLives <= 0; }
  int getLives() { return _curLives; }

  void reduce(int cnt = 1);
  void reset();
  void resurrect();
  void drawLives();

  virtual ~XLives() {}
  XLives() {}
  __decl_nocpyass(XLives)
};



NS_END


