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
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Popups.h"
#include "Splash.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
Popup::Popup() {
  setScale(0);
  runAction(c::EaseBackOut::create(c::ScaleTo::create(0.25, 1)));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Menu* Popup::addMenu() {

  if (E_NIL(_menu)) {
    _menu = c::Menu::create();
    _menu->setAnchorPoint(cx::anchotBL());
    _menu->setPosition(CC_ZPT);
    addChild(_menu);
  }

  return _menu;
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* Popup::addButton(c::MenuItem *button, const CCT_PT &position) {

  if ( addMenu() == CC_NIL || button == CC_NIL ) {
  return CC_NIL; }

  // position the button & add to menu_
  button->setPosition(position);
  _menu->addChild(button);
  return button;
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* Popup::addLabelButton(const sstr &text,
    const CCT_PT &position, BTNCB handler) {

  return addLabelButton(text, "btns", position, handler);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* Popup::addLabelButton(const sstr &text, const sstr &font,
    const CCT_PT &position, BTNCB handler) {

  auto b= cx::reifyMenuText(font, text);
  XCFG()->scaleBmfont(b, 36);
  b->setCallback(handler);
  return addButton(b, position);
}

//////////////////////////////////////////////////////////////////////////////
//
c::MenuItem* Popup::addSpriteButton(const sstr &frame,
    const CCT_PT &position, BTNCB handler) {

  auto b= cx::reifyMenuBtn(frame);
  XCFG()->fit(b);
  b->setCallback(handler);
  return addButton(b, position);
}

//////////////////////////////////////////////////////////////////////////////
//
void Popup::resumeGame(c::Node *sender) {
  //game_world_->ResumeGame();
  this->runAction(
      c::Sequence::create(
        c::EaseBackIn::create(
          c::ScaleTo::create(0.25, 0)),
        c::RemoveSelf::create(true),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Popup::restartGame(c::Node *sender) {
  removeFromParentAndCleanup(true);
  cx::runEx(Game::reify(mc_new(GameCtx)));
}

//////////////////////////////////////////////////////////////////////////////
//
void Popup::nextLevel(c::Node *sender) {
  ++ss->levelNumber;
  removeFromParentAndCleanup(true);
  cx::runEx(Game::reify(mc_new(GameCtx)));
}

//////////////////////////////////////////////////////////////////////////////
//
void Popup::quitToMainMenu(c::Node *sender) {
  removeFromParentAndCleanup(true);
  cx::prelude();
}

//////////////////////////////////////////////////////////////////////////////
//
PausePopup::PausePopup() {
  ss->popupActive = true;

  // add the title/message of the popup
  auto label = cx::reifyBmfLabel("title", "Game Paused");
  XCFG()->scaleBmfont(label, 52);
  auto wb= cx::visBox();
  CC_POS2(label, wb.cx, wb.top*0.75);
  addChild(label);

  // create menu & buttons
  addMenu();
  addLabelButton("Continue", CCT_PT(wb.cx, wb.top*0.6),
      [=](c::Ref*) { this->resumeGame(); });
  addLabelButton("Restart", CCT_PT(wb.cx, wb.cy),
      [=](c::Ref*) { this->restartGame(); });
  addLabelButton("Main Menu", CCT_PT(wb.cx, wb.top*0.4),
      [=](c::Ref*) { this->quitToMainMenu(); });
}

//////////////////////////////////////////////////////////////////////////////
//
owner<PausePopup*> PausePopup::create() {
  auto pause_popup = mc_new(PausePopup);
  pause_popup->initWithColor(c::ccc4(0, 0, 0, 128));
  pause_popup->autorelease();
  return pause_popup;
}

//////////////////////////////////////////////////////////////////////////////
//
LevelCompletePopup::LevelCompletePopup() {
  ss->popupActive = true;
  // add the title/message of the popup
  auto label = cx::reifyBmfLabel("title", "Stage Complete!");
  XCFG()->scaleBmfont(label, 52);
  CC_POS2(label, wb.cx, wb.top*0.75);
  addChild(label);

  addMenu();
  addLabelButton("Restart", CCT_PT(wb.cx, wb.top*0.4),
      [=](c::Ref*) { this->restartGame(); });
  addLabelButton("Main Menu", CCT_PT(wb.cx, wb.top*0.3),
      [=](c::Ref*) { this->QuitToMainMenu(); });
  addLabelButton("Next Level", CCT_PT(wb.cx, wb.cy),
      [=](c::Ref*) { this->nextLevel(); });
}

//////////////////////////////////////////////////////////////////////////////
//
owner<LevelCompletePopup*> LevelCompletePopup::create() {
  auto level_complete_popup = mc_new(LevelCompletePopup);
  level_complete_popup->initWithColor(c::ccc4(0, 0, 0, 128));
  level_complete_popup->autorelease();
  return level_complete_popup;
}

//////////////////////////////////////////////////////////////////////////////
//
GameOverPopup::GameOverPopup() {
  ss->popupActive = true;

  // add the title/message of the popup
  auto label = cx::reifyBmfLabel("title", "Game Over!");
  XCFG()->fit(label, 52);
  CC_POS2(label, wb.cx, wb.top*0.75);
  addChild(label);

  addMenu();
  addLabelButton("Restart", CCT_PT(wb.cx, wb.cy),
      [=](c::Ref*) { this->restartGame(); });
  addLabelButton("Main Menu", CCT_PT(wb.cx, wb.top*0.4),
      [=](c::Ref*) { this->quitToMainMenu(); });
}

//////////////////////////////////////////////////////////////////////////////
//
owner<GameOverPopup*> GameOverPopup::create() {
  auto game_over_popup = mc_new(GameOverPopup);
  game_over_popup->initWithColor(c::ccc4(0, 0, 0, 128));
  game_over_popup->autorelease();
  return game_over_popup;
}



NS_END


