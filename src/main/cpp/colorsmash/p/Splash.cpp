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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "n/lib.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wz= cx::visRect();
  auto wb= cx::visBox();

  // create a coloured layer as background
  auto background = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255),
      CC_ZW(wz.size),
      CC_ZH(wz.size));
  addItem(background);

  // create a label to display the name of the game
  auto titleLabel = cx::reifyLabel("dft", 64, "Color-Smash");
  titleLabel->setPosition(wb.cx, wb.top * 0.8);
  addItem(titleLabel, 1);

  // create a play button to move to the game world
  auto playButton = cx::reifyMenuBtn("play_button.png");
  auto menu=cx::mkMenu(playButton);
  playButton->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(new f::GCX() ));
      });
  playButton->setPosition(wb.cx,wb.cy);
  addItem(menu, 1);

  this->schedule(CC_SCHEDULE_SELECTOR(Splash::doAnimation), 2);
  doAnimation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::doAnimation(float) {
  auto numTiles = floor(cx::rand() * 30);
  for (auto i = 0;  i < numTiles; ++i) {
    auto tile = cx::reifySprite("tile.png");
    tile->setColor(getColorForTile(1 + floor(cx::rand() * MAX_COLORS)));
    tile->setPosition(getRandomPositionForTile());
    tile->setScale(0);
    addItem(tile);

    tile->runAction(c::Sequence::create(
          c::DelayTime::create(cx::rand() * 5),
          c::EaseBackOut::create(c::ScaleTo::create(0.125, 1)),
          c::DelayTime::create(cx::rand() * 5),
          c::EaseBackIn::create(c::ScaleTo::create(0.125, 0)),
          c::RemoveSelf::create(true), CC_NIL));
  }
}



NS_END


