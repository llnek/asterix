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

#include "n/GridAnimations.h"
#include "n/GridController.h"
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

  void buildGrid(GVars*,GridAnimations*);
  void addToScore();
  void startTimer();
  void tick();

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  DECL_PTR(a::NodeList, shared)
  DECL_PTR(a::NodeList, gcn)
  DECL_PTR(c::Sprite, timerBar)
};


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {

  shared=engine->getNodeList(SharedNode().typeId());
  gcn=engine->getNodeList(GridCtrlNode().typeId());

  auto anim= CC_GNLF(GridAnimations,ctrl,"anim");
  auto g= CC_GNLF(GridController,ctrl,"ctrl");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb= cx::vixBox();

  ss->selectedGemPosition = f::Cell2I();
  ss->gemsContainer = c::Node:create();
  ss->selectedIndex = f::Cell2I();
  ss->targetIndex = f::Cell2I();
  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
  ss->combos = 0;
  ss->addingCombos = false;

  ss->gemsContainer->setPosition( 25, 80);
  addItem(ss->gemsContainer);

  auto frame = cx::loadSprite("frame.png");
  frame->setPosition(wb.cx, wb.cy);
  addItem(frame);

  buildGrid(ss,anim);
  this->motionees.push_back(g);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  engine= mc_new(GEngine);
  centerImage("game.bg");
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *t) {
  auto ctrl=CC_GNLF(GridController,gcn,"ctrl");
  ctrl->onTouchDown(t->getLocation());
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(f::ComObj *co, c::Touch *t) {
  auto ctrl=CC_GNLF(GridController,gcn,"ctrl");
  ctrl->onTouchMove(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *t) {
  auto ctrl=CC_GNLF(GridController,gcn,"ctrl");
  ctrl->onTouchUp(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid(GVars *ss, GridAnimations *anim) {
  auto TILEGRID = TILE_SIZE + GRID_SPACE;
  ss->enabled = false;
  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto m= new f::FArrayPtr<Gem>(GRID_SIZE_Y);
    auto g= mc_new1(f::FArrInt,GRID_SIZE_Y);

    ss->gridGemsColumnMap.push_back(m);
    ss->grid.push_back(g);

    for (auto r = 0; r < GRID_SIZE_Y; ++r) {
      auto gem = Gem::create();
      auto idx= c<2
        ? getVerticalUnique(c,r)
        : getVerticalHorizontalUnique(c,r);
      auto t= getGemType(idx);

      gem->setType(t);
      g->set(r,t);
      gem->inflate( c*TILEGRID, r*TILEGRID);

      ss->gemsContainer->addChild(gem);
      m->set(r,gem);
      ss->allGems.push_back(gem);
    }
  }

  anim->animateIntro();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addToScore() {
  auto ss=CC_GNLF(GVars,shared,"slots");
  F__LOOP(it, ss->matchArray) {
    auto position = *it;
    auto gem = ss->gridGemsColumnMap[position.x][position.y];
    if (gem->getType() == TYPE_GEM_WHITE) {
        //got a diamond
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startTimer() {

  auto timeBarBg = c::loadSprite("timeBarBg.png");
  auto wb = cx::visBox();

  timeBarBg->setPosition(wb.cx, 40);
  addItem(timeBarBg);

  timeBar = cx::loadSprite("timeBar.png");
  timeBar->setAnchorPoint(cx::anchorL());
  timeBar->setPosition(wb.cx - 290, 40);
  addItem(timeBar);

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
    addItem(gameOver);
  }
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/game/start/timer" == topic) {
    y->startTimer();
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}




NS_END


