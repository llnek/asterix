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
#include "core/COMP.h"
#include "core/CCSX.h"
#include "cocos2d.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
enum class Moves { RUSH, VERT, HORZ, OLAP };
enum class Attacks { TSUIHIKIDAN, NORMAL };
class GEngine;
struct EnemyType {

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

  __decl_tv(Attacks, attackMode, Attacks::NORMAL)
  __decl_tv(Moves, moveType,Moves::RUSH)
  __decl_iz(type)
  __decl_td(sstr, textureName)
  __decl_td(sstr, bulletType)
  __decl_iz(HP)
  __decl_iz(scoreValue)

};

const s::array<EnemyType, 6> EnemyTypes = {
  EnemyType(Attacks::NORMAL, Moves::RUSH, 0, "E0.png", "W2.png", 1, 15),
  EnemyType(Attacks::NORMAL, Moves::RUSH, 1, "E1.png", "W2.png", 2, 40 ),
  EnemyType(Attacks::TSUIHIKIDAN, Moves::HORZ, 2, "E2.png", "W2.png", 4, 60),
  EnemyType(Attacks::NORMAL, Moves::OLAP, 3, "E3.png", "W2.png", 6, 80 ),
  EnemyType(Attacks::TSUIHIKIDAN, Moves::HORZ, 4, "E4.png", "W2.png", 10, 150 ),
  EnemyType(Attacks::NORMAL, Moves::HORZ, 5, "E5.png", "W2.png", 15, 200 )
};

//void flareEffect(not_null<c::Sprite*> flare, c::SEL_CallFunc cb, c::Ref* target);

void flareEffect(not_null<c::Sprite*> flare, VOIDFN cb);

inline void btnEffect() {  cx::sfxPlay("btnEffect"); }

void fireMissiles(GEngine*, not_null<ecs::Node*> ship, float dt);

void bornShip(GEngine*, not_null<ecs::Node*> ship);

void processTouch(not_null<ecs::Node*> ship, const c::Vec2& delta);


NS_END

