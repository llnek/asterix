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
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(funmath)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }
  void createNextLevelButton();
  void createCheckButton();

  DECL_PTR(ecs::Node, _shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createNextLevelButton() {

  auto btn= cx::reifyMenuBtn( "next_level.png");
  auto menu= cx::mkMenu(btn);
  auto sz= CC_CSIZE(btn);
  auto wb= cx::visBox();

  btn->setPosition(wb.right - CC_ZW(sz), wb.top - CC_ZH(sz));
  btn->setCallback([=](c::Ref*) {
    SENDMSG("/game/hud/gonextlevel");
  });
  CC_HIDE(btn);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createCheckButton() {

  auto btn= cx::reifyMenuBtn("check_solution.png",
                             "check_solution_highlight.png");
  auto menu= cx::mkMenu(btn);
  auto sz= CC_CSIZE(btn);
  auto wb= cx::visBox();

  btn->setPosition(wb.right - HWZ(sz), wb.top - 3 * HHZ(sz));
  btn->setCallback([=](c::Ref*) {
    SENDMSG("/game/hud/checksoln");
  });
  CC_HIDE(btn);
  addItem(menu);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  auto cv= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  _shared= _engine->getNodes("n/GVars")[0];

  if (ss->gameState == eStateTutorial &&
      cv == 0) {
    initializeGameTutorial();
  } else {
    auto dict= cx::readXmlAsDict( XCFG()->getAtlas("levels"));
    auto k= "Level" + FTOS(cv);
    auto levelDict= CC_GDV(c::Dictionary,dict,k);

    _leftNumber = cx::reifySprite(CC_GDS(levelDict, "left number") + ".png");
    _leftNumber->setPosition(wb.right / 5 - HWZ(CC_CSIZE(_leftNumber)), wb.cy);
    addAtlasItem("game-pics", _leftNumber);

    _operator = cx::reifySprite(CC_GDS(levelDict, "operation") + ".png");
    _operator->setPosition(2 * wb.right / 5 - HWZ(CC_CSIZE(_operator)), wb.cy);
    addAtlasItem("game-pics", _operator);

    _equalsSymbol = cx::reifySprite("equals.png");
    _equalsSymbol->setPosition(4 * wb.right / 5 - HWZ(CC_CSIZE(_equalsSymbol)), wb.cy);
    addAtlasItem("game-pics", _equalsSymbol);

    _result = cx::reifySprite(CC_GDS(levelDict, "result") + ".png");
    _result->setPosition(5 * wb.right / 5 - HWZ(CC_CSIZE(_result)), wb.cy);
    addAtlasItem("game-pics", _result);

    _solution = CC_GDS(levelDict, "solution");

    // Load options data
    auto levelOptions= CC_GDV(c::Dictionary,levelDict, "options");
    auto options = levelOptions->allKeys();
    auto cnt= options->count();

    _arrayOfOptions = c::Array::createWithCapacity(kNUM_TUTORIAL_OPTIONS);
    for (auto i = 0; i < cnt; ++i) {
      auto s= (c::String*)options->objectAtIndex(i)
      auto name= CC_GDS(levelOptions, s->getCString());
        // Initialize number
      auto optNum = cx::reifySprite(name + ".png");
      optNum->setName(name);
        // Set number position
      optNum->setPosition(((i+1) * wb.right/cn - HWZ(CC_CSIZE(optNum)), HHZ(CC_CSIZE(optNum)));
      addAtlasItem("game-pics", optNum);
      _arrayOfOptions->addObject(optNum);
    }

    _tutorialLabel = cx::reifyLabel("text",22, "Level "+cv);
    _tutorialLabel->setColor(cx::white());
    _tutorialLabel->setPosition(wb.cx, wb.top - 2 * CC_HZ(CC_CSIZE(_tutorialLabel)));

    _tutorialLabel->setAnchorPoint(c::Vec2(0.5, 0.5));
    addItem(_tutorialLabel);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &tap) {
  if (ss->gameState < eStateTutorialCheckingSolution) {
    defineDroppableArea();
  }
  checkNumberTouched(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}
//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &tap) {
  auto cv= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  if (ss->gameState < eStateTutorialCheckingSolution &&
      cv == 0) {
    getHUD()->setTutMsg("Drop the number in the highlighted area",20);
  } else {
    getHUD()->setTutMsg("Level "+ FTOS(cv), 22);
  }
  _rightNumber->setPosition(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &tap) {
  auto eq_sz= CC_CSIZE(_equalsSymbol);
  auto op_sz= CC_CSIZE(_operator);
  auto rt_sz= CC_CSIZE(_rightNumber);
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  // Define area to drop the number
  auto boardRect =
    c::Rect(
      3 * CC_ZW(wz.size)/5 - 3 * HWZ(op_sz),
      HTV(wz.size.height) - HHZ(eq_sz),
      2 * CC_ZW(op_sz),
      CC_ZH(eq_sz));

  // Only drop number inside the defined area
  if (boardRect.containsPoint(tap)) {
    _rightNumber->setPosition(3 * CC_ZW(wz.size)/5 - HWZ(rt_sz), wb.cy);
    // Set button visible
    CC_SHOW(_buttonCheckSolution);

    if (ss->gameState < eStateTutorialCheckingSolution &&
        CC_APPDB()->gettegerForKey(kCurrentLevel) == 0) {
      removeItem(_droppableArea);
      getHUD()->setTutMsg("Now check the solution by\n" +
                          "pushing the button on the right");
      ss->gameState = eStateTutorialCheckingSolution;
    } else{
      removeItem(_droppableArea);
    }
  } else {
    // Recover initial position
    _rightNumber->setPosition(_initialNumberPosition);
    removeItem(_droppableArea);
    if (ss->gameState < eStateTutorialCheckingSolution &&
        CC_APPDB()->getIntegerForKey(kCurrentLevel) == 0) {
      getHUD()->setTutMsg("Choose the correct number\n" +
                          "from those available");
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  ss->gameState = eStateTutorial;

  createNextLevelButton();
  createCheckButton();

  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




