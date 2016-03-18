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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"
#include "n/lib.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::toggleBtn(int tag, bool b) {
  F__LOOP(it, _buttons) {
    auto btn= *it;
    if (btn->getTag() == tag) {
       btn->setVisible(b);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showTut(const sstr &msg) {
  _tutorialLabel->setString(msg);
  CC_SHOW(_tutorialLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showMsg(const sstr &msg) {
  _messages->setString(msg);
  CC_SHOW(_messages);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showMenu() {
  CC_SHOW(_messages);
  CC_SHOW(_btnStart);
  CC_HIDE(_btnMenu);
  CC_HIDE(_btnAgain);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::toggleTutorial(bool b) {
  _tutorialLabel->setVisible(b);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::touchEnd(c::Touch *touch) {
  auto tap = touch->getLocation();
  F__LOOP(it, _buttons) {
    auto btn= *it;
    if (!btn->isVisible() ||
        !btn->boundingBox().containsPoint(tap)) {
      continue; }

    CC_HIDE(CC_GCT(btn,kSpriteBtnOn));
    switch (btn->getTag()) {
      case kSpriteBtnAgain:
        cx::sfxPlay("button");
        MGMS()->stop();
        CC_HIDE(_btnAgain);
        CC_HIDE(_btnMenu);
        SENDMSG("/game/player/resetlevel");
      break;
      case kSpriteBtnReset:
        MGMS()->stop();
        SENDMSG("/game/player/resetlevel");
      break;
      case kSpriteBtnPause:
        if (MGMS()->isLive()) {
          _messages->setString("paused");
          CC_SHOW(_messages);
        } else {
          CC_HIDE(_messages);
        }
        if (MGMS()->isLive()) { MGMS()->stop(); }
        else { MGMS()->play(); }
      break;
      case kSpriteBtnStart:
        cx::sfxPlay("button");
        CC_SHOW(_btnPause);
        CC_SHOW(_btnReset);
        CC_HIDE(_btnStart);
        CC_HIDE(_messages);
        MGMS()->play();
        if (ss->currentLevel == 0) {
          CC_SHOW(_tutorialLabel);
        }
      break;
      case kSpriteBtnMenu:
        cx::sfxPlay("button");
        cx::pauseMusic();
        SENDMSG("/game/unschedule");
        CC_DTOR()->replaceScene(
          c::TransitionMoveInL::create(0.2f, Splash::reify()));
      break;
    }
    return;
  }

  if (MGMS()->isLive()) {
    SENDMSG("/game/player/switchshape->true"); }
}

//////////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::touchStart(c::Touch *touch) {
  auto tap = touch->getLocation();
  F__LOOP(it,_buttons) {
    auto btn = *it;
    if (btn->isVisible()) {
      if (btn->boundingBox().containsPoint(tap)) {
        CC_SHOW(CC_GCT(btn,kSpriteBtnOn));
        return true;
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::createBtns() {

  auto wb= cx::visBox();
  auto wz=cx::visRect();
  c::Sprite *bOn;

  _btnStart = cx::reifySprite("btn_start_off.png");
  _btnStart->setPosition(wb.cx, wb.top * 0.45f);
  CC_HIDE(_btnStart);
  addAtlasItem("game-pics", _btnStart, kForeground, kSpriteBtnStart);
  //+
  bOn = cx::reifySprite("btn_start_on.png");
  bOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(bOn);
  _btnStart->addChild(bOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnStart);

  _btnReset = cx::reifySprite("btn_reset_off.png");
  _btnReset->setAnchorPoint(cx::anchorT());
  _btnReset->setPosition(wb.right * 0.9f, wb.top);
  CC_HIDE(_btnReset);
  addAtlasItem("game-pics", _btnReset, kForeground, kSpriteBtnReset);
  //+
  bOn = cx::reifySprite("btn_reset_on.png");
  bOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(bOn);
  _btnReset->addChild(bOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnReset);

  _btnPause = cx::reifySprite("btn_pause_off.png");
  _btnPause->setAnchorPoint(cx::anchorT());
  _btnPause->setPosition(wb.right * 0.1f, wb.top);
  CC_HIDE(_btnPause);
  addAtlasItem("game-pics", _btnPause, kForeground, kSpriteBtnPause);
  //+
  bOn = cx::reifySprite("btn_pause_on.png");
  bOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(bOn);
  _btnPause->addChild(bOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnPause);

  _btnAgain = cx::reifySprite("btn_again_off.png");
  _btnAgain->setPosition(wb.cx, wb.top * 0.45f);
  CC_HIDE(_btnAgain);
  addAtlasItem("game-pics", _btnAgain, kForeground, kSpriteBtnAgain);
  //+
  bOn = cx::reifySprite("btn_again_on.png");
  bOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(bOn);
  _btnAgain->addChild(bOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnAgain);

  _btnMenu = cx::reifySprite("btn_menu_off.png");
  _btnMenu->setPosition(wb.cx, wb.top * 0.3f);
  CC_HIDE(_btnMenu);
  addAtlasItem("game-pics", _btnMenu, kForeground, kSpriteBtnMenu);
  //+
  bOn = cx::reifySprite("btn_menu_on.png");
  bOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(bOn);
  _btnMenu->addChild(bOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnMenu);

  _messages = cx::reifyBmfLabel("font_msgs", "level");
  _messages->setAlignment(c::TextHAlignment::CENTER);
  _messages->setPosition(wb.cx, wb.top * 0.6f);
  CC_HIDE(_messages);
  addItem(_messages, kForeground);

  _tutorialLabel = cx::reifyLabel("dft", 25, TUTORIAL_1);
  _tutorialLabel->setAlignment(c::TextHAlignment::CENTER);
  _tutorialLabel->setPosition(wb.cx, wb.top * 0.2f);
  _tutorialLabel->setHeight(wz.size.height * 0.4f);
  _tutorialLabel->setWidth(wz.size.width * 0.7f);
  CC_HIDE(_tutorialLabel);
  addItem(_tutorialLabel, kForeground);

}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  regoAtlas("game-pics");

  createBtns();

  //scoreLabel= cx::reifyBmfLabel("font_msgs", "0");
  //addItem(scoreLabel);
  _score=0;
}


NS_END



