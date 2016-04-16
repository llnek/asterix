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

#include "ParticleLayer.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
ParticleLayer::ParticleLayer(const CCT_PT &p) {
  _enemyDie = ParticleSpin::create("EnemyDie.png", p);
  addChild(_enemyDie);

  _enemyGun = ParticleSpin::create("EnemyGun.png", p);
  addChild(_enemyGun);

  scheduleOnce( schedule_selector(ParticleLayer::removeSelf),2.0);
  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void ParticleLayer::update(float dt) {
  _enemyDie->update(dt);
  _enemyGun->update(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void ParticleLayer::removeSelf(float dt) {
  this->unscheduleUpdate();
  this->removeFromParent();
}

NS_END

