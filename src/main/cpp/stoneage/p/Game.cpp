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
#include "n/lib.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void onTouchMotion(f::ComObj*, c::Touch*);
  void onTouchEnd(f::ComObj*, c::Touch*);
  bool onTouchStart(f::ComObj*, c::Touch*);

  void buildGrid(GVars*);
  void addToScore();
  void startTimer();
  void tick(float);

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void postReify();

  DECL_PTR(a::NodeList, player)
  DECL_PTR(a::NodeList, shared)
  DECL_BF(touchDown)
  DECL_PTR(c::Sprite, timeBar)
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *touch) {

  auto ss=CC_GNLF(GVars,shared,"slots");
  auto loc=touch->getLocation();

  this->touchDown = true;
  if (!ss->enabled ) { return false; }

  auto touchedGem = findGemAtPosition(ss, loc);
  if (touchedGem.gem != CC_NIL ) {
    if (ENP(ss->selectedGem)) {
      selectStartGem(ss,touchedGem);
    } else {
      if (isValidTarget(ss, touchedGem.x, touchedGem.y, loc) ) {
        selectTargetGem(ss,touchedGem);
      }
      else {
        if (ss->selectedGem != CC_NIL) {
          ss->selectedGem->deselect();
        }
        ss->selectedGem = CC_NIL;
        selectStartGem (ss,touchedGem);
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *touch) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  if (!MGMS()->isLive()) { return; }
  this->touchDown = false;
  if (!ss->enabled) { return; }
  if (ss->selectedGem != CC_NIL) { dropSelectedGem(ss); }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(f::ComObj *co, c::Touch *touch) {

  auto ss=CC_GNLF(GVars,shared,"slots");
  auto loc=touch->getLocation();

  if (!MGMS()->isLive()) { return; }
  if (!ss->enabled ) { return; }

  //track to see if we have a valid target
  if (ss->selectedGem != CC_NIL && this->touchDown ) {
    ss->selectedGem->setPos(
      loc.x - ss->gemsContainer->getPositionX(),
      loc.y - ss->gemsContainer->getPositionY());
    auto touchedGem = findGemAtPosition(ss, loc);
    if (touchedGem.gem != CC_NIL &&
        isValidTarget(ss, touchedGem.x, touchedGem.y, loc)) {
      selectTargetGem(ss,touchedGem);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {

  player=engine->getNodeList(PlayerNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());

  auto py=CC_GNLF(Player,player,"player");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb= cx::visBox();

  ss->selectedGemPosition = c::Vec2(0,0);
  ss->gemsContainer = c::Node::create();
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

  buildGrid(ss);
  this->motionees.push_back(py);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  engine= mc_new(GEngine);
  centerImage("game.bg");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid(GVars *ss) {
  auto TILE_GRID = TILE_SIZE + GRID_SPACE;
  ss->enabled = false;
  for (auto c = 1; c <= GRID_SIZE_X; ++c) {
    auto m= new f::FArrayPtr<Gem>(GRID_SIZE_Y);
    auto g= mc_new1(f::FArrInt,GRID_SIZE_Y);

    ss->gridGemsColumnMap.push_back(m);
    ss->grid.push_back(g);

    for (auto r = 1; r <= GRID_SIZE_Y; ++r) {
      auto gem = Gem::create();
      auto idx= c<3
        ? getVerticalUnique(ss,c-1,r-1)
        : getVerticalHorizontalUnique(ss,c-1,r-1);
      auto t= getGemType(idx);

      gem->setType(t);
      g->set(r-1,t); // zero based
      gem->inflate( c*TILE_GRID, r*TILE_GRID);

      ss->gemsContainer->addChild(gem->node);
      m->set(r-1,gem); // zero based
      ss->allGems.push_back(gem);
    }
  }

  animateIntro(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::addToScore() {
  auto ss=CC_GNLF(GVars,shared,"slots");
  F__LOOP(it, ss->matchArray) {
    auto position = *it;
    auto gem = ss->gridGemsColumnMap[position.x]->get(position.y);
    if (gem->getType() == TYPE_GEM_WHITE) {
        //got a diamond
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::startTimer() {

  auto timeBarBg = cx::loadSprite("timeBarBg.png");
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
void GLayer::tick(float dt) {
  auto scaleNow = timeBar->getScaleX();
  auto speed = 0.007f;
  auto wb= cx::visBox();

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
  else
  if ("/game/player/addscore" == topic) {
    y->addToScore();
  }
  else
  if ("/game/player/earnscore" == topic) {
    auto msg= ( j::json*)m;
    y->getHUD()->updateScore(
        JS_STR(msg->operator[]("type")),
        JS_INT(msg->operator[]("score")));
  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}




NS_END


