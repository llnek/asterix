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
#include "lib.h"
#include "C.h"
#include "Game.h"
#include "GridAnimations.h"
#include "GridController.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

static int TILE_GRID= TILE_SIZE+GRID_SPACE;

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::checkGridMatches() {

  ss->matchArray.clear();
  auto rc=false;

  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    for (auto r = 0; r < GRID_SIZE_Y; ++r) {
      checkTypeMatch(c,r);
    }
  }

  if (ss->matchArray.size() >= 2) {
    SENDMSG("/game/player/addscore");
    rc= true;
  } else {
    CCLOG("no matches");
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::checkTypeMatch(int c, int r) {

    auto type = ss->grid[c]->get(r);
  auto stepC = c;
  auto stepR = r;
  s_vec<f::Cell2I> temp;

  //check top
  while (stepR -1 >= 0 &&
    ss->grid[c]->get(stepR-1) == type) {
    --stepR;
    temp.push_back(f::Cell2I( c, stepR));
  }

    if (temp.size() >= 2) { addMatches(temp); }

    //check bottom
    temp.clear();
    stepR = r;
    while (stepR + 1 < GRID_SIZE_Y && ss->grid[c]->get(stepR + 1) == type) {
      ++stepR;
      temp.push_back(f::Cell2I(c, stepR));
    }

    if (temp.size() >= 2) { addMatches(temp); }

    //check left
    temp.clear();
    while (stepC - 1 >= 0 && ss->grid[stepC - 1]->get(r) == type) {
      --stepC;
      temp.push_back(f::Cell2I(stepC, r));
    }

    if (temp.size() >= 2) { addMatches(temp); }

    //check right
    temp.clear();
    stepC = c;
    while (stepC + 1 < GRID_SIZE_X && ss->grid[stepC + 1]->get(r) == type) {
      ++stepC;
      temp.push_back(f::Cell2I(stepC,  r));
    }

    if (temp.size() >= 2) { addMatches(temp); }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::addMatches(const s_vec<f::Cell2I> &matches) {
  F__LOOP(it, matches) {
    auto &m= *it;
    if (! this->find(m,ss->matchArray)) {
      ss->matchArray.push_back(m);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::find (const f::Cell2I &np, const s_vec<f::Cell2I> &arr) {
  F__LOOP(it, arr) {
    auto &a= *it;
    if (a.x == np.x and a.y == np.y) return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
GemInfo GridController::findGemAtPosition(const c::Vec2 &position) {

  auto mx = position.x - ss->gemsContainer->getPositionX();
  auto my = position.y - ss->gemsContainer->getPositionY();
  auto gridHeight = GRID_SIZE_Y * TILE_GRID;
  auto gridWidth = GRID_SIZE_X * TILE_GRID;

  if (mx < 0) mx = 0;
  if (my < 0) my = 0;

  if (my > gridHeight) my = gridHeight;
  if (mx > gridWidth) mx = gridWidth;

  auto x = ceil((mx - TILE_SIZE * 0.5f) / TILE_GRID);
  auto y = ceil((my - TILE_SIZE * 0.5f) / TILE_GRID);

  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x >= GRID_SIZE_X) x = GRID_SIZE_X-1;
  if (y >= GRID_SIZE_Y) y = GRID_SIZE_Y-1;

  return GemInfo(x, y , ss->gridGemsColumnMap[x]->get(y));
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::selectStartGem(const GemInfo &touchedGem) {
  if (ENP(ss->selectedGem)) {
    ss->selectedGem = touchedGem.gem;
    ss->targetGem = CC_NIL;
      ss->targetIndex = f::Cell2I();
    touchedGem.gem->node->setLocalZOrder(Z_SWAP_2);
    ss->selectedIndex = f::Cell2I(touchedGem.x, touchedGem.y);
    ss->selectedGemPosition = touchedGem.gem->pos();
    anim->animateSelected(touchedGem.gem);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::selectTargetGem(const GemInfo &touchedGem) {
  if (ss->targetGem != CC_NIL) return;
  this->enabled = false;
  ss->targetIndex = f::Cell2I(touchedGem.x, touchedGem.y);
  ss->targetGem = touchedGem.gem;
  ss->targetGem->node->setLocalZOrder(Z_SWAP_1);
  swapGemsToNewPosition(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchDown(const c::Vec2 &touch) {

  if (!MGMS()->isLive()) {
    cx::runEx(Game::reify(mc_new(f::GCX)));
    //fullPathForFilename("background.mp3")
    return;
  }

  this->touchDown = true;
  if (!this->enabled ) { return; }

  auto touchedGem = findGemAtPosition(touch);
  if (touchedGem.gem != CC_NIL ) {
    if (ENP(ss->selectedGem)) {
      selectStartGem(touchedGem);
    } else {
      if (isValidTarget(touchedGem.x, touchedGem.y, touch) ) {
        selectTargetGem(touchedGem);
      }
      else {
        if (ss->selectedGem != CC_NIL) {
          ss->selectedGem->deselect();
        }
        ss->selectedGem = CC_NIL;
        selectStartGem (touchedGem);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchUp(const c::Vec2 &touch) {
  if (!MGMS()->isLive()) { return; }
  this->touchDown = false;
  if (!this->enabled) { return; }
  if (ss->selectedGem != CC_NIL) { dropSelectedGem(ss); }
}

//////////////////////////////////////////////////////////////////////////////
//
void GridController::onTouchMove(const c::Vec2 &touch) {

  if (!MGMS()->isLive()) { return; }
  if (!this->enabled ) { return; }

  //track to see if we have a valid target
  if (ss->selectedGem != CC_NIL && this->touchDown ) {
    ss->selectedGem->setPos(
    touch.x - ss->gemsContainer->getPositionX(),
    touch.y - ss->gemsContainer->getPositionY());
    auto touchedGem = findGemAtPosition(touch);
    if (touchedGem.gem != CC_NIL &&
        isValidTarget(touchedGem.x, touchedGem.y, touch)) {
      selectTargetGem(touchedGem);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GridController::isValidTarget(int px, int py, const c::Vec2 &touch) {

  auto offbounds = false;
  auto rc=true;

  if (px > ss->selectedIndex.x + 1) offbounds = true;
  if (px < ss->selectedIndex.x - 1) offbounds = true;
  if (py > ss->selectedIndex.y + 1) offbounds = true;
  if (py < ss->selectedIndex.y - 1) offbounds = true;

  auto cell = sin(atan2(pow(ss->selectedIndex.x - px, 2), pow( ss->selectedIndex.y- py, 2)));
  if (cell != 0 && cell != 1) {
    offbounds = true;
  }

  if (offbounds ) {
    return false;
  }

    auto touchedGem = ss->gridGemsColumnMap[px]->get(py);
  if (touchedGem == ss->selectedGem ||
      (px == ss->selectedIndex.x &&
       py == ss->selectedIndex.y)) {
    ss->targetGem = CC_NIL;
    rc=false;
  }

  return rc;
}


NS_END


