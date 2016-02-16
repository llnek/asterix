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
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();
  void buildGrid(GVars*);

  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb=cx::visBox();

  ss->gridController = GridController::create();
  ss->gridAnimations = GridAnimations::create();
  ss->schedulerID = CC_NIL;
  ss->enabled=false;
  //ss->grid = {};
  //ss->gridGemsColumnMap = {};
  //ss->allGems = {};
  ss->gemsContainer = c::Node::create();
  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
  ss->selectedIndex = Cell2P(0, 0);
  ss->targetIndex = Cell2P(0, 0);
  ss->selectedGemPosition = Cell2P(0, 0);
  ss->combos = 0;
  ss->addingCombos = false;

  ss->gemsContainer->setPosition( 25, 80);
  addItem(ss->gemsContainer);

  //build interface
  auto frame = cx::loadSprite("frame.png");
  frame->setPosition(wb.cx, wb.cy);
  addItem(frame);


  buildGrid(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid(GVars *ss) {

  auto sx=CC_CSV(c::Integer, "GRID_SIZE_X");
  auto sy=CC_CSV(c::Integer, "GRID_SIZE_Y");
  auto tile=CC_CSV(c::Integer, "TILE_SIZE");
  auto grid=CC_CSV(c::Integer, "GRID_SIZE");

  ss->enabled = false;

  for (auto c = 0; c < sx; ++c) {
    auto gg= = new f::FArrInt(sy);
    ss->grid[c] =gg;
    ss->gridGemsColumnMap[c] = new f::FArrayPtr<Gem>(sy);
    for (auto r = 0; r < sy; ++r) {
      if (c < 2) {
        gg->set(r, getGemType(getVertUnique(c,r)));
      } else {
        gg->set(r, getGemType(getVertHorzUnique(c,r)));
      }

      auto g = Gem::create();
      g->setType(gg[r]);
      g->setPosition(c * (tile + grid), r * (tile + grid));

      ss->gemsContainer->addChild(g);
      ss->gridGemsColumnMap[c]->set(r,g);
      ss->allGems.push_back(g);
    }
  }

  ss->gridAnimations->animateIntro();
}

//////////////////////////////////////////////////////////////////////////////
//
int GLayer::getVerticalUnique(int col, int row) {

  auto type = cx::randInt(GEMSET_SIZE);
  assert(type >= 0 && type < GEMSET_SIZE);

  if (ss->grid[col][row-1] == TYPES[type] &&
      ss->grid[col][row-2] != CC_NIL &&
      ss->grid[col][row-2] == TYPES[type]) {
      type = type + 1;
    if (type >= GEMSET_SIZE) type = 0;
  }
  return type;
}

//////////////////////////////////////////////////////////////////////////////
//
int GLayer::getVerticalHorizontalUnique(int col, int row) {

  auto type = this->getVerticalUnique(col, row);
  auto ss=CC_GNLF(GVars,shared,"slots");

  if (ss->grid[col-1][row] == ss->TYPES[type] &&
      ss->grid[col-2][row] != CC_NIL &&
      ss->grid[col-2][row] == ss->TYPES[type]) {

    auto unique = false;
    while (!unique) {
      type = getVerticalUnique(col, row);
      if (ss->grid[col-1][row] == TYPES[type] &&
          ss->grid[col-2][row] != CC_NIL &&
          ss->grid[col-2][row] == TYPES[type]) {
        //do nothing
      } else {
        unique = true;
      }
    }
  }
  return type;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::swapGemsToNewPosition() {

  auto DPOINTS=CC_CSV(c::Integer,"DIAMOND_POINTS");
  auto sx=CC_CSV(c::Integer, "GRID_SIZE_X");
  auto POINTS=CC_CSV(c::Integer,"POINTS");
  auto ss=CC_GNLF(GVars,shared,"slots");

  auto onMatchedAnimatedOut= [=](sender) {
    this->collapseGrid();
  };

  auto onReturnSwapComplete= [=](sender) {
    ss->gridController->enabled = true;
  };

  auto onNewSwapComplete= [=](sender) {
    ss->gridGemsColumnMap[ss->targetIndex.x][ss->targetIndex.y] = ss->selectedGem;
    ss->gridGemsColumnMap[ss->selectedIndex.x][ss->selectedIndex.y] = ss->targetGem;

    ss->grid[ss->targetIndex.x][ss->targetIndex.y] = ss->selectedGem->type;
    ss->grid[ss->selectedIndex.x][ss->selectedIndex.y] = ss->targetGem->type;

    ss->addingCombos = true;
    ss->combos = 0;

    //check for new matches
    if (ss->gridController->checkGridMatches()) {
      //animate matched gems
      if (ss->gridController->matchArray.size() > 3) {
        ss->combos = ss->combos +
          (ss->gridController->matchArray.size() - 3);
      }
      ss->gridAnimations->animateMatches(ss->gridController->matchArray, onMatchedAnimatedOut);
      showMatchParticle(ss->gridController->matchArray);
      setGemsScore(ss->gridController->matchArray * POINTS);
      cx::sfxPlay("match2");
    } else {
      //no matches, swap gems back
      ss->gridAnimations->swapGems(
          ss->targetGem, ss->selectedGem, onReturnSwapComplete);

      ss->gridGemsColumnMap[ss->targetIndex.x][ss->targetIndex.y] = ss->targetGem;
      ss->gridGemsColumnMap[ss->selectedIndex.x][ss->selectedIndex.y] = ss->selectedGem;

      ss->grid[ss->targetIndex.x][ss->targetIndex.y] = ss->targetGem->type;
      ss->grid[ss->selectedIndex.x][ss->selectedIndex.y] = ss->selectedGem->type;
      cx::sfxPlay("wrong");
    }

    ss->selectedGem = CC_NIL;
    ss->targetGem = CC_NIL;
  };

  ss->gridAnimations->swapGems(
      ss->selectedGem, ss->targetGem, onNewSwapComplete);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::collapseGrid() {

  auto DPOINTS=CC_CSV(c::Integer,"DIAMOND_POINTS");
  auto POINTS=CC_CSV(c::Integer,"POINTS");
  auto grid=CC_CSV(c::Integer, "GRID_SPACE");
  auto tile=CC_CSV(c::Integer, "TILE_SIZE");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto sx=CC_CSV(c::Integer, "GRID_SIZE_X");

  auto onGridCollapseComplete= [=](sender) {

    auto onMatchedAnimatedOut= [=](sender) {
      this->collapseGrid();
    };

    F__LOOP(it,ss->allGems) {
      auto gem = *it;
      auto pos= gem->pos();
      auto xIndex = ceil((pos.x - tile * 0.5f) / (tile + grid));
      auto yIndex = ceil((pos.y - tile * 0.5f) / (tile + grid));
      ss->gridGemsColumnMap[xIndex][yIndex] = gem;
      ss->grid[xIndex][yIndex] = gem->type;
    }

    if (ss->gridController->checkGridMatches()) {
     //animate matched games
     if (ss->addingCombos) {
       if (ss->gridController->matchArray.size() > 3) {
         ss->combos += ss->gridController->matchArray.size() - 3;
       }
     }
     ss->gridAnimations->animateMatches(
         ss->gridController->matchArray, onMatchedAnimatedOut);
     this->showMatchParticle(ss->gridController->matchArray);
     this->setGemsScore(ss->gridController->matchArray * POINTS);
     cx::sfxPlay("match");
    } else {
      //no more matches, check for combos
      if (self.combos > 0) {
        //now turn random gems into diamonds
        local diamonds = {}
        local removeGems = {}
        int i = 0;
        while (i < ss->combos) {
          ++i;

          local randomGem = nil
          local randomX,randomY = 0

          while (randomGem == CC_NIL) {
            randomX = cx::randInt(GRID_SIZE_X);
            randomY = cx::randInt(GRID_SIZE_Y);
            randomGem = ss->gridGemsColumnMap[randomX][randomY];
            if (randomGem.type == TYPE_GEM_WHITE) { randomGem = CC_NIL; }
          }

          auto pool= MGMS()->getPool("Diamonds");
          auto diamond = pool->get();
          diamond->setPos(randomGem->getPositionX(), randomGem->getPositionY());

          auto diamondParticle = self.objectPools:getDiamondParticle()
          diamondParticle->setPos(randomGem->getPositionX(), randomGem->getPositionY());

          table.insert(diamonds, diamond);
          table.insert(removeGems, {x=randomX, y=randomY});
        }
        this->setDiamondScore(diamonds.size() * DPOINTS);
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

  F__LOOP(it, ss->gridController->matchArray {
    ss->grid[ss->gridController->matchArray[i].x][self.gridController.matchArray[i].y] = -1
  }

  int newColumn = -1;
  int column = -1;
  int i;

  for (auto c = 0; c < sx; ++c) {
    column = ss->grid[c];
    newColumn = {}
    i = 1
    while (newColumn.size() < column.size()) {
      if (column.size() > i) {
        if (column[i] != -1) {
            //move gem
          table.insert(newColumn, column[i]);
        }
      } else {
          //create new gem
        table.insert(newColumn, 1, column[i])
      }
      i += 1;
    }
    ss->grid[c] = newColumn;
  }

  ss->gridAnimations->animateCollapse(onGridCollapseComplete);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::dropSelectedGem () {
  auto Z_GRID=CC_CSV(c::Integer, "Z_GRID");
  this->selectedGem->sprite->setLocalZOrder(Z_GRID);
  this->gridAnimations:resetSelectedGem();
}

//////////////////////////////////////////////////////////////////////////////
//
int GLayer:getNewGem () {
  return cx::randInt(TYPES);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer:addToScore() {
  F__LOOP(it, ss->gridController.matchArray) {
    auto position = ss->gridController.matchArray[i];
    auto gem = this->gridGemsColumnMap[position.x][position.y];
    if (gem->type == TYPE_GEM_WHITE) {
        //got a diamond
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer:showMatchParticle(matches) {
  auto p=MGMS()->getPool("MatchParticles");
  F__LOOP(it, matches) {
    auto gem = gridGemsColumnMap[matches[i].x][matches[i].y];
    auto particle = p->get();
    particle->setPos(gem->sprite->getPositionX() +
        ss->gemsContainer->getPositionX(),
        gem->sprite->getPositionY() + ss->gemsContainer->getPositionY());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::setDiamondScore(int value) {
  getHUD()->updateDiamondScore(value);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer:setGemsScore(int value) {
  getHUD()->updateGemScore(value);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::tick() {
  auto scaleNow = timeBar->getScaleX();
  auto speed = 0.007;
  if (scaleNow - speed > 0) {
    timeBar->setScaleX(scaleNow - speed);
  } else {
    //GameOver!!!
    timeBar->setScaleX(0);
    unscheduleScriptEntry(self.schedulerID);
    cx::pauseAudio();
    MGMS()->stop();
    // show game over
    auto gameOver = cx::loadSprite("gameOver.png");
    gameOver->setPos(wb.cx, wb.cy);
    addItem(gameOver);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer:startTimer() {

  auto timeBarBg = cx::loadSprite("timeBarBg.png");
  auto wb=cx::visBox();

  timeBarBg->setPosition(wb.cx, 40);
  addItem(timeBarBg);

  auto timeBar = cx::loadSprite("timeBar.png");
  timeBar->setAnchorPoint(cx::anchorL());
  timeBar->setPosition(wb.cx - 290, 40);
  addItem(timeBar);

  scheduleScriptFunc(tick, 1, false);
}


//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {

  engine = mc_new(GEngine);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




