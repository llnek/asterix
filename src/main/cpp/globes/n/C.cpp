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

#include "C.h"
NS_BEGIN(globes)

//////////////////////////////////////////////////////////////////////////////
//
owner<GameTile*> GameTile::create(GVars *ss, int tile) {
  assert(tile >= 0 && tile < ss->tileTypes.size());
  auto z= new GameTile();
  z->tileColor= ss->tileTypes[tile];
  z->initWithSpriteFrameName(z->tileColor);
  z->autorelease();
  return z;
}




NS_END




