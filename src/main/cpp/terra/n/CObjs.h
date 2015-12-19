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
#include "core/CCSX.h"
#include "s/utils.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::ComObj {

  Missile(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    attackMode = m;
    vel.x= 0;
    vel.y= CC_CSV(c::Float, "MISSILE_SPEED");
  }

  virtual const a::COMType typeId() { return "n/Missile"; }
  virtual ~Missile() {}
  NODFT(Missile)
  NOCPYASS(Missile)

  Attacks attackMode=Attacks::NORMAL;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::ComObj {

  Bomb(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    attackMode = m;
    vel.x= 0;
    vel.y= - CC_CSV(c::Float, "BOMB_SPEED");
  }

  virtual const a::COMType typeId() { return "n/Bomb"; }
  virtual ~Bomb() {}
  NODFT(Bomb)
  NOCPYASS(Bomb)

  Attacks attackMode=Attacks::NORMAL;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Enemy : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Enemy"; }

  Enemy(not_null<c::Sprite*> s, const EnemyType &et)
    : ComObj(s, et.HP, et.scoreValue) {
    delayTime= 1.2f * c::rand_0_1() + 1.0f;
    enemyType= et;
  }

  EnemyType enemyType;
  float delayTime= 0;
  float speed=0;

  virtual ~Enemy() {}
  NODFT(Enemy)
  NOCPYASS(Enemy)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Ship"; }

  Ship(not_null<c::Sprite*> s,
       not_null<c::Sprite*> x, int health=5)
    : ComObj(s, health, 0) {
    bornSprite = x;
  }

  c::Sprite *bornSprite=nullptr;
  bool canBeAttack=false;

  virtual ~Ship() {}
  NODFT(Ship)
  NOCPYASS(Ship)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {

  virtual const a::COMType typeId() { return "n/Motion"; }

  virtual ~Motion() {}
  Motion() {}
  NOCPYASS(Motion)

  bool right=false;
  bool left=false;
  bool down=false;
  bool up=false;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Spark : public f::ComObj {

  Spark(not_null<c::Sprite*> sp1, not_null<c::Sprite*> sp2)
    : ComObj(sp1,1,0) {
    sprite2= sp2;
  }

  virtual const a::COMType typeId() { return "n/Spark"; }
  virtual ~Spark() {}
  NODFT(Spark)
  NOCPYASS(Spark)

  c::Sprite *sprite2=nullptr;
  float duration=0.7f;
  float scale=1.2f;

  virtual void inflate(float x, float y) {

    auto scaleBy = c::ScaleBy::create(duration,3.0f,3.0f);
    auto right = c::RotateBy::create(duration, 45.0f);
    auto seq = c::Sequence::createWithTwoActions(
        c::FadeOut::create(duration),
        c::CallFunc::create([=]() { this->deflate(); }));

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
struct CC_DLL Explosion : public f::ComObj {

  virtual const a::COMType typeId() { return "n/Explosion"; }

  Explosion(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  virtual void inflate(float x, float y) {
    auto ac = c::AnimationCache::getInstance();
    auto ani= ac->getAnimation("Explosion");
    sprite->runAction(c::Sequence::create(
          c::Animate::create(ani),
          c::CallFunc::create([=]() {
            this->deflate();
          }), nullptr));
    ComObj::inflate(x, y);
  }

  virtual ~Explosion() {}
  NODFT(Explosion)
  NOCPYASS(Explosion)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HitEffect : public f::ComObj {

  virtual const a::COMType typeId() { return "n/HitEffect"; }

  HitEffect(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  virtual void inflate(float x, float y) {
    sprite->runAction(c::ScaleBy::create(0.3f, 2.0f, 2.0f));
    sprite->runAction(
        c::Sequence::create(c::FadeOut::create(0.3f),
        c::CallFunc::create([=]() { this->deflate();  } ), nullptr));
    sprite->setRotation( cx::randInt(360));
    sprite->setScale(scale);
    ComObj::inflate(x,y);
  }

  float scale=0.75f;

  virtual ~HitEffect() {}
  NODFT(HitEffect)
  NOCPYASS(HitEffect)
};

NS_END(terra)
#endif


