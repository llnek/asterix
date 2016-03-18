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
#include "n/lib.h"
#include "LevelSelector.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  centerImage("game.bg");

  //interval between balloon messages
  _msgCntDown = 5;
  createScreen();

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {

  //update balloon messages

  if (!_caption2->isVisible()) {
    _msgCntDown += dt;
  }

  if (_msgCntDown > 6) {
    if (!_balloon->isVisible()) {
      cx::sfxPlay("cap");
      _msgCntDown = 0;
      CC_SHOW(_caption1);
      CC_SHOW(_balloon);
    }
    else
    if (!_caption2->isVisible()) {
      cx::sfxPlay("cap");
      CC_HIDE(_caption1);
      CC_SHOW(_caption2);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::showLevels() {
  auto newScene = c::TransitionMoveInR::create(0.2, LevelSelector::reify());
  cx::sfxPlay("button");
  CC_DTOR()->replaceScene(newScene);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::showHelp() {
  /*
  auto levelsCompleted = c::UserDefault::getInstance()->getIntegerForKey("levelsCompleted");
  auto newScene = c::TransitionMoveInR::create(0.2f,
      Game::reify(0, levelsCompleted));
  cx::sfxPlay("button");
  CC_DTOR()->replaceScene(newScene);
  */
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::createScreen() {

  auto snow = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("snow"));
  auto wb= cx::visBox();

  snow->setPosition(c::Vec2(wb.cx, wb.top));
  addItem(snow, kBackground);

  regoAtlas("game-pics", kMiddleground);

  auto ground = cx::reifySprite("intro_igloo.png");
  ground->setAnchorPoint(c::Vec2(0.5, 0.05));
  ground->setPosition(HTV(wb.right), 0.0);
  addAtlasItem("game-pics", ground);

  auto logo = cx::reifySprite("logo.png");
  logo->setPosition(wb.cx, wb.top * 0.75);
  addAtlasItem("game-pics", logo);

  //create menu
  auto play=cx::reifyMenuBtn(
      "btn_play_on.png", "btn_play_off.png", "btn_play_off.png" );
  play->setCallback(
      [=](c::Ref*) { this->showLevels(); });

  auto help= cx::reifyMenuBtn(
      "btn_help_on.png", "btn_help_off.png", "btn_help_off.png");
  help->setCallback(
      [=](c::Ref*){ this->showHelp(); });

  auto mainMenu= cx::mkVMenu(s_vec<c::MenuItem*> {play,help});
  mainMenu->setPosition(wb.cx, wb.cy);
  addItem(mainMenu, kForeground);

  //add balloon caption
  _balloon = cx::reifySprite("cap.png");
  _balloon->setPosition(wb.right * 0.36, wb.top * 0.25);
  addAtlasItem("game-pics", _balloon, kForeground);
  CC_HIDE(_balloon);

  _caption1 = cx::reifySprite("cap_supper.png");
  _caption2 = cx::reifySprite("cap_where.png");
  CC_HIDE(_caption1);
  CC_HIDE(_caption2);
  _balloon->addChild(_caption1);
  _balloon->addChild(_caption2);

  auto rect = _balloon->getTextureRect();
  _caption1->setPosition(rect.size.width * 0.42, rect.size.height * 0.45);
  _caption2->setPosition(rect.size.width * 0.42, rect.size.height * 0.45);

}




NS_END


