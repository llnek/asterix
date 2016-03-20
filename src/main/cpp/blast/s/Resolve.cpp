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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

#include "n/Missile.h"
#include "n/Enemy.h"
#include "n/Blast.h"
#include "n/Bomb.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
    USING_NS_CC;
  c::Object *object = CC_NIL;
  CCARRAY_FOREACH(ss->enemies, object) {
    auto enemy = (Enemy*)object;
    if(enemy && enemy->mustBeRemoved) {
      ss->enemies->removeObject(enemy);
    }
  }

  object = CC_NIL;
  CCARRAY_FOREACH(ss->powerups, object) {
    auto powerup = (PowerUp*)object;
    if(powerup && powerup->mustBeRemoved) {
      ss->powerups->removeObject(powerup);
    }
  }

  object = CC_NIL;
  CCARRAY_FOREACH(ss->blasts, object) {
    auto blast = (Blast*)object;
    if(blast && blast->mustBeRemoved) {
      ss->blasts->removeObject(blast);
    }
  }

  object = NULL;
  CCARRAY_FOREACH(ss->missiles, object) {
    auto missile = (Missile*)object;
    if (missile && missile->mustBeRemoved) {
      ss->missiles->removeObject(missile);
    }
  }

}


NS_END




