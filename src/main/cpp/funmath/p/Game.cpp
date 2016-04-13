// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

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

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void checkNumberTouched(const CCT_PT &tap);
  void initializeGameTutorial();
  void initializeTutorialOptions();
  void defineDroppableArea();
  void createCheckButton();
  void checkSolution();
  void createNextLevelButton();
  void goToNextLevel();
  void onEnd();

  __decl_ptr(c::Sprite, _leftNumber)
  __decl_ptr(c::Sprite, _rightNumber)
  __decl_ptr(c::Sprite, _operator)
  __decl_ptr(c::Sprite, _equalsSymbol)
  __decl_ptr(c::Sprite, _result)
  __decl_ptr(ecs::Node, _shared)

  __decl_vec(c::Sprite*, _arrayOfOptions)
  __decl_md(GameState, _gameState)

  __decl_ptr(c::Label,_tutorialLabel)
  __decl_ptr(c::DrawNode,_droppableArea)
  __decl_md(CCT_PT, _initialNumberPosition)
  __decl_ptr(c::MenuItem,_buttonCheckSolution)
  __decl_ptr(c::MenuItem,_buttonNextLevel)
  __decl_md(sstr, _solution)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  _gameState = eStateTutorial;
  _engine= mc_new(GEngine);

  //centerImage("game.bg");
  regoAtlas("game-pics");

  createNextLevelButton();
  createCheckButton();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initializeGameTutorial() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();
  CCT_SZ sz;

  _leftNumber = cx::reifySprite("2.png");
  XCFG()->fit(_leftNumber);
  sz= CC_CSIZE(_leftNumber);
  CC_POS2(_leftNumber, wb.right/5 - HWZ(sz), wb.cy);
  addAtlasItem("game-pics",_leftNumber);

  _operator = cx::reifySprite("addition.png");
  XCFG()->fit(_operator);
  sz=CC_CSIZE(_operator);
  CC_POS2(_operator, 2*wb.right/5 - HWZ(sz), wb.cy);
  addAtlasItem("game-pics", _operator);

  _equalsSymbol = cx::reifySprite("equals.png");
  XCFG()->fit(_equalsSymbol);
  sz= CC_CSIZE(_equalsSymbol);
  CC_POS2(_equalsSymbol, 4*wz.width/5 - HWZ(sz), wb.cy);
  addAtlasItem("game-pics",_equalsSymbol);

  _result = cx::reifySprite("4.png");
  XCFG()->fit(_result);
  sz= CC_CSIZE(_result);
  CC_POS2(_result, wb.right - HWZ(sz), wb.cy);
  addAtlasItem("game-pics", _result);

  initializeTutorialOptions();

  _tutorialLabel = cx::reifyBmfLabel("dft",
      "Choose the correct number \nfrom those available");
  XCFG()->scaleNode(_tutorialLabel, 24);
  _tutorialLabel->setColor(cx::white());
  CC_POS2(_tutorialLabel, wb.cx, wb.top-CC_CHT(_tutorialLabel));
  addItem(_tutorialLabel);

  _gameState = eStateTutorialDragging;
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initializeTutorialOptions() {

  auto option = cx::reifySprite("3.png");
  XCFG()->fit(option);
  auto sz= CC_CSZ(option);
  auto wb= cx::visBox();

  _arrayOfOptions.clear();

  option->setName("3");
  CC_POS2(option, wb.right / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("2.png");
  XCFG()->fit(option);
  sz=CC_CSZ(option);
  option->setName("2");
  CC_POS2(option,2*wb.right / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("7.png");
  XCFG()->fit(option);
  sz=CC_CSZ(option);
  option->setName("7");
  CC_POS2(option,3*wb.right / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("9.png");
  XCFG()->fit(option);
  sz=CC_CSZ(option);
  option->setName("9");
  CC_POS2(option,4*wb.right / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {

  if (_gameState < eStateTutorialCheckingSolution) {
    defineDroppableArea();
  }
  checkNumberTouched(tap);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &tap) {

  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto fz= 24;

  if (_gameState < eStateTutorialCheckingSolution &&
      cur==0) {
    _tutorialLabel->setString("Drop the number in the highlighted area");
  } else {
    _tutorialLabel->setString("Level "+FTOS(cur));
    fz=32;
  }

  XCFG()->scaleNode(_tutorialLabel, fz);
  CC_POS1(_rightNumber, tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto wb=cx::visBox();
  auto boardRect =
    CCT_RT(3*wb.right/5 - 3*HWZ(CC_CSZ(_operator)), wb.cy-HHZ(CC_CSZ(_equalsSymbol)),
            2*CC_CWH(_operator), CC_CHT(_equalsSymbol));

  if (boardRect.containsPoint(tap)) {
    CC_POS2(_rightNumber,3*wb.right/5 - HWZ(CC_CSZ(_rightNumber)), wb.cy);
    CC_SHOW(_buttonCheckSolution);
    if (_gameState < eStateTutorialCheckingSolution  &&
        cur==0) {
      removeItem(_droppableArea);
      _droppableArea=CC_NIL;
      _tutorialLabel->setString("Now check the solution by\npushing the button on the right");
      _gameState = eStateTutorialCheckingSolution;
    } else{
      removeItem(_droppableArea);
      _droppableArea=CC_NIL;
    }
  } else {
    // Recover initial position
      if (_rightNumber)
    CC_POS1(_rightNumber, _initialNumberPosition);
    // Remove droppable area from scene
    removeItem(_droppableArea);
    _droppableArea=CC_NIL;
    if (_gameState < eStateTutorialCheckingSolution &&
        cur==0) {
      _tutorialLabel->setString("Choose the correct number \nfrom those available");
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::defineDroppableArea() {
  auto eqsz= CC_CSZ(_equalsSymbol);
  auto opsz= CC_CSZ(_operator);
  auto wb= cx::visBox();

  CCT_PT vs[4] = {
    CCT_PT(3*wb.right/5 - 3*HWZ(opsz), wb.cy - HHZ(eqsz)),
    CCT_PT(3*wb.right/5 - 3*HWZ(opsz), wb.cy + HHZ(eqsz)),
    CCT_PT(3*wb.right/5 + HWZ(opsz), wb.cy + HHZ(eqsz)),
    CCT_PT(3*wb.right/5 + HWZ(opsz), wb.cy - HHZ(eqsz))
  };

  // Initialize draw node
  _droppableArea = c::DrawNode::create();
  _droppableArea->setAnchorPoint(cx::anchorBL());
  _droppableArea->drawPolygon(vs,4,c::Color4F(c::Color3B::GREEN),2,
                                c::Color4F(cx::black()));
  addItem(_droppableArea);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkNumberTouched(const CCT_PT &tap) {
  F__LOOP(it, _arrayOfOptions) {
    auto &n= *it;
    if (n->boundingBox().containsPoint(tap)) {
      // The touch location belongs to the number
      _rightNumber = n;
      // Store the initial number position
      _initialNumberPosition = _rightNumber->getPosition();

      // Place the number over the rest of options
      _rightNumber->setZOrder(1);

      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createCheckButton() {

  auto btn= cx::reifyMenuBtn("check_solution.png", "check_solution_highlight.png");
  XCFG()->fit(btn);
  auto sz= CC_CSZ(btn);
  auto wb=cx::visBox();

  CC_POS2(btn, wb.right - HWZ(sz), wb.top - 3*CC_ZH(sz));
  CC_HIDE(btn);
  btn->setCallback([=](c::Ref*) {
      this->checkSolution();
  });

  _buttonCheckSolution=btn;
  addItem(cx::mkMenu(btn));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkSolution() {
  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto fz= 36;

  if (_gameState == eStateTutorialCheckingSolution &&
      cur ==0) {

    if (_rightNumber->getName() == "2") {
      _tutorialLabel->setString("Correct!");
      _gameState = eStateTutorialSuccess;
      CC_SHOW(_buttonNextLevel);
    } else {
      _tutorialLabel->setString("Try again!");
      _rightNumber->setPosition(_initialNumberPosition);
      _gameState = eStateTutorialDragging;
    }
    CC_HIDE(_buttonCheckSolution);

  } else {

    if (_rightNumber->getName() == _solution) {
      _tutorialLabel->setString("Correct!");
      CC_SHOW(_buttonNextLevel);
    } else {
      _tutorialLabel->setString("Try again!");
      _rightNumber->setPosition(_initialNumberPosition);
    }
    CC_HIDE(_buttonCheckSolution);
  }

  XCFG()->scaleNode(_tutorialLabel, fz);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createNextLevelButton() {

  auto btn= cx::reifyMenuBtn("next_level.png");
  XCFG()->fit(btn);
  auto wb= cx::visBox();
  auto sz= CC_CSZ(btn);

  _buttonNextLevel= btn;
  CC_POS2(btn, wb.right - CC_ZW(sz), wb.top - CC_ZH(sz));
  btn->setCallback([=](c::Ref*) {
      this->goToNextLevel();
      });

  CC_HIDE(btn);
  addItem(cx::mkMenu(btn));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::goToNextLevel() {

  if (_gameState == eStateTutorialSuccess) {
    CC_APPDB()->setBoolForKey(kTutorialSucceded, true);
  }

  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel) + 1;
  CC_APPDB()->setIntegerForKey(kCurrentLevel, cur);
  CC_APPDB()->flush();

  cx::runEx(Game::reify(mc_new(GameCtx)));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  if (_gameState == eStateTutorial && cur == 0) {
    initializeGameTutorial();

  } else {

    auto dict= cx::readXmlAsDict(XCFG()->getAtlas("levels"));
    auto k= "Level" + FTOS(cur);
    auto levelDict=(c::Dictionary*)dict->objectForKey(k);

    if (E_NIL(levelDict)) {
      onEnd();
      return;
    }

    _leftNumber = cx::reifySprite(sstr()+ CC_GDS(levelDict, "left number") +".png");
    XCFG()->fit(_leftNumber);
    CC_POS2(_leftNumber,wb.right/5 - HWZ(CC_CSZ(_leftNumber)), wb.cy);
    addAtlasItem("game-pics",_leftNumber);

    _operator = cx::reifySprite(sstr()+CC_GDS(levelDict, "operation") +".png");
    XCFG()->fit(_operator);
    CC_POS2(_operator,2*wb.right/5 - HWZ(CC_CSZ(_operator)), wb.cy);
    addAtlasItem("game-pics",_operator);

    _equalsSymbol = cx::reifySprite("equals.png");
    XCFG()->fit(_equalsSymbol);
    CC_POS2(_equalsSymbol,4*wb.right/5 - HWZ(CC_CSZ(_equalsSymbol)), wb.cy);
    addAtlasItem("game-pics",_equalsSymbol);

    _result = cx::reifySprite(sstr()+CC_GDS(levelDict, "result") +".png");
    XCFG()->fit(_result);
    CC_POS2(_result,5*wb.right/5 - HWZ(CC_CSZ(_result)), wb.cy);
    addAtlasItem("game-pics",_result);

    _solution = CC_GDS(levelDict, "solution");

    auto levelOptionsDict= (c::Dictionary*) levelDict->objectForKey("options");
    auto options = levelOptionsDict->allKeys();
    auto cnt= options->count();

    _arrayOfOptions.clear();

    for (auto i = 0; i < cnt; ++i) {
      auto obj= (c::String*) options->objectAtIndex(i);
      auto name = levelOptionsDict->valueForKey(obj->getCString());
      auto sp= cx::reifySprite(sstr() + name->getCString() + ".png");
      XCFG()->fit(sp);
      sp->setName(name->getCString());
      CC_POS2(sp, (i+ 1)*wb.right/cnt - HWZ(CC_CSZ(sp)), HHZ(CC_CSZ(sp)));
      addAtlasItem("game-pics", sp);
      _arrayOfOptions.push_back(sp);
    }

    _tutorialLabel = cx::reifyBmfLabel("dft", "Level " + FTOS(cur));
    XCFG()->scaleNode(_tutorialLabel, 24);
    _tutorialLabel->setColor(cx::white());
    CC_POS2(_tutorialLabel, wb.cx, wb.top - 2* CC_CHT(_tutorialLabel));
    addItem(_tutorialLabel);
  }

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




