// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__COBJS_H__)
#define __COBJS_H__

#include "core/XConfig.h"
#include "core/ComObj.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
public:
  Missile(c::Sprite* s, AttackMode m) {
    attackMode = m;
    init(s, 1, 0);
    vel.x= 0;
    vel.y= CC_CSV(c::Float, "MISSILE_SPEED");
  }
  virtual ~Missile() {}
  AttackMode attackMode;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
public:

  Bomb(c::Sprite* s, AttackMode m) {
    attackMode = m;
    init(s, 1, 0);
    vel.x= 0;
    vel.y= - CC_CSV(c::Float, "BOMB_SPEED");
  }

  virtual ~Bomb() {}
  AttackMode attackMode;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Enemy : public f::ComObj {
public:

  Enemy(c::Sprite* s, int health, int score) {
    delayTime= 1 + 1.2 * CC_RANDOM();
    init(s, health, score);
  }

  AttackMode attackMode;
  EnemyType enemyType;
  MoveType moveType;
  float delayTime;
  float speed;

  virtual ~Enemy() {}
};

//////////////////////////////////////////////////////////////////////////////
//

class CC_DLL Ship : public f::ComObj {
public:

  Ship(c::Sprite* s, c::Sprite x) {
    canBeAttack = false;
    bornSprite = x;
    init(s,5,0);
  }

  c::Sprite* bornSprite;
  bool canBeAttack;

  virtual ~Ship() {}
};

//////////////////////////////////////////////////////////////////////////////
//

class CC_DLL Motion : public a::Component {
public:
  virtual ~Motion() {}
  Motion() {
    right=false;
    left= false;
    down= false;
    up= false;
  }

  bool right;
  bool left;
  bool down;
  bool up;
};

//////////////////////////////////////////////////////////////////////////////
//

class CC_DLL Spark : public f::ComObj {
public:

  Spark(c::Sprite* sp1, c::Sprite* sp2) {
    duration = 0.7f;
    sprite2= sp2;
    init(sp1, 1, 0);
    scale = 1.2f;
  }

  c::Sprite* sprite2;
  float duration;
  float scale;

  void inflate(float x, float y, float scale) {

    sprite->setOpacity(255.0f);
    sprite->setPosition(x,y);
    sprite->setScale(scale);

    sprite2->setRotation( CCRANDOM_0_1() * 360.0f);
    sprite2->setOpacity(255.0f);
    sprite2->setPosition(x,y);
    sprite2->setScale(scale);

    auto scaleBy = c::ScaleBy::create(duration,3.0f,3.0f);
    auto right = c::RotateBy::create(duration, 45.0f);
    auto seq = c::Sequence::createWithTwoActions(
        c::FadeOut::create(duration),
        c::CallFunc::create([=]() { this->destroy(); }));

    sprite->runAction(right);
    sprite->runAction(scaleBy);
    sprite->runAction(seq);

    sprite2->runAction(scaleBy->clone());
    sprite2->runAction(seq->clone());

    sprite2->setVisible(true);
    status=true;
    sprite->setVisible(true);
  }

  virtual void deflate() {
    sprite2->setVisible(false);
    sprite->setVisible(false);
    status=false;
    sprite2->stopAllActions();
    sprite->stopAllActions();
  }

  virtual ~Spark() {}

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLLL Explosion : public f::ComObj {
public:

  Explosion(c::Sprite* s) {
    animation = c::AnimationCache::getInstance()->getAnimation("Explosion");
    init(s,0,0);
  }

  virtual void inflate(float x, float y) {
    sprite->setPosition(x, y);
    sprite->setVisible(true);
    status=true;
    sprite->runAction(c::Sequence(
        c::Animate::create(animation),
        c::CallFunc::create([=]() { this->deflate();  })));
  }

  virtual ~Explosion() {}
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL HitEffect : public f::ComObj {
public:

  HitEffect(c::Sprite* s) {
    scale = 0.75f;
    init(s, 0,0);
  }

  virtual void inflate(float x, float y) {
    sprite->setRotation( CCRANDOM_0_1() * 360.0f);
    sprite->setPosition(x, y);
    sprite->setScale(scale);
    sprite.setVisible(true);
    status=true;
    sprite->runAction(c::ScaleBy::create(0.3f, 2.0f, 2.0f));
    sprite->runAction(c::Sequence::create(c::FadeOut::create(0.3f),
                               c::CallFunc::create([=]() { this->deflate();  } )));
  }

  virtual ~HitEffect() {}

};




NS_END(terra)
#endif


