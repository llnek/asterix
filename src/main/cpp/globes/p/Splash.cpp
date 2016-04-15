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
NS_BEGIN(globes)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto bg= c::LayerGradient::create(c::Color4B(0x00,0x22,0x22,255),
      c::Color4B(0x22,0x00,0x44,255));
  auto wb= cx::visBox();

  addItem(bg,-1);

  auto b= cx::reifyMenuText("btns", "PLAY");
  XCFG()->scaleBmfont(b,36);
  b->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  CC_POS2(b, wb.cx, wb.top * 0.2);
  addItem(cx::mkMenu(b));
}


NS_END


