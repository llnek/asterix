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

#include "spine/spine-cocos2dx.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
//#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_USING(sp, spine)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wb= cx::visBox();

  bgLayer = mc_new1(ScrollingBgLayer,3);
  addItem(bgLayer);

  title = cx::reifyBmfLabe("dft", "Bazuka");
  title->setPosition(wb.cx, wb.top * 0.8f);
  addItem(title);

  // animate title a bit
  moveXXX(20.f);

  // Uuuh????
  auto skeleton = sp::SkeletonAnimation::createWithFile( "pics/player.json", "pics/player.atlas", 1.0f);
  skeleton->addAnimation("runCycle",true,0,0);
  skeleton->setPosition(wb.right * .125f , wb.top * 0.2f - HHZ(CC_CSIZE(skeleton)));
  addItem(skeleton);

  //options
  auto p = cx::reifyMenuBtn("play.png",); //"_bookgame_UI_play.png",
  auto pMenu = cx:mkMenu(p);
  p->setCallback([=](c::Ref*){
      cx::sfxPlay("pop");
      //Game::reify(new GameCtx());
      });

  pMenu->setPosition(wb.cx, wb.cy);
  addItem(pMenu, 10);

  auto newHighScoreLabel = cx::reifyBmfLabel("dft", "CURRENT HIGH SCORE");
  newHighScoreLabel->setPosition(wb.cx, wb.top * 0.15f);
  newHighScoreLabel->setScale(0.5f);
  addItem(newHighScoreLabel, 10);

  auto highScoreLabel = cx::reifyBmfLabel("dft", "0");
  highScoreLabel->setPosition(wb.cx, wb.top * 0.1);
  highScoreLabel->setScale(0.5f);
  addItem(highScoreLabel, 10);

  auto score = CC_APPDB()->getIntegerForKey("GameHighScore");
  highScoreLabel->setString(s::to_string(score));

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {
  bgLayer->sync();
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::moveXXX(float yOff) {
  auto loc= title->getPosition();
  auto move = c::MoveTo::create(1.0f, c::Vec2(loc.x, loc.y + yOff));
  title->runAction(
      c::Sequence::create(
        c::EaseSineInOut::create((c::ActionInterval*)move),
        c::CallFunc::create([=](){
          this->moveXXX(-1 * yOff);
        }),
        CC_NIL));
}



NS_END


