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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "n/Unit.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(gmath)


#define KeyFinishedTutorial "keyFinishedTutorial"
#define KeySound "keySound"
#define KeyMusic "keyMusic"
#define DataHighScores "highScores"
#define DictTotalScore "totalScore"
#define DictTurnsSurvived "turnsSurvived"
#define DictUnitsKilled "unitsKilled"
#define DictHighScoreIndex "hsIndex"

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {

  auto bg= c::LayerColor(c::Color4B(c::Color3B::WHITE),
                         wz.width*5,
                         wz.height*5);
  auto wb=cx::visBox();

  bg->setPosition(wb.cx, wb.cy);
  addItem(bg, -2);

  auto lblTurnsSurvivedDesc = cx::reifyBmfLabel("dft", "Turns Survived:");
  lblTurnsSurvivedDesc->setPosition(wb.right * 0.125, wb.top * 0.9);
  addItem(lblTurnsSurvivedDesc);

  _lblTurnsSurvived= cx::reifyBmfLabel("dft","0");
  _lblTurnsSurvived->setPosition(wb.right * 0.125, wb.top * 0.82);
  addItem(_lblTurnsSurvived);

  auto lblUnitsKilledDesc= cx::reifyBmfLabel("dft","Units Killed:");
  lblUnitsKilledDesc->setPosition(wb.right * 0.125, wb.top * 0.7);
  addItem(lblUnitsKilledDesc);

  _lblUnitsKilled= cx::reifyBmfLabel("dft","0");
  _lblUnitsKilled->setPosition(wb.right * 0.125, wb.top * 0.62);
  addItem(_lblUnitsKilled);

  auto lblTotalScoreDesc= cx::reifyBmfLabel("dft","Total Score:");
  lblTotalScoreDesc->setPosition(wb.right * 0.125, wb.cy);
  addItem(lblTotalScoreDesc);

  _lblTotalScore= cx::reifyBmfLabel("dft","1");
  _lblTotalScore->setPosition(wb.right * 0.125, wb.top * 0.42);
  addItem(_lblTotalScore);

  c::Color3B c(52/255.0, 73/255.0, 94/255.0);
  lblTurnsSurvivedDesc->setColor(c);
  _lblTurnsSurvived->setColor(c);
  lblTotalScoreDesc->setColor(c);
  _lblTotalScore->setColor(c);
  lblUnitsKilledDesc->setColor(c);
  _lblUnitsKilled->setColor(c);

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

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    board->setScale(0.8);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto friendly = Unit::friendlyUnit();
  friendly->setPosition(getPosForGridCoord(friendly->_gridPos));
  addAtlasItem("game-pics", friendly);

  ss->arrFriendlies.push_back(friendly);
  ss->arrEnemies.clear();

  _numTotalScore = 1;
  _numTurnSurvived = 0;
  _numUnitsKilled = 0;

  addObserver(this,@selector(moveUnit),
      kTurnCompletedNotification, CC_NIL);

  if (CC_APPDB()->getBoolForKey(KeyFinishedTutorial)) {
    spawnNewEnemy(getRandomEnemy());
    _tutorialPhase = 0;
  } else {
      //spawn enemy on far right with value of 1
    auto newEnemy= Unit::enemyUnitWithNumber(1, c::Vec2(9, 5));
    newEnemy->setPosition(getPosForGridCoord(newEnemy->_gridPos));
    newEnemy->setDirection(DirLeft); //2 is right wall
    spawnNewEnemy(newEnemy);
    _tutorialPhase = 1;
    showTutorialInstructions();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showTutorialInstructions() {
  sstr tutString = "";
  if (_tutorialPhase == 1) {
    tutString = "Drag Friendly Units";
    auto lblHowToPlay= cx::reifyBmfLabel("dft","How to Play:");
    lblHowToPlay->setColor(c::Color3B(52/255, 73/255, 94/255));
    lblHowToPlay->setPosition(getPosForGridCoord(c::Vec2(5,1)));
    lblHowToPlay->setName("lblHowToPlay");
    lblHowToPlay->setScale(0.8);
    addItem(lblHowToPlay, 2);

    auto bgHowTo=
      c::Sprite9Slice::createWithSpriteFrameName("imgUnit.png");
    bgHowTo->setMargin(0.2);
    bgHowTo->setPosition(0.5,0.4);
    //bgHowTo->setPositionType(c::PositionTypeNormalized);
    bgHowTo->setContentSize(c::Size(1.05,1.2));
    //bgHowTo.contentSizeType = CCSizeTypeNormalized;
    lblHowToPlay->addChild(bgHowTo,-1);
  }
  else if (_tutorialPhase == 2) {
    tutString = "Combine Friendly Units";

    auto fadeRemoveHowToPlay =
      c::Sequence::create(
          c::EaseInOut::create(c::FadeOut::create(0.5), 2),
          c::RemoveSelf::create(true),
          CC_NIL);
    getChildByName("lblHowToPlay")->runAction(fadeRemoveHowToPlay);
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

  auto lblTutorialText= cx::reifyBmfLabel("dft",tutString);
  lblTutorialText->setColor(c::Color3B(52/255,73/255,94/255));
  lblTutorialText->setPosition(getPosForGridCoord(c::Vec2(5,2)));
  lblTutorialText->setName("tutorialText");
  addItem(lblTutorialText,2);

  auto bg =
    c::Sprite9Slice::createWithSpriteFrameName("imgUnit.png");
  background->setMargin(0.2);
  background->setPosition(0.5,0.4);
  //background.positionType = CCPositionTypeNormalized;
  background->setContentSize(c::Size(1.05f, 1.2f));
  //background.contentSizeType = CCSizeTypeNormalized;
  lblTutorialText->addChild(bg,-1);

  auto finger = cx::reifySprite("imgFinger.png");
  finger->setAnchorPoint(c::Vec2(0.4,1));
  finger->setPosition(getPosForGridCoord(c::Vec2(5,5)));
  finger->setName("finger");
  finger->setOpacity(0);
  addAtlasItem("game-pics",finger,2);
  runFingerArrowActionsWithFinger(finger);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::runFingerArrowActionsWithFinger(c::Sprite *finger) {
  auto u = Unit::friendlyUnit();
  if (_tutorialPhase == 1 || _tutorialPhase == 3) {
    auto slideRight= c::Sequence::create(
        c::EaseIn::create(c::FadeIn::create(0.25),2),
        c::EaseInOut::create(
          c::MoveBy::create(1, c::Vec2(u->_gridWidth*2, 0)), 2),
        c::Delay::create(0.5),
        CC_NIL);

    auto fadeOutAndReposition =
      c::Sequence::create(
          c::Delay::create(0.25),
          c::EaseInOut::create(
            c::FadeOut::create(1), 2),
          c::Delay::create(0.5),
          c::CallFunc::create([=]() {
            finger->setPosition(getPosForGridCoord(c::Vec2(5,5)));
            }),
          CC_NIL);

    finger->runAction(
        c::RepeatForever::create(slideRight));
    finger->runAction(
        c::RepeatForever::create(fadeOutAndReposition));
  }
  else if (_tutorialPhase == 2) {
    finger->setPosition(getPosForGridCoord(c::Vec2(6,5)));
    auto slideLeft= c::Sequence::create(
        c::EaseIn::create(
          c::FadeIn::create(0.25), 2),
        c::EaseInOut::create(
          c::MoveBy::create(1, c::Vec2(-u->_gridWidth*2, 0)), 2),
        c::Delay::create(0.5),
        CC_NIL);

    auto fadeOutAndReposition = c::Sequence::create(
        c::Delay::create(0.25),
        c::EaseInOut::create(
          c::FadeOut::create(1), 2),
        c::Delay::create(0.5),
        c::CallFunc::create([=]() {
          finger->setPosition(getPosForGridCoord(c::Vec2(6,5)));
          }),
        CC_NIL);

    finger->runAction(
        c::RepeatForever::create(slideLeft));
    finger->runAction(
        c::RepeatForever::create(fadeOutAndReposition));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::advanceTutorial() {
  ++_tutorialPhase;
  removePreviousTutorialPhase();

  if (_tutorialPhase < 7) {
    showTutorialInstructions();
  } else {
    //the tutorial should be marked as "visible"
    CC_APPDB()->setKeyForBool(KeyFinishedTutorial,true);
    CC_APPDB()->flush();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::removePreviousTutorialPhase() {
  auto lblInstructions = (cc::Label*)getChildByName("tutorialText");
  lblInstructions->setName("old_instructions");

  auto fadeRemoveInstructions = c::Sequence::create(
      c::EaseInOut::create(
        c::FadeTo::create(0.5, 0), 2),
      c::RemoveSelf::create(true),
      CC_NIL);
  lblInstructions->runAction(fadeRemoveInstructions);

  auto finger = (c::Sprite*)getChildByName("finger");
  finger->setName("old_finger");
  auto fadeRemoveFinger = c::Sequence::create(
      c::EaseInOut::create(
        c::FadeTo::create(0.5,0), 2),
      c::RemoveSelf::create(true),
      CC_NIL);
  finger->runAction(fadeRemoveFinger);
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

  auto unitValue = (rand() % _upperBound) + 1;

  auto newEnemy = Unit::enemyUnitWithNumber(unitValue, c::Vec2(xPos, yPos));
  newEnemy->setPosition(getPosForGridCoord(newEnemy->_gridPos));
  newEnemy->setDirectionBasedOnWall(wall);

  return newEnemy;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnNewEnemy(Unit *enemy) {
  addAtlasItem("game-pics",enemy);
  ss->arrEnemies.push_back(enemy);
  enemy->setScale(0);
  pulseUnit(enemy);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::goToMenu() {
  cx::sfxPlay("click");
  rubberBandToScene(this, 0.5, kMoveDirectionDown);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::restartGame() {
  cx::sfxPlay("click");
  cx::runEx(Game::reify(new GameCtx()));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 GLayer::getPosForGridCoord(const c::Vec2 &pos) {
  auto u = Unit::friendlyUnit();
  auto ux= u->getBoundingBox();
  auto wz= cx::visSize();
  auto borderValue = 0.6;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    borderValue = 0.75;
  }

  return c::Vec2(
    wz.width * 0.625 + (ux.size.width + borderValue) * (pos.x-5),
    wz.height/2 - (ux.size.width + borderValue) * (pos.y-5));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::slideAllUnitsWithDistance(float dist, int dir) {
  for (Unit* u in ss->arrFriendlies) {
    u->slideUnitWithDistance(dist, dir);
  }

  for (Unit* u in arrEnemies) {
    u->slideUnitWithDistance(dist, dir);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveUnit(NSNotification *notif) {

  if (_tutorialPhase == 5 || _tutorialPhase == 6) {
    advanceTutorial();
  }

  NSDictionary *userInfo = [notif userInfo];
  auto u = (Unit*)userInfo[@"unit"];
  u->setPosition(getPosForGridCoord(u->_gridPos));

  cx::sfxPlay("moveUnit");
  ++numTurnSurvived;

  checkForNewFriendlyUnit();
  moveAllUnits();

  auto rate = 3; //spawn a unit every 3 turns

  if (_numTurnSurvived % rate == 0 || ss->arrEnemies.count() == 0) {
    if (_tutorialPhase == 4) {
      auto newEnemy = Unit::enemyUnitWithNumber(4, c::Vec2(5,9));
      newEnemy->setDirection(DirUp);
      newEnemy->setPosition(getPosForGridCoord(c::Vec2(5,9)));
      spawnNewEnemy(newEnemy);
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
  checkForAllCollisions();

  updateLabels();

  auto hasUnitAtCenter = false;
  for (Unit* u in ss->arrFriendlies) {
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
void GLayer::checkForNewFriendlyUnit() {
  //if there's a unit standing, that means there's one at the center, so return
  //don't worry about (5,5), as that should be a "combine" case if it ever were to come up...
  for (Unit *friendly in ss->arrFriendlies) {
    if (friendly->direction == DirStanding) {
      return;
    }
  }

  auto newFriendly = Unit::friendlyUnit();
  newFriendly->setPosition(getPosForGridCoord(newFriendly->_gridPos));
  addAtlasItem("game-pics",newFriendly, 1);
  ss->arrFriendlies.addObject(newFriendly);
  ++numTotalScore;
  newFriendly->setName("new");
  newFriendly->setScale(0);
  pulseUnit(newFriendly);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::updateLabels() {
  _lblTotalScore->setString(FTOS(numTotalScore));
  _lblTurnsSurvived->setString(FTOS(numTurnSurvived));
  _lblUnitsKilled->setString(FTOS(numUnitsKilled));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::moveAllUnits() {

  c::Array arrTaggedForDeletion;

  for (Unit *friendly in ss->arrFriendlies) {

    if (friendly->getName() == "new") {
      friendly->setName("");
    }
    else
    if (friendly->moveUnitDidIncreaseNumber()) {
      ++numTotalScore;
    }

    friendly->setPosition(getPosForGridCoord(friendly->_gridPos));

    if (friendly->_unitValue == 0) {
      arrTaggedForDeletion.addObject(friendly);
    }
    //if he's not already dead
    else if (! friendly->getName() == "dead") {
      for (Unit *other in ss->arrFriendlies) {
        //if other unit... and neither are dead... and
        checkForCombineWithUnit(friendly, other, arrTaggedForDeletion);
      }
    }
  }

  ss->arrFriendlies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }

  arrTaggedForDeletion.removeAllObjects();

  checkForDirectionalCollisions();

  //move all enemies
  for (Unit *enemy in arrEnemies) {
    enemy->moveUnitDidIncreaseNumber();
    enemy->setPosition(getPosForGridCoord(enemy->_gridPos));
    enemy->setNewDirectionForEnemy();

    if (! enemy->getName() == "dead") {
      for (Unit *other in ss->arrEnemies) {
        checkForCombineWithUnit(enemy, other, arrTaggedForDeletion);
      }
    }
  }

  ss->arrEnemies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }

  checkForAllCollisions();
  checkForAllCombines();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAllCombines() {
  c::Array arrTaggedForDeletion;
  for (Unit *friendly in ss->arrFriendlies) {
    for (Unit *otherFriendly in ss->arrFriendlies) {
      if (friendly != otherFriendly)
        checkForAnyDirectionCombineWithUnit(friendly, otherFriendly, arrTaggedForDeletion);
    }
  }

  ss->arrFriendlies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }
  arrTaggedForDeletion.removeAllObjects();

  for (Unit *enemy in ss->arrEnemies) {
    for (Unit *otherEnemy in ss->arrEnemies) {
      if (enemy != otherEnemy)
        checkForAnyDirectionCombineWithUnit(enemy, otherEnemy, arrTaggedForDeletion);
    }
  }

  ss->arrEnemies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }
  arrTaggedForDeletion.removeAllObjects();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAnyDirectionCombineWithUnit(Unit *first,
    Unit *other, c::Array *array) {

  if (other != first &&
    !(other->getName() == "dead") &&
    !(first->getName() == "dead") &&
    first->_gridPos.x == other->_gridPos.x &&
    first->_gridPos.y == other->_gridPos.y) {

    auto fv = first->_unitValue;
    auto ov = other->_unitValue;

    if (first->_isFriendly) {
      playUnitCombineSoundWithValue(fv+ov);
    }

    if (ov > fv) {
      first->setName("dead");
      array.addObject(first);
      other->_unitValue += fv;
      first->_direction = other->_direction;
      other->updateLabel();
      pulseUnit(other);
    } else {
      other->setName("dead");
      array.addObject(other);
      first->_unitValue += ov;
      other->_direction = first->_direction;
      first->updateLabel();

      pulseUnit(first);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForCombineWithUnit(Unit *first, Unit *other, c::Array *array) {
  if (other != first &&
    !(other->getName() == "dead") &&
    !(first->getName() == "dead") &&
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
        playUnitCombineSoundWithValue(fv+ov);
      }

      if (ov > fv) {
        first->setName("dead");
        array.addObject(first);
        other->_unitValue += fv;
        first->_direction = other->_direction;
        other->updateLabel();

        pulseUnit(other);
      } else {
        other->setName("dead");
        array.addObject(other);
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

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    baseScale = 0.8;
  }

  auto scaleUp = c::EaseInOut::create(
      c::ScaleTo::create(0.15 , baseScale * 1.2), 2);
  auto scaleDown = c::EaseInOut::create(
      c::ScaleTo::create(0.15, baseScale * 0.9), 2);
  auto scaleToFinal = c::EaseInOut::create(
      c::ScaleTo::create(0.25, baseScale), 2);

  unit->stopAllActions();
  unit->runAction(
      c::Sequence::create(scaleUp, scaleDown, scaleToFinal, CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForDirectionalCollisions() {
  c::Array arrTaggedForDeletion;

  for (Unit *f in ss->arrFriendlies) {
    for (Unit *e in ss->arrEnemies) {
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
          handleCollisionWithFriendly(f, e, arrTaggedForDeletion, true);
          //exit the for so no "bad things" happen
          //goto afterDirCollide;
          break;
        }
      }
    }
    //afterDirCollide:{}
  }

  ss->arrFriendlies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::checkForAllCollisions() {
  c::Array arrTaggedForDeletion;
  for (Unit *f in ss->arrFriendlies) {
    for (Unit *e in ss->arrEnemies) {
      //at same coordinate...
      if (f->_gridPos.x == e->_gridPos.x &&
        f->_gridPos.y == e->_gridPos.y) {
        //collision!
        handleCollisionWithFriendly(f, e, arrTaggedForDeletion,false);

        //exit the for so no "bad things" happen
        //goto afterAllCollide;
        break;
      }
    }
    //afterAllCollide:{}
  }

  ss->arrFriendlies.removeObjectsInArray(arrTaggedForDeletion);
  ss->arrEnemies.removeObjectsInArray(arrTaggedForDeletion);
  for (Unit *u in arrTaggedForDeletion) {
    removeItem(u);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::handleCollisionWithFriendly(Unit *friendly, Unit *enemy,
    c::Array *array, bool isDirectional) {

  auto fv = friendly->_unitValue;
  auto ev = enemy->_unitValue;

  if (isDirectional) {
    enemy->_unitValue -= fv;
    friendly->_unitValue -= (ev+1);
  } else {
    friendly->_unitValue -= ev;
    enemy->_unitValue -= fv;
  }

  friendly->updateLabel();
  enemy->updateLabel();

  if (friendly->_unitValue <= 0) {
    array.addObject(friendly);
  }

  if ((enemy->_unitValue <= 0 && !isDirectional) ||
    (enemy->_unitValue < 0 && isDirectional)) {
    arrEnemies.removeObject(enemy);
    removeItem(enemy);
    ++numUnitsKilled;

    if (_tutorialPhase == 3 || _tutorialPhase == 4) {
      advanceTutorial();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::playUnitCombineSoundWithValue(int total) {
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
}

//////////////////////////////////////////////////////////////////////////////
//
c::Image* GLayer::takeScreenshot() {

  CC_DTOR()->setNextDeltaTimeZero(true);

  auto wz= cx::visSize();
  c::RenderTexture *rtx =
    new c::RenderTexture(wz.width, wz.height);
  rtx->begin();
  //[startNode visit];
  CC_DTOR()->getRunningScene()->visit();
  rtx->end();

  return rtx->getUIImage();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::endGame() {
  //right here:
  auto hsIndex = saveHighScore();
  auto image = takeScreenshot();

  NSDictionary *scoreData = @{
DictTotalScore : @(numTotalScore),
                 DictTurnsSurvived : @(numTurnSurvived),
                 DictUnitsKilled : @(numUnitsKilled),
                 DictHighScoreIndex : @(hsIndex),
                 @"screenshot" : image};

  //kenl
  //rubberBandToScene([GameOverScene sceneWithScoreData:scoreData] fromParent:self withDuration:0.5f withDirection:kMoveDirectionUp];
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::rubberBandToScene() {
}


NS_END



