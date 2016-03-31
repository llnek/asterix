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

NS_ALIAS(cui,cocos2d::ui)
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(gmath)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  __decl_ptr(c::Label,_lblTurnsSurvived)
  __decl_ptr(c::Label,_lblUnitsKilled)
  __decl_ptr(c::Label,_lblTotalScore)
  __decl_iz(_numTurnSurvived)
  __decl_iz(_numTotalScore)
  __decl_iz(_numUnitsKilled)
  __decl_iz(_tutorialPhase)

  void showTutHelp(GVars*);
  void runFingerArrowActions(c::Sprite *finger);
  void advanceTutorial();
  void removePrevTut();
  Unit* getRandomEnemy();
  void spawnNewEnemy(Unit*);
  void gotoMenu();
  void restartGame();
  void slideAllByDist(float, int);
  void moveUnit(c::Ref*);
  void checkForNewFriend();
  void updateLabels();
  void moveAllUnits();
  void checkForAllCombines();

  void checkForAnyDirCombine(Unit*, Unit*, c::Array*);
  void checkForCombine(Unit*, Unit*, c::Array*);
  void pulseUnit(c::Node*);
  void checkForDirHits();
  void checkForAllHits();
  void handleHitWithFriend(Unit*, Unit*, c::Array *array, bool isDirectional);
  void playCombineSound(int total);
  int saveHighScore();
  void rubberBandToScene();
  void endGame();

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }

  __decl_ptr(ecs::Node, _shared)

  STATIC_REIFY_LAYER(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const c::Vec2&);
  virtual bool onMouseStart(const c::Vec2&);
  virtual void onMouseClick(const c::Vec2&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  c::Color3B c(52/255.0, 73/255.0, 94/255.0);
    auto wz=cx::visSize();
  auto wb=cx::visBox();
    auto bg= c::LayerColor::create(c::Color4B::WHITE,
                         wz.width*5, wz.height*5);
  bg->setPosition(wb.cx, wb.cy);
  addItem(bg, -2);

  regoAtlas("game-pics");

  auto desc = cx::reifyBmfLabel("dft", "Turns Survived:");
  desc->setPosition(wb.right * 0.125, wb.top * 0.9);
  desc->setColor(c);
  addItem(desc);

  _lblTurnsSurvived= cx::reifyBmfLabel("dft","0");
  _lblTurnsSurvived->setPosition(wb.right * 0.125, wb.top * 0.82);
  _lblTurnsSurvived->setColor(c);
  addItem(_lblTurnsSurvived);

  desc= cx::reifyBmfLabel("dft","Units Killed:");
  desc->setPosition(wb.right * 0.125, wb.top * 0.7);
  desc->setColor(c);
  addItem(desc);

  _lblUnitsKilled= cx::reifyBmfLabel("dft","0");
  _lblUnitsKilled->setPosition(wb.right * 0.125, wb.top * 0.62);
  _lblUnitsKilled->setColor(c);
  addItem(_lblUnitsKilled);

  desc= cx::reifyBmfLabel("dft","Total Score:");
  desc->setPosition(wb.right * 0.125, wb.cy);
  desc->setColor(c);
  addItem(desc);

  _lblTotalScore= cx::reifyBmfLabel("dft","1");
  _lblTotalScore->setPosition(wb.right * 0.125, wb.top * 0.42);
  _lblTotalScore->setColor(c);
  addItem(_lblTotalScore);

  auto btnR= cx::reifyMenuBtn("btnRestart.png");
  auto btnM= cx::reifyMenuBtn("btnMenu.png");
  btnM->setCallback([=](c::Ref*){
      this->gotoMenu();
      });
  btnR->setCallback([=](c::Ref*){
      this->restartGame();
      });

  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{btnR,btnM});
  menu->setPosition(wb.right * 0.125, wb.top * 0.15);
  addItem(menu);

  auto board= cx::reifySprite("imgBoard.png");
  board->setPosition(wb.right * 0.625, wb.cy);
  addAtlasItem("game-pics", board);

  if (c::ApplicationProtocol::Platform::OS_IPHONE ==
      XCFG()->getPlatform()) {
    board->setScale(0.8);
  }

  _engine=mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

    _shared=_engine->getNodes("n/GVars")[0];
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto fu = Unit::friendly(ss);
  ss->unitSize= fu->boundingBox().size;
  fu->setPosition(getPosAsGrid(ss,fu->_gridPos));
  addAtlasItem("game-pics", fu);

  //ss->enemies.removeAllObjects();
  ss->friends->addObject(fu);

  _numTurnSurvived = 0;
  _numTotalScore = 1;
  _numUnitsKilled = 0;

  c::NotificationCenter::getInstance()->addObserver(
      this, CC_CALLFUNCO_SELECTOR(GLayer::moveUnit), kTurnCompletedNotification, CC_NIL);

  if (CC_APPDB()->getBoolForKey(KeyFinishedTutorial)) {
    spawnNewEnemy(getRandomEnemy());
    _tutorialPhase = 0;
  } else {
      //spawn enemy on far right with value of 1
    auto enemy= Unit::enemyWith(ss, 1, f::Cell2I(9, 5));
    enemy->setPosition(getPosAsGrid(ss,enemy->_gridPos));
    enemy->setDir(DirLeft); //2 is right wall
    spawnNewEnemy(enemy);
    _tutorialPhase = 1;
    showTutHelp(ss);
  }

  _engine=mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showTutHelp(GVars *ss) {
  auto tutString = "";
  if (_tutorialPhase == 1) {
    auto lbl= cx::reifyBmfLabel("dft", "How to Play:");
    lbl->setPosition(getPosAsGrid(ss,f::Cell2I(5,1)));
    lbl->setColor(c::Color3B(52, 73, 94));
    lbl->setName("lblHowToPlay");
    lbl->setScale(0.8);
    addItem(lbl, 2);

    auto bgHowTo=
      cui::Scale9Sprite::createWithSpriteFrameName("imgUnit.png");
    //bgHowTo->setMargin(0.2);
    bgHowTo->setPosition(0.5,0.4);
    //bgHowTo->setPositionType(c::PositionTypeNormalized);
    bgHowTo->setContentSize(c::Size(1.05,1.2));
    //bgHowTo.contentSizeType = CCSizeTypeNormalized;
    lbl->addChild(bgHowTo,-1);

    tutString = "Drag Friendly Units";
  }
  else if (_tutorialPhase == 2) {
    getChildByName("lblHowToPlay")->runAction(
      c::Sequence::create(
          c::EaseInOut::create(c::FadeOut::create(0.5), 2),
          c::RemoveSelf::create(true),
          CC_NIL));
    tutString = "Combine Friendly Units";
  }
  else if (_tutorialPhase == 3) {
    tutString = "Defeat Enemies";
  }
  else if (_tutorialPhase == 4) {
    tutString = "Protect Center";
  }
  else if (_tutorialPhase == 5) {
    tutString = "Survive";
  }
  else if (_tutorialPhase == 6) {
    tutString = "Enjoy! :)";
  }

  auto lbl= cx::reifyBmfLabel("dft", tutString);
  lbl->setPosition(getPosAsGrid(ss, f::Cell2I(5,2)));
  lbl->setColor(c::Color3B(52,73,94));
  lbl->setName("tutorialText");
  addItem(lbl,2);

  auto bg =
    cui::Scale9Sprite::createWithSpriteFrameName("imgUnit.png");
  bg->setContentSize(c::Size(1.05, 1.2));
  //bg->setMargin(0.2);
  bg->setPosition(0.5,0.4);
  //background.positionType = CCPositionTypeNormalized;
  //background.contentSizeType = CCSizeTypeNormalized;
  lbl->addChild(bg,-1);

  auto finger = cx::reifySprite("imgFinger.png");
  finger->setPosition(getPosAsGrid(ss, f::Cell2I(5,5)));
  finger->setAnchorPoint(c::Vec2(0.4,1));
  finger->setName("finger");
  finger->setOpacity(0);
  addAtlasItem("game-pics",finger,2);
  runFingerArrowActions(finger);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::runFingerArrowActions(c::Sprite *finger) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto u = Unit::friendly(ss);
  if (_tutorialPhase == 1 ||
      _tutorialPhase == 3) {

    finger->runAction(
        c::RepeatForever::create(
          c::Sequence::create(
            c::EaseIn::create(c::FadeIn::create(0.25),2),
            c::EaseInOut::create(
              c::MoveBy::create(1, c::Vec2(u->_gridWidth*2, 0)), 2),
            c::DelayTime::create(0.5),
            CC_NIL)));

    finger->runAction(
        c::RepeatForever::create(
          c::Sequence::create(
              c::DelayTime::create(0.25),
              c::EaseInOut::create(
                c::FadeOut::create(1), 2),
              c::DelayTime::create(0.5),
              c::CallFunc::create([=]() {
                finger->setPosition(getPosAsGrid(ss, f::Cell2I(5,5)));
                }),
              CC_NIL)));
  }
  else if (_tutorialPhase == 2) {

    finger->setPosition(getPosAsGrid(ss, f::Cell2I(6,5)));

    finger->runAction(
        c::RepeatForever::create(
          c::Sequence::create(
              c::EaseIn::create(
                c::FadeIn::create(0.25), 2),
              c::EaseInOut::create(
                c::MoveBy::create(1, c::Vec2(-u->_gridWidth*2, 0)), 2),
              c::DelayTime::create(0.5),
              CC_NIL)));

    finger->runAction(
        c::RepeatForever::create(
          c::Sequence::create(
              c::DelayTime::create(0.25),
              c::EaseInOut::create(
                c::FadeOut::create(1), 2),
              c::DelayTime::create(0.5),
              c::CallFunc::create([=]() {
                finger->setPosition(getPosAsGrid(ss, f::Cell2I(6,5)));
                }),
              CC_NIL)));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::advanceTutorial() {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  ++_tutorialPhase;
  removePrevTut();

  if (_tutorialPhase < 7) {
    showTutHelp(ss);
  } else {
    //the tutorial should be marked as "visible"
    CC_APPDB()->setBoolForKey(KeyFinishedTutorial,true);
    CC_APPDB()->flush();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::removePrevTut() {

  auto lbl= (c::Label*) getChildByName("tutorialText");
  lbl->setName("old_instructions");
  lbl->runAction(
    c::Sequence::create(
        c::EaseInOut::create(
          c::FadeTo::create(0.5, 0), 2),
        c::RemoveSelf::create(true),
        CC_NIL));

  auto finger = (c::Sprite*)getChildByName("finger");
  finger->setName("old_finger");
  finger->runAction(
    c::Sequence::create(
        c::EaseInOut::create(
          c::FadeTo::create(0.5,0), 2),
        c::RemoveSelf::create(true),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
Unit* GLayer::getRandomEnemy() {
  auto wall = rand() % 4 + 1; //1 is top wall, 2 is right wall, 3 is bottom wall, 4 is left wall
  auto xPos = 1;
  auto yPos = 1;
  if (wall == 1) {
    xPos = rand() % 9 + 1;
  }
  else if (wall == 2) {
    xPos = 9;
    yPos = rand() % 9 + 1;
  }
  else if (wall == 3) {
    yPos = 9;
    xPos = rand() % 9 + 1;
  }
  else if (wall == 4) {
    yPos = rand() % 9 + 1;
  }

  //base difficulty: 3
  auto upperBound = 3 + (_numTurnSurvived / 17); //up difficulty every 17 turns (15 felt a little harsh, 20 might be too easy)

  auto unitValue = (rand() % upperBound) + 1;
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto e = Unit::enemyWith(ss, unitValue, f::Cell2I(xPos, yPos));
  e->setPosition(getPosAsGrid(ss, e->_gridPos));
  e->setDir(wall);

  return e;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnNewEnemy(Unit *enemy) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");

  addAtlasItem("game-pics",enemy);
  ss->enemies->addObject(enemy);
  enemy->setScale(0);
  pulseUnit(enemy);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::gotoMenu() {
  cx::sfxPlay("click");
  //rubberBandToScene(this, 0.5, kMoveDirectionDown);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::restartGame() {
  cx::sfxPlay("click");
  cx::runEx(Game::reify(new GameCtx()));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::slideAllByDist(float dist, int dir) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  c::Ref *u=CC_NIL;
  CCARRAY_FOREACH(ss->friends, u) {
    static_cast<Unit*>(u)->slideWithDist(dist, dir);
  }

  u=CC_NIL;
  CCARRAY_FOREACH(ss->enemies, u) {
    static_cast<Unit*>(u)->slideWithDist(dist, dir);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveUnit(c::Ref *r) {

    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto u= static_cast<Unit*>(r);

  if (_tutorialPhase == 5 ||
      _tutorialPhase == 6) {

    advanceTutorial();
  }

  u->setPosition(getPosAsGrid(ss, u->_gridPos));
  cx::sfxPlay("moveUnit");
  ++_numTurnSurvived;

  checkForNewFriend();
  moveAllUnits();

  //spawn a unit every 3 turns
  auto rate = 3;

  if (_numTurnSurvived % rate == 0 ||
      ss->enemies->count() == 0) {

    if (_tutorialPhase == 4) {
      auto e = Unit::enemyWith(ss, 4, f::Cell2I(5,9));
      e->setDir(DirUp);
      e->setPosition(getPosAsGrid(ss, f::Cell2I(5,9)));
      spawnNewEnemy(e);
    } else {
      if (_numTurnSurvived > 200) {
        //10% chance to spawn a 2nd unit... hehe... and at this level of difficulty? Upper bound of, 18? HAH! Good luck.
        if (rand() % 100 < 10) {
          spawnNewEnemy(getRandomEnemy());
        }
      }
      spawnNewEnemy(getRandomEnemy());
    }
  }

  checkForAllCombines();
  checkForAllHits();

  updateLabels();

  auto hasUnitAtCenter = false;

  c::Ref *rr=CC_NIL;
  CCARRAY_FOREACH(ss->friends,rr) {
    auto u=static_cast<Unit*>(r);
    if (u->_gridPos.x == 5 &&
        u->_gridPos.y == 5) {
      hasUnitAtCenter = true;
    }
  }

  if (!hasUnitAtCenter) {
    //end game!
    SENDMSG("/game/player/lose");
    return;
  }

  if (_tutorialPhase == 1 || _tutorialPhase == 2) {
    advanceTutorial();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForNewFriend() {
  //if there's a unit standing, that means there's one at the center, so return
  //don't worry about (5,5), as that should be a "combine" case if it ever were to come up...
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  c::Ref *r= CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto f=SCAST(Unit*,r);
    if (f->_direction == DirStanding) {
      return;
    }
  }

  auto f = Unit::friendly(ss);
  f->setPosition(getPosAsGrid(ss, f->_gridPos));
  addAtlasItem("game-pics",f, 1);
  ss->friends->addObject(f);
  ++_numTotalScore;
  f->setName("new");
  f->setScale(0);
  pulseUnit(f);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateLabels() {
  _lblTotalScore->setString(FTOS(_numTotalScore));
  _lblTurnsSurvived->setString(FTOS(_numTurnSurvived));
  _lblUnitsKilled->setString(FTOS(_numUnitsKilled));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveAllUnits() {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto arrDel= c::Array::create();
  sstr dead= sstr("dead");

  c::Ref *r= CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto f=SCAST(Unit*,r);
    if (f->getName() == "new") {
      f->setName("");
    }
    else
    if (f->didMoveIncNumber()) {
      ++_numTotalScore;
    }

    f->setPosition(getPosAsGrid(ss, f->_gridPos));

    if (f->_unitValue == 0) {
      arrDel->addObject(f);
    }
    //if he's not already dead
    else if (!  (dead == f->getName() )) {
        c::Ref *rr=CC_NIL;
      CCARRAY_FOREACH(ss->friends,rr) {
        auto other=SCAST(Unit*,rr);
        //if other unit... and neither are dead... and
        checkForCombine(f, other, arrDel);
      }
    }
  }

  ss->friends->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }

  arrDel->removeAllObjects();

  checkForDirHits();

  //move all enemies
  r=CC_NIL;
  CCARRAY_FOREACH(ss->enemies,r) {
    auto e=SCAST(Unit*,r);
    e->didMoveIncNumber();
    e->setPosition(getPosAsGrid(ss, e->_gridPos));
    e->setNewDirForEnemy();

    if (! (dead == e->getName() )) {
        c::Ref *rr=CC_NIL;
      CCARRAY_FOREACH(ss->enemies,rr) {
        auto other=SCAST(Unit*,rr);
        checkForCombine(e, other, arrDel);
      }
    }
  }

  ss->enemies->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }

  checkForAllHits();
  checkForAllCombines();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAllCombines() {

    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto arrDel= c::Array::create();

  c::Ref *r=CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto f=SCAST(Unit*,r);
    c::Ref *rr=CC_NIL;
    CCARRAY_FOREACH(ss->friends,rr) {
      auto other=SCAST(Unit*,rr);
      if (f != other)
        checkForAnyDirCombine(f, other, arrDel);
    }
  }

  ss->friends->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }
  arrDel->removeAllObjects();

  r=CC_NIL;
  CCARRAY_FOREACH(ss->enemies,r) {
    auto e=SCAST(Unit*,r);
    c::Ref *rr=CC_NIL;
    CCARRAY_FOREACH(ss->enemies,rr) {
      auto other=SCAST(Unit*,rr);
      if (e != other)
        checkForAnyDirCombine(e, other, arrDel);
    }
  }

  ss->enemies->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }
  arrDel->removeAllObjects();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAnyDirCombine(Unit *first, Unit *other, c::Array *array) {

  sstr dead="dead";

  if (other != first &&
    !(other->getName() == dead) &&
    !(first->getName() == dead) &&
    first->_gridPos.x == other->_gridPos.x &&
    first->_gridPos.y == other->_gridPos.y) {

    auto fv = first->_unitValue;
    auto ov = other->_unitValue;

    if (first->_isFriendly) {
      playCombineSound(fv+ov);
    }

    if (ov > fv) {
      first->setName("dead");
      array->addObject(first);
      other->_unitValue += fv;
      first->_direction = other->_direction;
      other->updateLabel();
      pulseUnit(other);
    } else {
      other->setName("dead");
      array->addObject(other);
      first->_unitValue += ov;
      other->_direction = first->_direction;
      first->updateLabel();
      pulseUnit(first);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForCombine(Unit *first, Unit *other, c::Array *array) {
  sstr dead="dead";
  if (other != first &&
    !(other->getName() == dead) &&
    !(first->getName() == dead) &&
    first->_gridPos.x == other->_gridPos.x &&
    first->_gridPos.y == other->_gridPos.y) {

    //if the opposite way... or at a wall (but collided just now, so you must've been going the opposite to make that happen)
    if ((first->_direction == DirUp && other->_direction == DirDown) ||
        (first->_direction == DirDown && other->_direction == DirUp) ||
        (first->_direction == DirLeft && other->_direction == DirRight) ||
        (first->_direction == DirRight && other->_direction == DirLeft) ||
        first->_direction == DirAtWall ||
        first->_direction == DirStanding) {

      auto fv = first->_unitValue;
      auto ov = other->_unitValue;

      if (first->_isFriendly) {
        playCombineSound(fv+ov);
      }

      if (ov > fv) {
        first->setName("dead");
        array->addObject(first);
        other->_unitValue += fv;
        first->_direction = other->_direction;
        other->updateLabel();
        pulseUnit(other);
      } else {
        other->setName("dead");
        array->addObject(other);
        first->_unitValue += ov;
        other->_direction = first->_direction;
        first->updateLabel();
        pulseUnit(first);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::pulseUnit(c::Node *unit) {

  auto baseScale = 1.0;

  if (c::ApplicationProtocol::Platform::OS_IPHONE ==
      XCFG()->getPlatform()) {
    baseScale = 0.8;
  }

  unit->stopAllActions();
  unit->runAction(
      c::Sequence::create(
        c::EaseInOut::create(
            c::ScaleTo::create(0.15 , baseScale * 1.2), 2),
        c::EaseInOut::create(
            c::ScaleTo::create(0.15, baseScale * 0.9), 2),
        c::EaseInOut::create(
            c::ScaleTo::create(0.25, baseScale), 2),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForDirHits() {

    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto arrDel= c::Array::create();
  c::Ref *r=CC_NIL;

  CCARRAY_FOREACH(ss->friends,r) {
    auto f=SCAST(Unit*,r);
    c::Ref *rr=CC_NIL;
    CCARRAY_FOREACH(ss->enemies,rr) {
      auto e=SCAST(Unit*,rr);
      //at same coordinate...
      if (f->_gridPos.x == e->_gridPos.x &&
        f->_gridPos.y == e->_gridPos.y) {
        //if the opposite way... or at a wall (but collided just now, so you must've been going the opposite to make that happen)
        if ((f->_direction == DirUp && e->_direction == DirDown) ||
          (f->_direction == DirDown && e->_direction == DirUp) ||
          (f->_direction == DirLeft && e->_direction == DirRight) ||
          (f->_direction == DirRight && e->_direction == DirLeft) ||
          f->_direction == DirAtWall) {
          //collision!
          handleHitWithFriend(f, e, arrDel, true);
          //exit the for so no "bad things" happen
          //goto afterDirCollide;
          break;
        }
      }
    }
    //afterDirCollide:{}
  }

  ss->friends->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAllHits() {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto arrDel=c::Array::create();
  c::Ref *r=CC_NIL;

  CCARRAY_FOREACH(ss->friends,r) {
    auto f=SCAST(Unit*,r);
    c::Ref *rr=CC_NIL;
    CCARRAY_FOREACH(ss->enemies,rr) {
      auto e=SCAST(Unit*,rr);
      //at same coordinate...
      if (f->_gridPos.x == e->_gridPos.x &&
        f->_gridPos.y == e->_gridPos.y) {
        //collision!
        handleHitWithFriend(f, e, arrDel,false);

        //exit the for so no "bad things" happen
        //goto afterAllCollide;
        break;
      }
    }
    //afterAllCollide:{}
  }

  ss->friends->removeObjectsInArray(arrDel);
  ss->enemies->removeObjectsInArray(arrDel);

  r=CC_NIL;
  CCARRAY_FOREACH(arrDel,r) {
    removeItem(SCAST(Unit*,r));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleHitWithFriend(Unit *f, Unit *e,
    c::Array *array, bool isDirectional) {

    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto fv = f->_unitValue;
  auto ev = e->_unitValue;

  if (isDirectional) {
    e->_unitValue -= fv;
    f->_unitValue -= (ev+1);
  } else {
    f->_unitValue -= ev;
    e->_unitValue -= fv;
  }

  f->updateLabel();
  e->updateLabel();

  if (f->_unitValue <= 0) {
    array->addObject(f);
  }

  if ((e->_unitValue <= 0 && !isDirectional) ||
    (e->_unitValue < 0 && isDirectional)) {
    ss->enemies->removeObject(e);
    removeItem(e);
    ++_numUnitsKilled;

    if (_tutorialPhase == 3 ||
        _tutorialPhase == 4) {
      advanceTutorial();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::playCombineSound(int total) {
  auto pitchValue = 1.0 - (total / 100.0); //eg: fv+ov = 20 ... 1.0 - 0.2 = 0.8
  if (total < 50) {
    cx::sfxPlay("unitCombine");
  } else {
    cx::sfxPlay("largeUnitCombine");
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int GLayer::saveHighScore() {
#if 0
  //save top 20 scores

  //an array of Dictionaries...
  //keys in each dictionary:
  //  [DictTotalScore]
  //  [DictTurnsSurvived]
  //  [DictUnitsKilled]
  NSMutableArray *arrScores = [[[NSUserDefaults standardUserDefaults] arrayForKey:DataHighScores] mutableCopy];

  auto index = -1;
  for (NSDictionary *dictHighScore in arrScores) {
    if (_numTotalScore > [dictHighScore[DictTotalScore] integerValue]) {
      index = [arrScores indexOfObject:dictHighScore];
      break;
    }
  }

  if (index > -1) {
    NSDictionary *newHighScore = @{ DictTotalScore : @(numTotalScore),
                    DictTurnsSurvived : @(numTurnSurvived),
                    DictUnitsKilled : @(numUnitsKilled) };

    arrScores.insertObject(newHighScore, index);
    arrScores.removeLastObject();

    CC_APPDB()->setObjectForKey(DataHighScores, arrScores);
    CC_APPDB()->flush();
  }

  return index;
#endif
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  //right here:
  auto hsIndex = saveHighScore();
  //kenl
  //rubberBandToScene([GameOverScene sceneWithScoreData:scoreData] fromParent:self withDuration:0.5f withDirection:kMoveDirectionUp];
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::rubberBandToScene() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const c::Vec2 &loc) {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  c::Ref *r=CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto u=SCAST(Unit*,r);
    if (u->getBoundingBox().containsPoint(loc)) {
      u->touchEnd(loc);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const c::Vec2 &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  c::Ref *r=CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto u=SCAST(Unit*,r);
    if (u->getBoundingBox().containsPoint(loc)) {
      u->touchMotion(loc);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  c::Ref *r=CC_NIL;
  CCARRAY_FOREACH(ss->friends,r) {
    auto u=SCAST(Unit*,r);
    if (u->getBoundingBox().containsPoint(loc)) {
      u->touchStart(loc);
    }
  }
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
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
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




