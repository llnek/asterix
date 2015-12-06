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

#if !defined(__UTILS_H__)
#define __UTILS_H__

#include "core/ComObj.h"
#include "cocos2d.h"
NS_ALIAS(terra)

//////////////////////////////////////////////////////////////////////////////
//
enum class Moves { RUSH, VERT, HORZ, OLAP };

enum class Attacks { TSUIHIKIDAN, NORMAL };

struct EnemyType {

  EnemyType(Attacks attackMode, Moves moveType,
    int type,
    stdtsr textureName,
    stdstr bulletType, int HP, int scoreValue) {
    this->attackMode = attackMode;
    this->moveType= moveType;
    this->ytpe= type;
    this->textureName = textureName;
    this->bulletType = bulletType;
    this->HP= HP;
    this->scoreValue = scoreValue;
  }

  Attacks attackMode;
  Moves moveType;
  int type;
  stdtsr textureName;
  stdstr bulletType;
  int HP;
  int scoreValue;
};

const s::array<EnemyType, 6> EnemyTypes = {

  {
    Attacks::NORMAL,
    Moves::RUSH,
    0,
    "E0.png",
    "W2.png",
    1,
    15
  },
  {
    Attacks::NORMAL,
    Moves::RUSH,
    1,
    "E1.png",
    "W2.png",
    2,
    40
  },
  {
    Attacks::TSUIHIKIDAN,
    Moves::HORZ,
    2,
    "E2.png",
    "W2.png",
    4,
    60
  },
  {
    Attacks::NORMAL,
    Moves::OLAP,
    3,
    "E3.png",
    "W2.png",
    6,
    80
  },
  {
    Attacks::TSUIHIKIDAN,
    Moves::HORZ,
    4,
    "E4.png",
    "W2.png",
    10,
    150
  },
  {
    Attacks::NORMAL,
    Moves::HORZ,
    5,
    "E5.png",
    "W2.png",
    15,
    200
  }
};

void flareEffect(not_null<c::Sprite*> flare, c::SEL_CallFunc cb, c::Ref* target);
void btnEffect();

void fireMissiles(not_null<f::ComObj*> ship, float dt);

void bornShip(not_null<f::ComObj*> ship);

void processTouch(not_null<f::ComObj*> ship, const c::Vec2& delta);

NS_END(terra)
#endif

