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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "lib.h"
#include "Gem.h"

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
  assert(idx >= 0 && idx < GEMSET_SIZE);
  return idx;
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr getGemPngPath(int type) {
  return "pics/" + getGemPng(type);
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
  assert(col >=0 && col < GRID_SIZE_X);
  assert(row >=0 && row < GRID_SIZE_Y);
  auto t = cx::randInt(GEMSET_SIZE);

  if (col >= 0 && row > 1 &&
      ss->grid[col]->get(row-1) == t &&
      ss->grid[col]->get(row-2) == t) {
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
      ss->grid[col-1]->get(row) == t &&
      ss->grid[col-2]->get(row) == t) {
    auto unique = false;
    while (!unique) {
      t = getVerticalUnique(ss, col, row);
      if (ss->grid[col-1]->get(row) == t &&
          ss->grid[col-2]->get(row) == t)
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
void dropSelectedGem(GVars *ss, GridAnimations *anim) {
  ss->selectedGem->node->setLocalZOrder(Z_GRID);
  anim->resetSelectedGem();
}

//////////////////////////////////////////////////////////////////////////////
//
void onNewSwapComplete(GVars *ss, GridController *ctrl, GridAnimations *anim) {

  ss->gridGemsColumnMap[ss->selectedIndex.x]->set(
      ss->selectedIndex.y, ss->targetGem);

  ss->gridGemsColumnMap[ss->targetIndex.x]->set(
      ss->targetIndex.y, ss->selectedGem);

  ss->grid[ss->selectedIndex.x]->set(
      ss->selectedIndex.y, ss->targetGem->type);
  ss->grid[ss->targetIndex.x]->set(
      ss->targetIndex.y, ss->selectedGem->type);

  ss->addingCombos = true;
  ss->combos = 0;

  //check for new matches
  if (ctrl->checkGridMatches()) {

    //animate matched gems
    if (ss->matchArray.size() > 3) {
      ss->combos += ss->matchArray.size() - 3;
    }

    anim->animateMatches(
        ss->matchArray,
        [=]() {
         collapseGrid(ss);
        });

    showMatchParticle(ss->matchArray);

    auto msg= j::json({
        {"score", (int) ss->matchArray.size() * POINTS },
        {"type", "match2"}
        });
    SENDMSGEX("/game/player/earnscore", &msg);
    cx::sfxPlay("match2");

  } else {

    //no matches, swap gems back
    anim->swapGems(
        ss->targetGem,
        ss->selectedGem,
        [=]() { ctrl->enabled = true; });

    ss->gridGemsColumnMap[ss->selectedIndex.x]->set(
        ss->selectedIndex.y, ss->selectedGem);
    ss->gridGemsColumnMap[ss->targetIndex.x]->set(
        ss->targetIndex.y, ss->targetGem);

    ss->grid[ss->selectedIndex.x]->set(
        ss->selectedIndex.y, ss->selectedGem->getType());
    ss->grid[ss->targetIndex.x]->set(
        ss->targetIndex.y, ss->targetGem->getType());

    cx::sfxPlay("wrong");
  }

  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
}

//////////////////////////////////////////////////////////////////////////////
//
void swapGemsToNewPosition(GVars *ss, GridAnimations *anim) {

  anim->swapGems(
      ss->selectedGem,
      ss->targetGem,
      [=]() { onNewSwapComplete(ss)});
}

//////////////////////////////////////////////////////////////////////////////
//
void onGridCollapseComplete(GVars *ss, GridController *ctrl, GridAnimations *anim) {

  F__LOOP(it,ss->allGems) {
    auto gem = *it;
    auto pos=gem->pos();
    auto yIndex = ceil((pos.y - TILE_SIZE * 0.5f)/(TILE_SIZE+GRID_SPACE));
    auto xIndex = ceil((pos.x - TILE_SIZE * 0.5f)/(TILE_SIZE+GRID_SPACE));
    ss->gridGemsColumnMap[xIndex]->set(yIndex, gem);
    ss->grid[xIndex]->set(yIndex, gem->getType());
  }

  if (ctrl->checkGridMatches()) {

     //animate matched games
   if (ss->addingCombos) {
     if (ss->matchArray.size() > 3) {
       ss->combos += (ss->matchArray.size() - 3);
     }
   }

   anim->animateMatches(
       ss->matchArray,
       [=]() { collapseGrid(ss); });

   showMatchParticle(ss->matchArray);

   auto msg= j::json({
       {"score", (int)ss->matchArray.size() * POINTS },
       {"type", "match"}
       });
   SENDMSGEX("/game/player/earnscore",&msg);

   cx::sfxPlay("match");

  } else {

    //no more matches, check for combos
    if (ss->combos > 0) {
      //now turn random gems into diamonds
      s_vec<f::Cell2I>removeGems;
        s_vec<f::ComObj*> diamonds;
      int i = 0;

      auto p1=MGMS()->getPool("DiamondParticles");
      auto p2=MGMS()->getPool("Diamonds");

      //math.randomseed(os.clock())

      while (i < ss->combos) {
        ++i;
        Gem *randomGem = CC_NIL;
        int randomY = 0;
        int randomX=0;

        while (ENP(randomGem)) {
          randomX = cx::randInt(GRID_SIZE_X);
          randomY = cx::randInt(GRID_SIZE_Y);
          randomGem = ss->gridGemsColumnMap[randomX]->get(randomY);
          if (randomGem->getType() == TYPE_GEM_WHITE) { randomGem = CC_NIL; }
        }

        auto diamondParticle = p1->getAndSet(true);
        auto diamond = p2->getAndSet(true);
        auto pos= randomGem->pos();

        diamondParticle->node->setPosition(pos.x, pos.y);
        diamond->node->setPosition(pos.x,pos.y);

        removeGems.push_back(f::Cell2I(randomX, randomY));
        diamonds.push_back(diamond);
      }

      auto msg= j::json({
          {"score", (int)diamonds.size() * DIAMOND_POINTS },
          {"type", "diamond"}
      });
      SENDMSGEX("/game/player/earnscore", &msg);

      anim->animateMatches(
          removeGems,
          [=]() { collapseGrid(ss); });

      anim->collectDiamonds(diamonds);
      ss->combos = 0;
      cx::sfxPlay("diamond");

    } else {

      ctrl->enabled = true;
    }

    ss->addingCombos = false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void collapseGrid(GVars *ss, GridAnimations *anim) {

  F__LOOP(it, ss->matchArray) {
    auto &z= *it;
    ss->grid[z.x]->set(z.y, -1);
  }

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
  anim->animateCollapse(
      [=](c::Ref *r) {
        onGridCollapseComplete(ss,r);
      });

}

//////////////////////////////////////////////////////////////////////////////
//
int getNewGem() {
  return cx::randInt(GEMSET_SIZE);
}

//////////////////////////////////////////////////////////////////////////////
//
void showMatchParticle(const s_vec<f::Cell2I> &matches) {
  auto p= MGMS()->getPool("MatchParticles");
  F__LOOP(it,matches) {
    auto &pos= *it;
    auto gem = ss->gridGemsColumnMap[pos.x]->get(pos.y);
    auto particle = p->getAndSet(true);
    particle->node->setPos(
        gem->node->getPositionX() + ss->gemsContainer->getPositionX(),
        gem->node->getPositionY() + ss->gemsContainer->getPositionY());
  }
}



NS_END




