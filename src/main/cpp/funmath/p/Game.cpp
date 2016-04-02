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

  void checkNumberTouched(const c::Vec2 &tap);
  void initializeGameTutorial();
  void initializeTutorialOptions();
  void defineDroppableArea();
  void createCheckButton();
  void checkSolution();
  void createNextLevelButton();
  void goToNextLevel();

  __decl_ptr(c::Sprite, _leftNumber)
  __decl_ptr(c::Sprite, _rightNumber)
  __decl_ptr(c::Sprite, _operator)
  __decl_ptr(c::Sprite, _equalsSymbol)
  __decl_ptr(c::Sprite, _result)
  __decl_ptr(ecs::Node, _shared)

  s_vec<c::Sprite*> _arrayOfOptions;
  GameState _gameState;

  __decl_ptr(c::Label,_tutorialLabel)
  __decl_ptr(c::DrawNode,_droppableArea)
  __decl_md(c::Vec2, _initialNumberPosition)
  __decl_ptr(c::MenuItem,_buttonCheckSolution)
  __decl_ptr(c::MenuItem,_buttonNextLevel)
  __decl_md(sstr, _solution)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);
  virtual bool onMouseStart(const c::Vec2&);

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
void GLayer::decoUI() {

  _gameState = eStateTutorial;
  _engine= mc_new(GEngine);

  //centerImage("game.bg");
  regoAtlas("game-pics");

  createCheckButton();
  createNextLevelButton();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initializeGameTutorial() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

  _leftNumber = cx::reifySprite("2.png");
  _leftNumber->setPosition(wz.width/5 - HWZ(CC_CSZ(_leftNumber)), wz.height/2);
  addAtlasItem("game-pics",_leftNumber);

  _operator = cx::reifySprite("addition.png");
  _operator->setPosition(2*wz.width/5 - HWZ(CC_CSZ(_operator)), wz.height/2);
  addAtlasItem("game-pics", _operator);

  _equalsSymbol = cx::reifySprite("equals.png");
  _equalsSymbol->setPosition(4*wz.width/5 - HWZ(CC_CSZ(_equalsSymbol)), wz.height/2);
  addAtlasItem("game-pics",_equalsSymbol);

  _result = cx::reifySprite("4.png");
  _result->setPosition(5*wz.width/5 - HWZ(CC_CSZ(_result)), wz.height/2);
  addAtlasItem("game-pics", _result);

  initializeTutorialOptions();

  _tutorialLabel = cx::reifyLabel("text",20,
      sstr() + "Choose the correct number \n" +
      "from those available");
  _tutorialLabel->setColor(cx::white());
  _tutorialLabel->setPosition(wz.width/2, wz.height - CC_CSZ(_tutorialLabel).height);

  _tutorialLabel->setAnchorPoint(cx::anchorC());
  addItem(_tutorialLabel);

  _gameState = eStateTutorialDragging;
}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initializeTutorialOptions() {

  auto option = cx::reifySprite("3.png");
  auto sz= CC_CSZ(option);
  auto wz= cx::visSize();

  _arrayOfOptions.clear();

  option->setName("3");
  option->setPosition(wz.width / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("2.png");
  sz=CC_CSZ(option);
  option->setName("2");
  option->setPosition(2*wz.width / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("7.png");
  sz=CC_CSZ(option);
  option->setName("7");
  option->setPosition(3*wz.width / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

  option = cx::reifySprite("9.png");
  sz=CC_CSZ(option);
  option->setName("9");
  option->setPosition(4*wz.width / kNUM_TUTORIAL_OPTIONS - HWZ(sz), HHZ(sz));
  addAtlasItem("game-pics", option);
  _arrayOfOptions.push_back(option);

}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &tap) {

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
void GLayer::onMouseMotion(const c::Vec2 &tap) {

  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
    c::TTFConfig cfg (_tutorialLabel->getTTFConfig());
  if (_gameState < eStateTutorialCheckingSolution &&
      cur==0) {
      cfg.fontSize=20;
      _tutorialLabel->setTTFConfig(cfg);
    //_tutorialLabel->setFontSize(20);
    _tutorialLabel->setString("Drop the number in the highlighted area");
  } else {
      cfg.fontSize=22;
      _tutorialLabel->setTTFConfig(cfg);
    //_tutorialLabel->setFontSize(22);
    _tutorialLabel->setString("Level "+FTOS(cur));
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
  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
  auto wz=cx::visSize();
  auto boardRect =
    c::Rect(3*wz.width/5 - 3*HWZ(CC_CSZ(_operator)), wz.height/2  - HHZ(CC_CSZ(_equalsSymbol)),
            2*CC_CSZ(_operator).width, CC_CSZ(_equalsSymbol).height);

  if (boardRect.containsPoint(tap)) {
    _rightNumber->setPosition(3 *wz.width/5 - HWZ(CC_CSZ(_rightNumber)), wz.height/2);
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
    _rightNumber->setPosition( _initialNumberPosition);
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
  auto wz= cx::visSize();

  c::Vec2 vs[4] = {
    c::Vec2(3*wz.width/5 - 3*HWZ(opsz), wz.height/2 - HHZ(eqsz)),
    c::Vec2(3*wz.width/5 - 3*HWZ(opsz), wz.height/2 + HHZ(eqsz)),
    c::Vec2(3*wz.width/5 + HWZ(opsz), wz.height/2 + HHZ(eqsz)),
    c::Vec2(3*wz.width/5 + HWZ(opsz), wz.height/2 - HHZ(eqsz))
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
void GLayer::checkNumberTouched(const c::Vec2 &tap) {
  F__LOOP(it, _arrayOfOptions) {
    auto n= *it;
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
  auto sz= CC_CSZ(btn);
    auto wb=cx::visBox();
  auto menu=cx::mkMenu(btn);

  btn->setPosition(wb.right - HWZ(sz), wb.top - 3*HHZ(sz));
  CC_HIDE(btn);
  btn->setCallback([=](c::Ref*) {
      this->checkSolution();
  });

  _buttonCheckSolution=btn;
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkSolution() {
  auto cur= CC_APPDB()->getIntegerForKey(kCurrentLevel);
    c::TTFConfig cfg(_tutorialLabel->getTTFConfig());

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
    cfg.fontSize= 25;
    CC_HIDE(_buttonCheckSolution);

  } else {

    if (_rightNumber->getName() == _solution) {
      _tutorialLabel->setString("Correct!");
      CC_SHOW(_buttonNextLevel);
    } else {
      _tutorialLabel->setString("Try again!");
      _rightNumber->setPosition(_initialNumberPosition);
    }
      cfg.fontSize= 25;
      CC_HIDE(_buttonCheckSolution);
  }

    _tutorialLabel->setTTFConfig(cfg);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createNextLevelButton() {

  auto btn= cx::reifyMenuBtn("next_level.png");
  auto wb= cx::visBox();
  auto sz= CC_CSZ(btn);
  auto menu= cx::mkMenu(btn);

  _buttonNextLevel= btn;
  btn->setPosition(wb.right - CC_ZW(sz), wb.top - CC_ZH(sz));
  btn->setCallback([=](c::Ref*) {
      this->goToNextLevel();
      });

  CC_HIDE(btn);
  addItem(menu);
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

  cx::runEx(Game::reify(new GameCtx() ));
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
    _leftNumber = cx::reifySprite( sstr() + CC_GDS(levelDict, "left number") +".png");
    _leftNumber->setPosition(wz.width/5 - HWZ(CC_CSZ(_leftNumber)), wz.height/2);
    addAtlasItem("game-pics",_leftNumber);

    _operator = cx::reifySprite( sstr() + CC_GDS(levelDict, "operation") +".png");
    _operator->setPosition(2*wz.width/5 - HWZ(CC_CSZ(_operator)), wz.height/2);
    addAtlasItem("game-pics",_operator);

    _equalsSymbol = cx::reifySprite("equals.png");
    _equalsSymbol->setPosition(4*wz.width/5 - HWZ(CC_CSZ(_equalsSymbol)), wz.height/2);
    addAtlasItem("game-pics",_equalsSymbol);

    _result = cx::reifySprite( sstr() + CC_GDS(levelDict, "result") +".png");
    _result->setPosition(5*wz.width/5 - HWZ(CC_CSZ(_result)), wz.height/2);
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
      sp->setName(name->getCString());

      sp->setPosition((i+ 1) * wz.width / cnt - HWZ(CC_CSZ(sp)), HHZ(CC_CSZ(sp)));
      addAtlasItem("game-pics", sp);
      _arrayOfOptions.push_back(sp);
    }

    _tutorialLabel = cx::reifyLabel("text", 22, "Level " + FTOS(cur));
    _tutorialLabel->setColor(cx::white());
    _tutorialLabel->setPosition(wb.cx, wb.top - 2* CC_CSZ(_tutorialLabel).height);
    _tutorialLabel->setAnchorPoint(cx::anchorC());
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




