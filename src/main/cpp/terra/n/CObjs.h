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
#include "core/CCSX.h"
#include "core/ComObj.h"
#include "s/utils.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::ComObj {
public:

  Missile(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    attackMode = m;
    vel.x= 0;
    vel.y= CC_CSV(c::Float, "MISSILE_SPEED");
  }

  virtual const a::COMType typeId() { return "n/Missile"; }
  virtual ~Missile() {}
  NOCPYASS(Missile)
  NODFT(Missile)

  Attacks attackMode;
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Bomb : public f::ComObj {
public:

  Bomb(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    attackMode = m;
    vel.x= 0;
    vel.y= - CC_CSV(c::Float, "BOMB_SPEED");
  }

  virtual const a::COMType typeId() { return "n/Bomb"; }
  virtual ~Bomb() {}
  NOCPYASS(Bomb)
  NODFT(Bomb)

  Attacks attackMode;
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Enemy : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Enemy"; }

  Enemy(not_null<c::Sprite*> s, const EnemyType& et)
    : ComObj(s) {
    delayTime= 1.2f * c::rand_0_1() + 1;
    enemyType= et;
  }

  EnemyType enemyType;
  Attacks attackMode;
  Moves moveType;
  float delayTime;
  float speed;

  virtual ~Enemy() {}
  NOCPYASS(Enemy)
  NODFT(Enemy)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Ship"; }

  Ship(not_null<c::Sprite*> s,
       not_null<c::Sprite*> x, int health=5)
    : ComObj(s, health, 0) {

    canBeAttack = false;
    bornSprite = x;
  }

  c::Sprite* bornSprite;
  bool canBeAttack;

  virtual ~Ship() {}
  NOCPYASS(Ship)
  NODFT(Ship)
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motion : public a::Component {
public:

  virtual const a::COMType typeId() { return "n/Motion"; }
  virtual ~Motion() {}
  NOCPYASS(Motion)

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

  Spark(not_null<c::Sprite*> sp1, not_null<c::Sprite*> sp2)
    : ComObj(sp1,1,0) {
    duration = 0.7f;
    sprite2= sp2;
    scale = 1.2f;
  }

  virtual const a::COMType typeId() { return "n/Spark"; }
  virtual ~Spark() {}
  NOCPYASS(Spark)
  NODFT(Spark)

  c::Sprite* sprite2;
  float duration;
  float scale;

  void inflate(float x, float y, float scale) {

    auto scaleBy = c::ScaleBy::create(duration,3.0f,3.0f);
    auto right = c::RotateBy::create(duration, 45.0f);
    auto seq = c::Sequence::createWithTwoActions(
        c::FadeOut::create(duration),
        c::CallFunc::create([=]() { this->destroy(); }));

    sprite2->setRotation( cx::randInt(360));
    sprite2->setOpacity(255.0f);
    sprite2->setPosition(x,y);
    sprite2->setScale(scale);
    sprite2->setVisible(true);
    sprite2->runAction(scaleBy->clone());
    sprite2->runAction(seq->clone());

    sprite->setOpacity(255.0f);
    sprite->setScale(scale);
    sprite->runAction(right);
    sprite->runAction(scaleBy);
    sprite->runAction(seq);

    ComObj::inflate(x,y);
  }

  virtual void deflate() {
    sprite2->setVisible(false);
    sprite2->stopAllActions();
    ComObj::deflate();
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/Explosion"; }

  Explosion(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  virtual void inflate(float x, float y) {
    auto ani = c::AnimationCache::getInstance()->getAnimation("Explosion");
    sprite->runAction(c::Sequence::create(
          c::Animate::create(ani),
          c::CallFunc::create([=]() { this->deflate();  })));
    ComObj::inflate(x, y);
  }

  virtual ~Explosion() {}
  NOCPYASS(Explosion)
  NODFT(Explosion)
};


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL HitEffect : public f::ComObj {
public:

  virtual const a::COMType typeId() { return "n/HitEffect"; }

  HitEffect(not_null<c::Sprite*> s)
    : ComObj(s) {
    scale = 0.75f;
  }

  virtual void inflate(float x, float y) {
    sprite->runAction(c::ScaleBy::create(0.3f, 2.0f, 2.0f));
    sprite->runAction(
        c::Sequence::create(c::FadeOut::create(0.3f),
          c::CallFunc::create([=]() { this->deflate();  } )));
    sprite->setRotation( cx::randInt(360));
    sprite->setScale(scale);
    ComObj::inflate(x,y);
  }

  float scale;

  virtual ~HitEffect() {}
  NOCPYASS(HitEffect)
  NODFT(HitEffect)
};

NS_END(terra)
#endif


