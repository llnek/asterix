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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

static s_arr<sstr,GEMSET_SIZE> SKINS = {
  "gem_blue.png",
  "gem_yellow.png",
  "gem_red.png",
  "gem_green.png",
  "gem_pink.png",
  "gem_white.png"
};

//////////////////////////////////////////////////////////////////////////////
//
int getGemType(int idx) {

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr getGemPng(int type) {
  assert(type >= 0 && type < GEMSET_SIZE);
  return SKINS[type];
}

//////////////////////////////////////////////////////////////////////////////
//
int getVerticalUnique(GVars *ss, int col, int row) {
  auto t = cx::randInt(GEMSET_SIZE);

  if (col >= 0 && row > 1 &&
      ss->grid[col][row-1] == t &&
      ss->grid[col][row-2] == t) {
      t += 1;
      if (t >= GEMSET_SIZE) { t=0; }
  }
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
int getVerticalHorizontalUnique(GVars *ss, int col, int row) {

  auto t= getVerticalUnique(ss, col, row);

  if (col > 1 && row >= 0 &&
      ss->grid[col-1][row] == t &&
      ss->grid[col-2][row] == t) {
    auto unique = false;
    while (!unique) {
      t = getVerticalUnique(ss, col, row);
      if (ss->grid[col-1][row] == t &&
          ss->grid[col-2][row] == t)
      {}
      else {
       unique = true;
      }
    }
  }
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
void dropSelectedGem(GVars *ss) {
  ss->selectedGem->node->setLocalZOrder(Z_GRID);
  ss->gridAnimations->resetSelectedGem();
}

//////////////////////////////////////////////////////////////////////////////
//
void swapGemsToNewPosition(GVars *ss) {

  auto onMatchedAnimatedOut=[=](sender) {
    collapseGrid(ss);
  };

  auto onReturnSwapComplete= [=](sender) {
    ss->gridController->enabled = true;
  };

  auto onNewSwapComplete=[=](sender) {

    ss->gridGemsColumnMap[ss->selectedIndex.x][ss->selectedIndex.y] = ss->targetGem;
    ss->gridGemsColumnMap[ss->targetIndex.x][ss->targetIndex.y] = ss->selectedGem;

    ss->grid[ss->selectedIndex.x][ss->selectedIndex.y] = ss->targetGem->type;
    ss->grid[ss->targetIndex.x][ss->targetIndex.y] = ss->selectedGem->type;

    ss->addingCombos = true;
    ss->combos = 0;

    //check for new matches
    if (ss->gridController->checkGridMatches()) {
      //animate matched gems
      if (ss->gridController->matchArray.size() > 3) {
        ss->combos += ss->gridController->matchArray.size() - 3;
      }

      ss->gridAnimations->animateMatches(
          ss->gridController->matchArray, onMatchedAnimatedOut);

      ss->showMatchParticle(ss->gridController->matchArray);

      ss->setGemsScore(ss->gridController->matchArray * POINTS);

      cx::sfxPlay("match2");

    } else {
      //no matches, swap gems back
      ss->gridAnimations->swapGems(ss->targetGem, ss->selectedGem, onReturnSwapComplete);

      ss->gridGemsColumnMap[ss->selectedIndex.x][ss->selectedIndex.y] = ss->selectedGem;
      ss->gridGemsColumnMap[ss->targetIndex.x][ss->targetIndex.y] = ss->targetGem;

      ss->grid[ss->selectedIndex.x][ss->selectedIndex.y] = ss->selectedGem.type;
      ss->grid[ss->targetIndex.x][ss->targetIndex.y] = ss->targetGem.type;

      cx::sfxPlay("wrong");
    }

    ss->selectedGem = CC_NIL;
    ss->targetGem = CC_NIL;
  };

  ss->gridAnimations->swapGems(ss->selectedGem, ss->targetGem, onNewSwapComplete);
}

//////////////////////////////////////////////////////////////////////////////
//
void onGridCollapseComplete(GVars *ss) {

  auto onMatchedAnimatedOut = [=](sender) {
    collapseGrid();
  };

  F__LOOP(it,ss->allGems) {
    auto gem = *it;
    auto xIndex = ceil((gem->getPositionX() - TILE_SIZE * 0.5f)/(TILE_SIZE + GRID_SPACE));
    auto yIndex = ceil((gem->getPositionY() - TILE_SIZE * 0.5f)/(TILE_SIZE + .GRID_SPACE));
    ss->gridGemsColumnMap[xIndex]->set(yIndex, gem);
    ss->grid[xIndex]->set(yIndex, gem->type);
  }

  if (ss->gridController->checkGridMatches()) {

     //animate matched games
   if (ss->addingCombos) {
     if (ss->gridController->matchArray.size() > 3) {
       ss->combos += (ss->gridController->matchArray.size() - 3);
     }
   }

   ss->gridAnimations->animateMatches(ss->gridController->matchArray, onMatchedAnimatedOut);
   ss->showMatchParticle(ss->gridController->matchArray);

   auto msg= j::json({
       "score", (int)ss->gridController->matchArray.size() * POINTS
       });
   SENDMSGEX("/game/player/earnscore",&msg);

   cx::sfxPlay("match");

  } else {

    //no more matches, check for combos
    if (ss->combos > 0) {
      //now turn random gems into diamonds
      s_vec<f::Cell2P>removeGems;
      s_vec<Gem*> diamonds;
      int i = 0;

      auto p1=MGMS()->getPool("DiamondParticles");
      auto p2=MGMS()->getPool("Diamonds");

      //math.randomseed(os.clock())

      while (i < ss->combos) {
        ++i;
        Gem *randomGem = CC_NIL;
        int randomX=0;
        int randomY = 0;

        while (ENP(randomGem)) {
          randomX = cx::randInt(GRID_SIZE_X);
          randomY = cx::randInt(GRID_SIZE_Y);
          randomGem = ss->gridGemsColumnMap[randomX][randomY];
          if (randomGem->type == TYPE_GEM_WHITE) { randomGem = CC_NIL; }
        }

        auto diamondParticle = p1->get(true);
        auto diamond = p2->get(true);

        diamond->node->setPosition(randomGem->node->getPositionX(),
            randomGem->node->getPositionY());

        diamondParticle->node->setPosition(
            randomGem->node->getPositionX(),
            randomGem->node->getPositionY());

        diamonds.push_back(diamond);
        removeGems.push_back(f::Cell2P(randomX, randomY));
      }

      auto msg= j::json({
          {"score", (int)diamonds.size() * DIAMOND_POINTS},
          {"type", "diamond"}
      });
      SENDMSGEX("/game/player/earnscore", &msg);

      ss->gridAnimations->animateMatches(removeGems, onMatchedAnimatedOut);
      ss->gridAnimations->collectDiamonds(diamonds);
      ss->combos = 0;
      cx::sfxPlay("diamond");

    } else {

      ss->gridController->enabled = true;
    }

    ss->addingCombos = false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void collapseGrid(GVars *ss) {

  F__LOOP(it, ss->gridController->matchArray) {
    auto &z= *it;
    ss->grid[z.x]->set(z.y, -1);
  }

  local column = nil
  local newColumn = nil
  local i

  F__LOOP(it,ss->grid) {
    s_vec<int> nc;
    auto gc= *it;
    int v;
    int i=0;
    while (nc.size() < gc->size()) {
      if (gc->size() > i) {
        if (gc->get(i) != -1) {
            //move gem
            nc.push_back(gc->get(i));
        }
      } else {
        //create new gem
        nc.insert(nc.begin(), gc->getLast());//gc->get(i));
      }
      i += 1;
    }
    assert(nc.size() == gc->size());
    for (i=0; i < gc->size(); ++i) {
      gc->set(i, nc[i]);
    }
  }
  ss->gridAnimations->animateCollapse(
      [=](c::Ref *r) {
        onGridCollapseComplete(ss,r);
      });

}



NS_END




