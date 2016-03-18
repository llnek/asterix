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
#include "Move.h"
#include "n/Enemy.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto hero= CC_GEC(Hero,_player,"f/CPixie");
  auto sz= hero->csize();
  auto wb= cx::visBox();
  auto gameOver=false;

  hero->animate();

  //enemies
  auto po= MGMS()->getPool("Enemies");
  auto es= po->ls();
  F__LOOP(it, es) {
    auto n= *it;
    auto e= CC_GEC(Enemy,n,"f/CPixie");
    if (n->status()) {
      e->sync();
      if (e->node->getPositionX() + HWZ(e->csize()) < 0) {
        gameOver=true;
        break;
      }
    }
  }

  if (gameOver) {
    SENDMSG("/game/player/lose");
    return;
  }

  //enemy bullets
  po = MGMS()->getPool("Bullets");
  auto bs= po->ls();
  F__LOOP(it, bs) {
    auto b= *it;
    auto p= CC_GEC(Projectile,b,"f/CPixie");
    p->sync();
    if (p->node->getPositionX() <= wb.left ) {
      cx::hibernate((ecs::Node*)b);
    }
  }

  //player bullets
  po = MGMS()->getPool("Rockets");
  auto rs= po->ls();
  F__LOOP(it, rs) {
    auto r= *it;
    auto p = CC_GEC(Projectile,r,"f/CPixie");
    p->sync();
    if (p->node->getPositionX() >= wb.right) {
      p->deflate();
      cx::hibernate((ecs::Node*)r);
    }
  }

  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
}


NS_END




