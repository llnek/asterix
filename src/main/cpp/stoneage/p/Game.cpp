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

  DECL_PTR(a::NodeList, shared)
};

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss=CC_GNLF(GVars,shared,"slots");

  ss->gridController = GridController::create();
  ss->gridAnimations = GridAnimations::create();
  ss->schedulerID = CC_NIL;
  ss->enabled=false;
  ss->grid = {};
  ss->gridGemsColumnMap = {};
  ss->allGems = {};
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
    self.grid[c] = {}
    self.gridGemsColumnMap[c] = {}
    for (auto r = 0; r < sy; ++r) {
      if (c < 2) {
        ss->grid[c][r] = TYPES[ getVerticalUnique(c,r) ];
      } else {
        ss->grid[c][r] = TYPES[ getVerticalHorizontalUnique(c,r) ];
      }

      auto g = Gem::create();
      g->setType(  ss->grid[c][r] );
      g->setPos( c * (tile + grid), r * (tile + grid));

      ss->gemsContainer->addChild(g);
      ss->gridGemsColumnMap[c][r] = g;
      table.insert(self.allGems, g);
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




