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

#include "x2d/XNode.h"
#include "C.h"

NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Fairytale {

  __decl_vec(c::Sprite*, silhouetteSprites)
  __decl_vec(c::Sprite*, castleSprites)

  __decl_md(CCT_SZ,silhouetteSpriteSize)
  __decl_md(CCT_SZ,castleSpriteSize)
  __decl_ptr(f::XNode,parentNode)
  __decl_iz(lastSilhouetteIndex)
  __decl_iz(lastCastleIndex)

  void createSilhouette();
  void createCastle();
  void createStars();
  void updateCastle();
  void updateSilhouette();

public:

  Fairytale(f::XNode *n) { parentNode=n; }
  void update(float);
  void init();

};

NS_END




