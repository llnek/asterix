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

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
GridController* GridController::create() {
  return mc_new(GridController);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::checkGridMatches() {

  auto sy=CC_CSV(c::Integer, "GRID_SIZE_Y");
  auto sx=CC_CSV(c::Integer, "GRID_SIZE_X");

  matchArray.clear();

  for (auto c = 0; c < sx; ++c) {
    for (auto r = 0; r < sy; ++r) {
      this->checkTypeMatch(c,r);
    }
  }

  if (matchArray.size() >= 2) {
    MGML()->addToScore();
    return true
  }

  CCLOG("no matches");
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::checkTypeMatch(int c, int r) {
  auto sx=CC_CSV(c::Integer, "GRID_SIZE_X");
  auto sy=CC_CSV(c::Integer, "GRID_SIZE_Y");
  auto type = MGML()->grid[c][r];
  auto stepC = c;
  auto stepR = r;
  auto temp_matches = {};

  //check top
  while (stepR -1 >= 0 &&
      MGML()->grid[c][stepR-1] == type ) {
    stepR -= 1;
    table.insert (temp_matches, {x = c, y = stepR});
  }

  if (temp_matches.size() >= 2) {
    this->addMatches(temp_matches);
  }

  temp_matches.clear();

  //check bottom
  stepR = r;
  while (stepR + 1 < sy &&
      MGML()->grid[c][stepR + 1] == type) {
    stepR += 1;
    table.insert(temp_matches, {x = c, y= stepR});
  }

  if (temp_matches.size() >= 2) {
    this->addMatches(temp_matches);
  }

  temp_matches.clear();

  //check left
  while (stepC - 1 >= 0 &&
      MGML()->grid[stepC - 1][r] == type) {
    stepC -= stepC;
    table.insert (temp_matches, {x = stepC, y= r});
  }

  if (temp_matches.size() >= 2) {
    this->addMatches(temp_matches);
  }

  temp_matches.clear();

  //check right
  stepC = c;
  while (stepC + 1 < sx &&
      MGML()->grid[stepC + 1][r] == type) {
    stepC += 1;
    table.insert (temp_matches, {x = stepC, y = r});
  }

  if (temp_matches.size() >= 2) {
    this->addMatches(temp_matches);
  }

}


//////////////////////////////////////////////////////////////////////////////
//
void GridController::addMatches(matches) {
  F__LOOP(it,matches) {
      if (self:find(value, self.matchArray) == false) then
          table.insert(self.matchArray, value)
      end
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::find (np, arr) {
  F__LOOP(it,arr) {
    if (value.x == np.x and value.y == np.y) then return true end
  }
  return false
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::findGemAtPosition(const f::Cell2P position) {
  auto grid= CC_CSV(c::Integer,"GRID_SPACE");
  auto tile= CC_CSV(c::Integer,"TILE_SIZE");
  auto sx=CC_CSV(c::Integer,"GRID_SIZE_X");
  auto sy=CC_CSV(c::Integer,"GRID_SIZE_Y");
  auto mx = position.x;
  auto my = position.y;

  auto gridHeight = sy * (tile + grid);
  auto gridWidth = sx * (tile + grid);

  mx = mx - MGML()->gemsContainer->getPositionX();
  my = my - MGML()->gemsContainer->getPositionY();

  if (mx < 0) mx = 0;
  if (my < 0) my = 0;

  if (my > gridHeight) my = gridHeight;
  if (mx > gridWidth) mx = gridWidth;

  auto x = ceil((mx - tile * 0.5) / (tile + grid));
  auto y = ceil((my - tile * 0.5) / (tile + grid));

  if (x < 1) x = 1;
  if (y < 1) y = 1;
  if (x > grid) x = grid;
  if (y > grid) y = grid;

  return GemPos(x, y, MGML()->gridGemsColumnMap[x][y]);
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::selectStartGem(Gem *touchedGem) {

  if (MGML()->selectedGem == nullptr) {
    auto z2=CC_CSV(c::Integer, "Z_SWAP_2");
    MGML()->selectedGem = touchedGem.gem;
    MGML()->targetGem = CC_NIL;
    MGML()->targetIndex = CC_NIL;
    touchedGem->gem->setLocalZOrder(z2);
    MGML()->selectedIndex = f::Cell2P(touchedGem.x, touchedGem.y);

    MGML()->selectedGemPosition = f::Cell2P(touchedGem->gem->getPositionX(),
                                          touchedGem->gem->getPositionY());
    MGML()->gridAnimations->animateSelected(touchedGem.gem);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
function GridController::selectTargetGem (touchedGem) {

  auto z1=CC_CSV(c::Integer, "Z_SWAP_1");

  if (MGML()->targetGem != CC_NIL) {
    return;
  }

  this->enabled = false;

  MGML()->targetIndex = f::Cell2P(touchedGem.x, touchedGem.y);
  MGML()->targetGem = touchedGem.gem;
  MGML()->targetGem->sprite->setLocalZOrder(z1);
  MGML()->swapGemsToNewPosition();
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchDown(c::Touch *touch) {

  if (!MGMS()->isLive()) {
    cx::runEx(Game::reify(mc_new(f::GCX)));
    //("background.mp3")
    return;
  }

  this->touchDown = true;

  if (!this->enabled) return;

  GemPos touchedGem = this->findGemAtPosition(touch);
  if (touchedGem.gem != CC_NIL) {
      if (MGML()->selectedGem == CC_NIL) {
        selectStartGem(touchedGem);
      } else {
        if (isValidTarget(touchedGem.x, touchedGem.y, touch)) {
          selectTargetGem(touchedGem);
        } else {
          if (MGML()->selectedGem != CC_NIL) {
            MGML()->selectedGem->deselect();
          }
          MGML()->selectedGem = CC_NIL;
          this->selectStartGem (touchedGem);
        }
      }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchUp(c::Touch *touch) {
  if (!MGMS()->isLive()) { return; }
  this->touchDown = false;
  if (!this->enabled ) { return; }
  if (MGML()->selectedGem != CC_NIL) {
    MGML()->dropSelectedGem();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchMove(c::Touch *touch) {
  if (!MGMS()->isLive()){ return; }
  if (!this->enabled ) { return; }
  //track to see if we have a valid target
  if (MGML()->selectedGem != CC_NIL &&
      this->touchDown ) {
    auto loc= touch->getLocation();
    MGML()->selectedGem->sprite->setPosition(
      loc.x - MGMS()->gemsContainer->getPositionX(),
      loc.y - MGMS()->gemsContainer->getPositionY());

    auto touchedGem = this->findGemAtPosition(touch);
    if (touchedGem.gem != CC_NIL &&
        this->isValidTarget(touchedGem.x, touchedGem.y, touch) ) {
      this->selectTargetGem(touchedGem);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::isValidTarget(float px, float py, c::Touch *touch) {

  auto offbounds = false;

  if (px > MGML()->selectedIndex.x + 1) offbounds = true;
  if (px < MGML()->selectedIndex.x - 1) offbounds = true;
  if (py > MGML()->selectedIndex.y + 1) offbounds = true;
  if (py < MGML()->selectedIndex.y - 1) offbounds = true;

  auto cell = sin(atan2(pow(MGML()->selectedIndex.x - px, 2),
        pow(MGML()->selectedIndex.y- py, 2)));

  if (cell != 0 && cell != 1) {
    offbounds = true;
  }

  if (offbounds ) {
    return false;
  }

  auto touchedGem = MGML()->gridGemsColumnMap[px][py];

  if (touchedGem.gem == MGML()->selectedGem ||
      (px == MGML()->selectedIndex.x &&
       py == MGML()->selectedIndex.y)) {
      self.gameLayer.targetGem = nil
    return false;
  }

  return true;
}






NS_END



