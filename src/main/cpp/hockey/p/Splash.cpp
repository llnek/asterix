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
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  auto title= cx::reifySprite("logo.png");
  CC_POS2(title, wb.cx, wb.top * 0.8);
  addAtlasItem("game-pics", title);

  auto p= cx::reifyMenuText("btns", "PLAY");
  XCFG()->scaleNode(p, 36);
  CC_POS2(p, wb.cx, wb.top * 0.2);
  p->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  addItem(cx::mkMenu(p));

}



NS_END


