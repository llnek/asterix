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

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "Enemy.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
Enemy* Enemy::create() {
  auto i= CC_TCAC()->addImage( XCFG()->getImage("enemy_anim.png"));
  auto a= c::SpriteBatchNode::createWithTexture(i);
  auto s= cx::reifyEnemy("enemy_idle_1.png");
  s->addChild(a);
  //this->schedule(schedule_selector(Enemy::shoot),1.8);
  return  mc_new1(Enemy,s);
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::sync() {
  node->setPosition(c::ccpAdd(node->getPosition(), c::Vec2(-3, 0)));
}

void Enemy::shoot(float dt)
{

  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("gunshot.wav");

  //CCLog("[Enemy] shoot");
  CCPoint p = this->getPosition();

  p.x = p.x - this->getContentSize().width/2;
  p.y = p.y - this->getContentSize().height * 0.05;

  Projectile* pr= Projectile::createProjectile(p,1);

  projLayer->addChild(pr);
  projLayer->enemyBullets->addObject(pr);
}


NS_END


