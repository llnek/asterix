// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Tower.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Tower::init() {
  auto sz= cx::getSpriteFrame("opst_02")->getOriginalSize();
  towerSpriteSize = XCFG()->fit(sz);
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  // create the first pair of towers
  // they should be two whole screens away from the dragon
  auto initialPosition = CCT_PT(CC_ZW(wz)*2, wb.cy);
  firstTowerIndex = 0;
  createTower(initialPosition);
  // create the remaining towers
  lastTowerIndex = 0;
  createTower(getNextTowerPosition());
  lastTowerIndex = 1;
  createTower(getNextTowerPosition());
  lastTowerIndex = 2;
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::createTower(const CCT_PT &position) {
  // create a new tower and add it to the array
  TowerBody tower(position);

  // create lower tower sprite & add it to GameWorld's batch node
  tower.lowerSprite = cx::reifySprite("opst_02");
  XCFG()->fit(tower.lowerSprite);
  CC_POS2(tower.lowerSprite,
      position.x,
      position.y - HTV(VERT_GAP_BWN_TOWERS) - HHZ(towerSpriteSize));

  parentNode->addAtlasItem("dhtex", tower.lowerSprite, E_LAYER_TOWER);

  // create upper tower sprite & add it to GameWorld's batch node
  tower.upperSprite = cx::reifySprite("opst_01");
  XCFG()->fit(tower.upperSprite);
  CC_POS2(tower.upperSprite,
      position.x,
      position.y + HTV(VERT_GAP_BWN_TOWERS) + HHZ(towerSpriteSize));
  parentNode->addAtlasItem("dhtex", tower.upperSprite, E_LAYER_TOWER);

  towers.push_back(tower);
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::update(float dt) {
  auto n=0;
  F__LOOP(it, towers) {
    auto &tower = *it;
    // first update the position of the tower
    tower.position.x -= MAX_SCROLLING_SPEED;
    CC_POS2(tower.lowerSprite, tower.position.x, tower.lowerSprite->getPositionY());
    CC_POS2(tower.upperSprite, tower.position.x, tower.upperSprite->getPositionY());

    // if the tower has moved out of the screen, reposition them at the end
    if (tower.position.x <  -HWZ(towerSpriteSize)) {
      repositionTower(n);
      // this tower now becomes the tower at the end
      lastTowerIndex = n;
      // that means some other tower has become first
      firstTowerIndex = ((n+1) >= towers.size()) ? 0 : (n+1);
    }

    ++n;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Tower::repositionTower(int index) {
  auto &tower = towers[index];
  // update tower's position and sprites
  tower.position = getNextTowerPosition();
  CC_POS2(tower.lowerSprite,
      tower.position.x,
      tower.position.y - HTV(VERT_GAP_BWN_TOWERS) - HHZ(towerSpriteSize));
  CC_POS2(tower.upperSprite,
      tower.position.x,
      tower.position.y + HTV(VERT_GAP_BWN_TOWERS)+ HHZ(towerSpriteSize));
}

//////////////////////////////////////////////////////////////////////////////
//
const CCT_PT Tower::getNextTowerPosition() {
  // randomly select either above or below last tower
  auto r=  cx::rand();
  auto offset = r * VERT_GAP_BWN_TOWERS * 0.75;
  auto isAbove = r > 0.5;
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  offset *= (isAbove) ? 1 : -1;

  auto &tower = towers[lastTowerIndex];
  // new position calculated by adding to last tower's position
  auto newPositionX = tower.position.x + HWZ(wz);
  auto newPositionY = tower.position.y + offset;

  // limit the point to stay within 30-80% of the screen
  if (newPositionY >= wb.top  * 0.8) {
    newPositionY -= VERT_GAP_BWN_TOWERS;
  }
  else if (newPositionY <= wb.top * 0.3) {
    newPositionY += VERT_GAP_BWN_TOWERS;
  }

  // return the new tower position
  return CCT_PT(newPositionX, newPositionY);
}

//////////////////////////////////////////////////////////////////////////////
//
TowerBody& Tower::getFrontTower() {
  return towers[firstTowerIndex];
}




NS_END




