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
#include "Collide.h"
#include "n/Enemy.h"
#include "p/ParticleLayer.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  players=engine->getNodeList(PlayerNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto hero=CC_GNLF(Hero,players,"player");
  auto pr= MGMS()->getPool("Rockets");
  auto pe= MGMS()->getPool("Enemies");
  auto pb= MGMS()->getPool("Bullets");
  auto &rs= pr->list();
  auto &es= pe->list();
  auto &bs= pb->list();
  bool gameOver=false;

  F__LOOP(it, rs) {
    auto r= (Projectile*) *it;
    if (!r->status) { continue;  }
    F__LOOP(it2, es) {
      auto e = (Enemy*) *it;
      if (e->status && cx::collide(r,e)) {
        auto pLayer = mc_new1(ParticleLayer, e->pos());
        MGML()->addItem(pLayer);
        cx::sfxPlay("rocketExplode");
        cx::sfxPlay("enemyKill");
        r->hurt();
        e->hurt();
        auto msg= j::json({
            {"score", 1}
            });
        SENDMSGEX("/game/player/earnscore", &msg);
        r->deflate();
        e->deflate();
      }
    }
  }

  F__LOOP(it, bs) {
    auto b= (Projectile*) *it;
    if (b->status && cx::collide(b, hero)) {
      cx::sfxPlay("playerKill");
      hero->hurt();
      b->hurt();
      b->deflate();
      gameOver=true;
      break;
    }
  }

  if (gameOver) {
    SENDMSG("/game/player/lose");
  }
}

NS_END




