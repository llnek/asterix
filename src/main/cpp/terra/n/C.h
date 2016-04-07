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
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
enum class Moves { RUSH, VERT, HORZ, OLAP };
enum class Attacks { TSUIHIKIDAN, NORMAL };

struct CC_DLL EnemyType {

  EnemyType(Attacks attackMode, Moves moveType,
    int type,
    const sstr &textureName,
    const sstr &bulletType, int HP, int scoreValue) {

    this->attackMode = attackMode;
    this->moveType= moveType;
    this->type= type;
    this->textureName = textureName;
    this->bulletType = bulletType;
    this->HP= HP;
    this->scoreValue = scoreValue;
  }

  EnemyType() {}

  __decl_mv(Attacks, attackMode, Attacks::NORMAL)
  __decl_mv(Moves, moveType,Moves::RUSH)
  __decl_iz(type)
  __decl_md(sstr, textureName)
  __decl_md(sstr, bulletType)
  __decl_iz(HP)
  __decl_iz(scoreValue)

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Missile : public f::CPixie {
  __decl_md(Attacks, _attackMode)
  Missile(Attacks m) {
    _attackMode=m;
  }
public:

  static owner<Missile*> create(const sstr& png, Attacks m= Attacks::NORMAL);

  __decl_gsms(Attacks,_attackMode, AttackMode)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Enemy : public f::CStats {

  __decl_md(EnemyType, enemyType)
  __decl_fz(delayTime)
  __decl_comp_tpid( "n/Enemy")

  Enemy(const EnemyType &et)
  : CStats(et.scoreValue) {
    delayTime= 1.2 * cx::rand() + 1.0;
    enemyType= et;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Ship : public f::CPixie {

  virtual bool initWithSpriteFrameName(const sstr&);

  __decl_ptr(c::Sprite, _bornSprite)
  Ship() {}

public:

  __decl_getr(c::Sprite*,_bornSprite,BornSprite)
  static owner<Ship*> create();

  void hideGhost() { CC_HIDE(_bornSprite); }
  void showGhost() {
    _bornSprite->setScale(8);
    CC_SHOW(_bornSprite);
    _bornSprite->runAction(
      c::ScaleTo::create(0.5, 1, 1));
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Spark : public f::CPixie {

  __decl_mv(float, _duration, 0.7)
  __decl_mv(float, _scale, 1.2)
  __decl_ptr(c::Sprite, _sprite2)

    virtual bool initWithSpriteFrameName(const sstr&);
  Spark() {}

public:

  __decl_getr(c::Sprite*,_sprite2,Ghost)
  __decl_getr(float,_duration,Duration)
  __decl_getr(float,_scale,Scale)

  virtual void inflate(float, float);
  virtual void deflate();

  static owner<Spark*> create(const sstr&);
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Explosion : public f::CPixie {

  Explosion() {}

public:

  virtual void inflate(float x, float y);

  static owner<Explosion*> create(const sstr&);
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL HitEffect : public f::CPixie {

  __decl_mv(float, _scale, 0.75)

  HitEffect() {}

public:

  virtual void inflate(float x, float y);
  static owner<HitEffect*> create(const sstr&);

  __decl_getr(float,_scale,Scale)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")
  __decl_iz(secCount)
  __decl_ptr(ecs::Node, backSkyRe)
  __decl_ptr(ecs::Node, backSky)
  __decl_md(CCT_SZ, backSkyDim)
};

void flareEffect(not_null<c::Sprite*> flare, VOIDFN cb);

inline void btnEffect() {  cx::sfxPlay("btnEffect"); }

void fireMissiles(not_null<ecs::Node*> ship, float dt);

void bornShip(not_null<ecs::Node*> ship);

void processTouch(not_null<ecs::Node*> ship, const CCT_PT&);

const s_arr<EnemyType,6>& EnemyTypes();

NS_END


