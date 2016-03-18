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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Enemy.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
Enemy* Enemy::create() {

  auto sp= cx::reifySprite("enemy.png");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  CC_HIDE(sp);
  MGML()->addAtlasItem("game-pics", sp);

  auto anim = c::Animation::create();
  auto enemy= mc_new1(Enemy,sp);

  for (auto i = 1; i <= 4; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("enemy_idle_"+ FTOS(i)+".png"));
  }
  anim->setDelayPerUnit(0.25);

  enemy->idle= c::RepeatForever::create(c::Animate::create(anim));
  CC_KEEP(enemy->idle);

  return enemy;
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::lockAndLoad() {
  node->schedule([=](float dt) {
      this->shoot(dt);
      },
      CC_CSV(c::Float, "ENEMY+FIRE+DELAY"),
      "Enemy::shoot");
  node->runAction(this->idle->clone());
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::sync() {
  node->setPositionX(node->getPositionX() - 3);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::shoot(float dt) {
  auto po= MGMS()->getPool("Bullets");
  auto e=po->take(true);
  auto b= CC_GEC(Projectile,e,"f/CPixie");
  auto pos= this->pos();
  auto sz= b->csize();

  b->inflate(pos.x - HWZ(sz), pos.y - CC_ZH(sz) * 0.05);
  cx::sfxPlay("gunshot");
}


NS_END

