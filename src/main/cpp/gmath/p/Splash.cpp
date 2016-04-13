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

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(gmath)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto bg= c::LayerColor::create(c::Color4B::BLACK,
                         wz.width*5, wz.height*5);

  addItem(bg, -1);
  regoAtlas("game-pics");

  auto title= cx::reifyBmfLabel("title", "GMath");
  CC_POS2(title, wb.cx, wb.top * 0.8);
  addItem(title);

  auto board= cx::reifySprite("imgBoard.png");
  CC_POS2(board, wb.cx, wb.cy);
  addAtlasItem("game-pics", board);

  auto b= cx::reifyMenuText("btns", "PLAY");
  XCFG()->scaleNode(b, 36);
  CC_POS2(b, wb.cx, wb.top * 0.2);
  b->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  addItem(cx::mkMenu(b));

}


NS_END


