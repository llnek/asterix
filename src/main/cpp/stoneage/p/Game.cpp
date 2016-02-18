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
//
void GLayer::postReify() {

  shared=engine->getNodeList(SharedNode().typeId());
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb= cx::vixBox();

  ss->gridController = GridController:create();
  ss->gridAnimations = GridAnimations:create();

  ss->selectedGemPosition = f::Cell2P();
  ss->gemsContainer = c::Node:create();
  ss->selectedIndex = f::Cell2P();
  ss->targetIndex = f::Cell2P();
  ss->schedulerID = CC_NIL;
  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
  ss->combos = 0;
  ss->addingCombos = false;

  ss->gemsContainer->setPosition( 25, 80);
  addItem(ss->gemsContainer);

  auto frame = cx::loadSprite("frame.png");
  frame->setPosition(wb.cx, wb.cy);
  addItem(frame);

  buildGrid();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  centerImage("game.bg");
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchDown(t->getLocation());
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchMove(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchUp(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid () {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->enabled = false;
  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto m= new f::FArrayPtr<Gem>(GRID_SIZE_Y);
    auto g= mc_new1(f::FArrInt,GRID_SIZE_Y);

    ss->gridGemsColumnMap.push_back(m);
    ss->grid.push_back(g);

    for (auto r = 0; r < GRID_SIZE_Y; ++r) {
      auto gem = Gem:create();
      auto idx= c<2
        ? getVerticalUnique(c,r) : getVerticalHorizontalUnique(c,r);
      auto t= getGemType(idx);

      gem->setType(t);
      g->set(r,t);

      gem->setPosition(
          c*(TILE_SIZE + GRID_SPACE),
          r*(TILE_SIZE + GRID_SPACE));

      ss->gemsContainer->addChild(gem);
      m->set(r,gem);
      ss->allGems.push_back(gem);
    }
  }
  ss->gridAnimations->animateIntro();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::dropSelectedGem() {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->selectedGem->node->setLocalZOrder(Z_GRID);
  ss->gridAnimations->resetSelectedGem();
}

//////////////////////////////////////////////////////////////////////////////
//
int GLayer::getNewGem() {
  return cx::randInt(GEMSET_SIZE);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addToScore() {
  F__LOOP(it, ss->gridController->matchArray) {
    auto position = *it;
    auto gem = ss->gridGemsColumnMap[position.x][position.y];
    if (gem->type == TYPE_GEM_WHITE) {
        //got a diamond
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMatchParticle(const s_vec<f::Cell2P> &matches) {
  auto p= MGMS()->getPool("MatchParticles");
  F__LOOP(it,matches) {
    auto &pos= *it;
    auto gem = ss->gridGemsColumnMap[pos.x][pos.y];
    auto particle = p->get(true);
    particle->node->setPosition(
        gem->node->getPositionX() + ss->gemsContainer->getPositionX(),
        gem->node->getPositionY() + ss->gemsContainer->getPositionY());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startTimer() {

    local timeBarBg = cc.Sprite:create("timeBarBg.png")
    timeBarBg:setPosition(self.middle.x, 40)
    self:addChild(timeBarBg)

    local timeBar = cc.Sprite:create("timeBar.png")
    timeBar:setAnchorPoint(cc.p(0,0.5))
    timeBar:setPosition(self.middle.x - 290, 40)
    self:addChild(timeBar)
    this->schedule(CC_SCHEDULE_SELECTOR(GLayer::tick), 1);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::tick() {
  auto scaleNow = timeBar->getScaleX();
  auto speed = 0.007f;
  if (scaleNow - speed > 0) {
    timeBar->setScaleX(scaleNow - speed);
  } else {
    this->unschedule(CC_SCHEDULE_SELECTOR(GLayer::tick));
      //GameOver!!!
    timeBar->setScaleX(0);
    cx::pauseAudio();
    MGMS()->stop();
    // show game over
    auto gameOver = cx::loadSprite("gameOver.png");
    gameOver->setPosition(wb.cx, wb.cy);
    addItem(gameOve);
  }
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


