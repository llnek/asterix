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
#include "n/C.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();

  // create a coloured layer as background
  auto background = c::LayerColor::create(
      c::Color4B(25, 0, 51, 255),
      CC_ZW(wz),
      CC_ZH(wz));
  addItem(background, -1);
  regoAtlas("game-pics");

  // create a label to display the name of the game
  auto titleLabel = cx::reifyBmfLabel("title", "Color-Smash");
  XCFG()->scaleBmfont(titleLabel,64);
  CC_POS2(titleLabel, wb.cx, wb.top * 0.8);
  addItem(titleLabel, 1);

  // create a play button to move to the game world
  auto b = cx::reifyMenuBtn("play_button.png");
  b->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  CC_POS2(b, wb.cx,wb.cy);
  addItem(cx::mkMenu(b), 1);

  doAnimation(0);
  this->schedule(CC_SCHEDULE_SELECTOR(Splash::doAnimation), 2);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::doAnimation(float) {
  auto numTiles = cx::randInt(30);
  for (auto i = 0;  i < numTiles; ++i) {
    auto tile = cx::reifySprite("tile.png");
    XCFG()->fit(tile);
    tile->setColor(getColorForTile(1 + cx::randInt(MAX_COLORS)));
    CC_POS1(tile, getRandomPositionForTile());
    tile->setOpacity(0);
    addAtlasItem("game-pics", tile);

    tile->runAction(c::Sequence::create(
          c::DelayTime::create(cx::randInt(5)),
          c::EaseBackOut::create(c::FadeTo::create(0.125, 255)),
          c::DelayTime::create(cx::randInt(5)),
          c::EaseBackIn::create(c::FadeTo::create(0.125, 0)),
          c::RemoveSelf::create(true),
          CC_NIL));
  }
}



NS_END


